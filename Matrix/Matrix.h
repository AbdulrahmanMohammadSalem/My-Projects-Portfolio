// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

////////////////////////////////////////////////////////////////
//                __  __       _        _                     //
//               |  \/  | __ _| |_ _ __(_)_  __               //
//               | |\/| |/ _` | __| '__| \ \/ /               //
//               | |  | | (_| | |_| |  | |>  <                //
//               |_|  |_|\__,_|\__|_|  |_/_/\_\               //
//                                                            //
////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <unordered_set>
#include <type_traits>
#include <cmath>
#include <sstream>
#include <iomanip>

template <class T>
class Matrix {
public:
	enum ErrorCodes : short {
		SUCCESS, INVALID_DIMENSIONS, INVALID_EMPTY_MATRIX, INVALID_INDICES, NO_INVERSE_EXISTS, INVALID_POSITION
	};

	struct Result_Mat;
	struct Result_Val;
	struct Result_Split;
	struct Result_Split_Quadrants;

	struct Result_Vct {
		std::vector<T> vct;
		ErrorCodes error_code;
	};

	struct Position {
		size_t row, column;
	};

	struct Dimensions {
		size_t rows, columns;
	};

private:
	std::vector<std::vector<T>> _InnerVector;

	static bool _checkMatrixDimensions(const Matrix<T> & _mat) {
		for (const auto & row : _mat._InnerVector)
			if (row.size() != _mat._InnerVector.front().size() || row.empty())
				return false;

		return true;
	}

	static bool _checkMatricesDimensions_Multiplication(const Matrix<T> & _mat1, const Matrix<T> & _mat2) {
		return _mat1._InnerVector.front().size() == _mat2._InnerVector.size();
	}

	static bool _checkMatricesDimensions_Same(const Matrix<T> & _mat1, const Matrix<T> & _mat2) {
		return _mat1._InnerVector.size() == _mat2._InnerVector.size() && _mat1._InnerVector.front().size() == _mat2._InnerVector.front().size();
	}

	static bool _isMatEmpty(const Matrix<T> & _mat) {
		return !_mat._InnerVector.size();
	}

	bool _checkIdentityMultiple(const T _multiple) const {
		if (!isSquareMatrix())
			return false;

		for (size_t i = 0; i < _InnerVector.size(); i++)
			for (size_t j = 0; j < _InnerVector.size(); j++)
				if (i == j ? _InnerVector[i][j] != _multiple : _InnerVector[i][j] != 0)
					return false;

		return true;
	}

	static std::pair<size_t, size_t> _getTotalRows_Columns(const std::vector<Matrix<T>> & matrices) {
		std::pair<size_t, size_t> _sum = { 0, 0 };

		for (const auto & M : matrices) {
			_sum.first += M._InnerVector.size();
			_sum.second += M._InnerVector.front().size();
		}
		
		return _sum;
	}

	std::string _vctToStr(const std::vector<T> & _vct, const short _precision, const std::string & _separator) const {
		if (_vct.empty())
			return "()";

		std::ostringstream _oss;
		
		if (std::is_arithmetic_v<T>) {
			_oss << "(" << std::fixed << std::setprecision(_precision) << _vct.front();

			for (size_t i = 1; i < _vct.size(); i++)
				_oss << _separator << std::fixed << std::setprecision(_precision) << _vct[i];
		}
		else {
			_oss << "(" << _vct.front();

			for (size_t i = 1; i < _vct.size(); i++)
				_oss << _separator << _vct[i];
		}
		
		_oss << ')';
		return _oss.str();
	}

	static bool _hasDuplicates(const std::vector<size_t> & _vct) {
		std::unordered_set<size_t> _set;

		for (const size_t e : _vct)
			if (!_set.insert(e).second)
				return true;

		return false;
	}

	Result_Mat _additionBase(const Matrix<T> &, const bool) const;

	static bool _isWhole(const float num) {
		return static_cast<int>(num) == num;
	}

	//No validation here.
	static Matrix<T> _getSubMatrix(const Matrix<T> & parent, const size_t row, const size_t column) {
		Matrix<T> _result(parent._InnerVector.size() - 1, parent._InnerVector.size() - 1, 0);
		std::vector<T> _acceptedElements;

		for (size_t i = 0; i < parent._InnerVector.size(); i++)
			for (size_t j = 0; j < parent._InnerVector.size(); j++)
				if (i != row && j != column)
					_acceptedElements.push_back(parent._InnerVector[i][j]);

		for (size_t e = 0, i = size_t(-1), j = parent._InnerVector.size() - 1; e < _acceptedElements.size(); e++, j++) {
			if (_isWhole((float) e / (float) (parent._InnerVector.size() - 1))) {
				i++; //As if '\n'
				j -= parent._InnerVector.size() - 1;
			}

			_result._InnerVector[i][j] = _acceptedElements[e];
		}

		return _result;
	}

	//Validation must be done outside, this function supposes square matrices
	static T _getMatrixDeterminant(const Matrix<T> & mat) {
		if (mat._InnerVector.size() == 1)
			return mat._InnerVector.front().front();

		if (mat._InnerVector.size() == 2)
			return mat._InnerVector[0][0] * mat._InnerVector[1][1] - mat._InnerVector[0][1] * mat._InnerVector[1][0];

		T _result = 0;

		for (size_t i = 0; i < mat._InnerVector.size(); i++)
			_result += (i % 2 ? -1 : 1) * mat._InnerVector[i].front() * _getMatrixDeterminant(_getSubMatrix(mat, i, 0));

		return _result;
	}

	static size_t _getFirstNonEmptyMatrixPosition(const std::vector<Matrix<T>> & _matrices) {
		for (size_t i = 0; i < _matrices.size(); i++)
			if (!_matrices[i].isEmpty())
				return i;

		return size_t(-1);
	}

	//Empty matrices will be ignored -- (0,1) = (horizontally,vertically)
	static Result_Mat _concatenateMatrices(const std::vector<Matrix<T>> &, const bool &);

	//(0,1) = (right,left)
	static void _rotateElements_Vct(std::vector<T> & _vct, size_t steps, const bool direction) {
		std::vector<T> _result;
		size_t factor = 0;

		while (steps >= _vct.size())
			steps -= _vct.size();

		if (direction) {
			for (size_t i = 0; i < _vct.size(); i++) {
				if (i + steps - factor * _vct.size() >= _vct.size())
					factor++;

				_result.push_back(_vct[i + steps - factor * _vct.size()]);
			}

			_vct = _result;
		}
		else
			_rotateElements_Vct(_vct, _vct.size() - steps, 1);
	}

	//Bubble Sort
	static void _sortVct(std::vector<T> & _vct, const bool reverse) {
		bool didSwapOccur = true;

		if (reverse) {
			while (didSwapOccur) {
				didSwapOccur = false;

				for (unsigned short i = 1; i < _vct.size(); i++)
					if (_vct[i] > _vct[i - 1]) {
						std::swap(_vct[i], _vct[i - 1]);
						didSwapOccur = true;
					}
			}
		}
		else {
			while (didSwapOccur) {
				didSwapOccur = false;

				for (unsigned short i = 1; i < _vct.size(); i++)
					if (_vct[i] < _vct[i - 1]) {
						std::swap(_vct[i], _vct[i - 1]);
						didSwapOccur = true;
					}
			}
		}
	}

	//(0,1) = (right,left)
	Result_Mat _shiftRow(const size_t, const size_t, const bool) const;

	//(0,1) = (down,up)
	Result_Mat _shiftColumn(const size_t, const size_t, const bool) const;

	//(0,1) = (right,left)
	Result_Mat _shiftHorizontally(const size_t, const bool) const;

	//(0,1) = (down,up)
	Result_Mat _shiftVertically(const size_t, const bool) const;

public:
	//Constructs an empty matrix
	Matrix(){}

	//If the sizes of the vectors are not valid, and empty matrix will be constructed
	Matrix(std::initializer_list<std::vector<T>> initializer_list) {
		_InnerVector.assign(initializer_list);

		if (!_checkMatrixDimensions(*this))
			_InnerVector = {};
	}

	//Constructs a matrix from a nested vector
	//If the sizes of the vectors are not valid, and empty matrix will be constructed
	Matrix(std::vector<std::vector<T>> nested_vector) {
		_InnerVector = nested_vector;

		if (!_checkMatrixDimensions(*this))
			_InnerVector = {};
	}

	//Construct a diagonal matrix with specified diagonal-values
	Matrix(const std::vector<T> & diagonal_values) {
		static_assert(std::is_arithmetic_v<T>, "constructBlockDiagonalMatrix(const std::vector<T>&) is not available for the specified type.");

		for (size_t i = 0; i < diagonal_values.size(); i++) {
			_InnerVector.push_back(std::vector<T>(diagonal_values.size(), 0));

			for (size_t j = 0; j < diagonal_values.size(); j++)
				if (i == j) _InnerVector[i][j] = diagonal_values[i];
		}
	}

	//Constructs a square matrix with a specified filler-value
	Matrix(const size_t dimension, const T filler_value) {
		*this = Matrix<T>(dimension, dimension, filler_value);
	}

	//Constructs a matrix with specified dimensions, and a filler-value
	Matrix(const size_t rows, const size_t columns, const T filler_value) {
		for (size_t i = 0; i < rows; i++)
			_InnerVector.push_back(std::vector<T>(columns, filler_value));
	}

	//Constructs an identity matrix with a given dimension
	Matrix(const size_t dimension) {
		static_assert(std::is_arithmetic_v<T>, "constructIdentityMatrix(const size_t) is not available for the specified type.");

		for (size_t i = 0; i < dimension; i++) {
			_InnerVector.push_back(std::vector<T>(dimension, 0));

			for (size_t j = 0; j < dimension; j++)
				if (i == j) _InnerVector[i][j]++;
		}
	}

	//Constructs a diagonal matrix where each element is a sub-matrix from the provided matrices
	Matrix(const std::vector<Matrix<T>> & inner_matrices) {
		static_assert(std::is_arithmetic_v<T>, "constructBlockDiagonalMatrix(const std::vector<Matrix<T>>&) is not available for the specified type.");

		if (inner_matrices.empty()) return;

		size_t _curr_mat_pos = 0;

		do {
			if (inner_matrices[_curr_mat_pos].isEmpty()) _curr_mat_pos++;
			else break;
		}
		while (_curr_mat_pos < inner_matrices.size());
		
		if (_curr_mat_pos == inner_matrices.size()) return;

		std::pair<size_t, size_t> rows_cols = _getTotalRows_Columns(inner_matrices);
		size_t _columns_filled_yet = 0;

		for (_curr_mat_pos; _curr_mat_pos < inner_matrices.size(); _curr_mat_pos++) {
			for (size_t i = 0; i < inner_matrices[_curr_mat_pos]._InnerVector.size(); i++) {
				_InnerVector.push_back(std::vector<T>(rows_cols.second, 0));

				for (size_t j = 0; j < inner_matrices[_curr_mat_pos]._InnerVector.front().size(); j++)
					_InnerVector.back()[j + _columns_filled_yet] = inner_matrices[_curr_mat_pos]._InnerVector[i][j];
			}

			_columns_filled_yet += inner_matrices[_curr_mat_pos]._InnerVector.front().size();
		}
	}

	//Returns a matrix from a vector of rows
	//If the sizes of the vectors are not valid, and empty matrix will be constructed
	static Matrix<T> constructMatrixFromRows(const std::vector<std::vector<T>> & rows) {
		return Matrix<T>(rows);
	}
	
	//Returns a matrix from a vector of columns
	//If the sizes of the vectors are not valid, and empty matrix will be constructed
	static Matrix<T> constructMatrixFromColumns(const std::vector<std::vector<T>> & columns) {
		return constructMatrixFromRows(columns).getTranspose().mat;
	}

	//Returns a diagonal matrix with specified diagonal-values
	static Matrix<T> constructDiagonalMatrix(const std::vector<T> & diagonal_values) {
		return Matrix<T>(diagonal_values);
	}

	//Returns a diagonal matrix where each element is a sub-matrix from the provided matrices
	static Matrix<T> constructBlockDiagonalMatrix(const std::vector<Matrix<T>> & inner_matrices) {
		return Matrix<T>(inner_matrices);
	}

	//Returns a square matrix with a specified filler-value
	static Matrix<T> constructSquareMatrix(const size_t dimension, const T filler_value) {
		return Matrix<T>(dimension, filler_value);
	}

	//Returns a matrix with specified dimensions, and a filler value
	static Matrix<T> constructMatrix(const size_t rows, const size_t columns, const T filler_value) {
		return Matrix<T>(rows, columns, filler_value);
	}

	//Returns an identity matrix of a given dimension
	static Matrix<T> constructIdentityMatrix(const size_t dimension) {
		return Matrix<T>(dimension);
	}

	//Returns an identity matrix with the same dimensions of another matrix
	static Matrix<T> constructIdentityMatrix(const Matrix & square_matrix) {
		static_assert(std::is_arithmetic_v<T>, "constructIdentityMatrix(const Matrix&) is not available for the specified type.");

		if (square_matrix.isSquareMatrix())
			return Matrix<T>(square_matrix._InnerVector.size());

		return {};
	}

	//Returns a scalar matrix with the specified scalar
	static Matrix<T> constructScalarMatrix(const size_t dimension, const T scalar) {
		static_assert(std::is_arithmetic_v<T>, "constructIdentityMatrix(const size_t) is not available for the specified type.");

		Matrix<T> _mat;

		for (size_t i = 0; i < dimension; i++) {
			_mat._InnerVector.push_back(std::vector<T>(dimension, 0));

			for (size_t j = 0; j < dimension; j++)
				if (i == j) _mat._InnerVector[i][j] += scalar;
		}

		return _mat;
	}

	static Matrix<T> constructRowMatrix(const std::vector<T> & row) {
		Matrix<T> _mat;
		_mat._InnerVector.push_back(row);
		return _mat;
	}

	static Matrix<T> constructColumnMatrix(const std::vector<T> & column) {
		Matrix<T> _mat;
		for (const T & value : column)
			_mat._InnerVector.push_back({ value });
		return _mat;
	}

	//This will split the matrix into two matrices, where the specified row will not appear in the first matrix.
	Result_Split splitHorizontally(const size_t row_index) const;

	//This will split the matrix into two matrices, where the specified column will not appear in the first matrix.
	Result_Split splitVertically(const size_t column_index) const;

	//Splits an even-dimension matrix into 4 sub-matrices
	Result_Split_Quadrants splitIntoQuadrants() const;

	bool isEmpty() const {
		return _isMatEmpty(*this);
	}

	bool isEqualTo(const Matrix<T> & other_matrix) const {
		if (_InnerVector.size() != other_matrix._InnerVector.size() || (_InnerVector.empty() ? false : _InnerVector.front().size() != other_matrix._InnerVector.front().size()))
			return false;

		for (size_t i = 0; i < _InnerVector.size(); i++)
			for (size_t j = 0; j < _InnerVector.size(); j++)
				if (_InnerVector[i][j] != other_matrix._InnerVector[i][j])
					return false;

		return true;
	}

	bool isRowMatrix() const {
		return _InnerVector.size() == 1;
	}

	bool isColumnMatrix() const {
		return !isEmpty() && _InnerVector.front().size() == 1;
	}

	bool isSquareMatrix() const {
		return !isEmpty() && _InnerVector.size() == _InnerVector.front().size();
	}

	bool isZeroMatrix() const {
		static_assert(std::is_arithmetic_v<T>, "isZeroMatrix() is not available for the specified type.");

		if (isEmpty())
			return false;

		for (const auto & row : _InnerVector)
			for (const T & element : row)
				if (element != 0)
					return false;

		return true;
	}

	bool isDiagonalMatrix() const {
		static_assert(std::is_arithmetic_v<T>, "isDiagonalMatrix() is not available for the specified type.");

		if (!isSquareMatrix()) //Being empty is checked already
			return false;

		for (size_t i = 0; i < _InnerVector.size(); i++)
			for (size_t j = 0; j < _InnerVector.size(); j++)
				if (i != j && _InnerVector[i][j] != 0)
					return false;

		return true;
	}

	bool isScalarMatrix() const {
		static_assert(std::is_arithmetic_v<T>, "isScalarMatrix() is not available for the specified type.");

		return _checkIdentityMultiple(_InnerVector.front().front()); //Being empty is checked already
	}

	bool isIdentityMatrix() const {
		static_assert(std::is_arithmetic_v<T>, "isIdentityMatrix() is not available for the specified type.");

		return _checkIdentityMultiple(1); //Being empty is checked already
	}

	bool isUpperTriangularMatrix() const {
		static_assert(std::is_arithmetic_v<T>, "isUpperTriangularMatrix() is not available for the specified type.");

		if (!isSquareMatrix()) //Being empty is checked already
			return false;

		for (size_t i = 1; i < _InnerVector.size(); i++)
			for (size_t j = 0; j < i; j++)
				if (_InnerVector[i][j] != 0)
					return false;

		return true;
	}

	bool isLowerTriangularMatrix() const {
		static_assert(std::is_arithmetic_v<T>, "isLowerTriangularMatrix() is not available for the specified type.");

		if (!isSquareMatrix()) //Being empty is checked already
			return false;

		for (size_t i = 0; i < _InnerVector.size() - 1; i++)
			for (size_t j = _InnerVector.front().size() - 1; j > i; j--)
				if (_InnerVector[i][j] != 0)
					return false;

		return true;
	}

	bool isTriangularMatrix() const {
		static_assert(std::is_arithmetic_v<T>, "isTriangular() is not available for the specified type.");

		return isUpperTriangularMatrix() || isLowerTriangularMatrix(); //Being empty is checked already
	}

	bool isSymmetricMatrix() const;

	bool isSkewSymmetricMatrix() const;

	bool isSingular() const;

	bool isInvertible() const;

	//Overrides the current matrix with a new one
	//If the new mat is not valid, and empty matrix will be stored
	void assign(const Matrix<T> & new_matrix) {
		if (_checkMatrixDimensions(new_matrix))
			_InnerVector = new_matrix._InnerVector;
		else
			_InnerVector = {};
	}

	Result_Vct getRow(size_t index) const {
		if (index >= _InnerVector.size())
			return { {}, ErrorCodes::INVALID_INDICES };

		return { _InnerVector[index], ErrorCodes::SUCCESS };
	}

	//Throws an std::out_of_range exception when passing an invalid index
	Result_Vct getColumn(size_t index) const {
		if (index >= _InnerVector.front().size())
			return { {}, ErrorCodes::INVALID_INDICES };

		std::vector<T> _result;

		for (const auto & row : _InnerVector)
			_result.push_back(row[index]);

		return { _result, ErrorCodes::SUCCESS };
	}

	Result_Val getElement(const size_t row, const size_t column) const;

	size_t getSize() const {
		if (isEmpty())
			return 0;

		return _InnerVector.size() * _InnerVector.front().size();
	}

	size_t getNumberOfRows() const {
		return _InnerVector.size();
	}

	size_t getNumberOfColumns() const {
		return _InnerVector.front().size();
	}

	Dimensions getDimensions() const {
		return { _InnerVector.size(), _InnerVector.front().size() };
	}

	Result_Mat appendRow(const std::vector<T> & row) const;

	Result_Mat appendColumn(const std::vector<T> & column) const;

	Result_Mat prependRow(const std::vector<T> & row) const;

	Result_Mat prependColumn(const std::vector<T> & column) const;

	Result_Mat insertRow(const size_t position, const std::vector<T> & row) const;

	Result_Mat insertColumn(const size_t position, const std::vector<T> & column) const;

	Result_Mat swapRows(const size_t first_row_index, const size_t second_row_index) const;

	Result_Mat swapColumns(const size_t first_column_index, const size_t second_column_index) const;

	Result_Mat permuteRows(const std::vector<size_t> & order) const;

	Result_Mat permuteColumns(const std::vector<size_t> & order) const;

	void swap(Matrix<T> & other_matrix) {
		std::swap(*this, other_matrix);
	}

	Result_Mat appendMatrixHorizontally(const Matrix<T> & other_matrix) const;

	Result_Mat appendMatrixVertically(const Matrix<T> & other_matrix) const;

	Result_Mat prependMatrixHorizontally(const Matrix<T> & other_matrix) const;

	Result_Mat prependMatrixVertically(const Matrix<T> & other_matrix) const;

	//The new matrix will be inserted before the specifidied column
	Result_Mat insertMatrixHorizontally(const size_t column_position, const Matrix<T> & other_matrix) const;

	//The new matrix will be inserted before the specified row
	Result_Mat insertMatrixVertically(const size_t row_position, const Matrix<T> & other_matrix) const;

	//Empty matrices will be ignored
	static Result_Mat concatenateMatricesHorizontally(const std::vector<Matrix<T>> & _matrices);

	//Empty matrices will be ignored
	static Result_Mat concatenateMatricesVertically(const std::vector<Matrix<T>> & _matrices);

	Result_Mat eraseRow(const size_t row_index) const;

	Result_Mat eraseColumn(const size_t column_index) const;

	Result_Mat eraseSubMatrix_Indices(const size_t start_row_index, const size_t end_row_index, const size_t start_column_index, const size_t end_column_index) const;

	Result_Mat eraseSubMatrix_Dimensions(const size_t row_index, const size_t column_index, const size_t width, const size_t height) const;

	Result_Mat reverseVertically() const;

	Result_Mat reverseHorizontally() const;

	Result_Mat reverse() const;

	Result_Mat reverseRow(const size_t row_index) const;

	Result_Mat reverseColumn(const size_t column_index) const;

	//Shifts the elements in the specified row a certain number of steps to the left
	Result_Mat shiftRow_Left(const size_t row_index, const size_t steps) const;

	//Shifts the elements in the specified row a certain number of steps to the right
	Result_Mat shiftRow_Right(const size_t row_index, const size_t steps) const;

	//Shifts the elements in the specified column a certain number of steps upwards
	Result_Mat shiftColumn_Up(const size_t column_index, const size_t steps) const;

	//Shifts the elements in the specified column a certain number of steps downwards
	Result_Mat shiftColumn_Down(const size_t column_index, const size_t steps) const;

	//Shifts the whole matrix horizontally to the left
	Result_Mat shift_Left(const size_t steps) const;

	//Shifts the whole matrix horizontally to the right
	Result_Mat shift_Right(const size_t steps) const;

	//Shifts the whole matrix vertically upwards
	Result_Mat shift_Up(const size_t steps) const;

	//Shifts the whole matrix vertically downwards
	Result_Mat shift_Down(const size_t steps) const;

	//A Bubble Sort algorithm
	Result_Mat sortRow(const size_t row_index, const bool reverse = false) const;

	//A Bubble Sort algorithm
	Result_Mat sortColumn(const size_t column_index, const bool reverse = false) const;

	//A Bubble Sort algorithm
	Result_Mat sortHorizontally(const bool reverse = false) const;

	//A Bubble Sort algorithm
	Result_Mat sortVertically(const bool reverse = false) const;

	Position find(const T value) {
		for (size_t i = 0; i < _InnerVector.size(); i++)
			for (size_t j = 0; j < _InnerVector.front().size(); j++)
				if (_InnerVector[i][j] == value)
					return { i, j };

		return { size_t(-1), size_t(-1) };
	}

	Position rfind(const T value) {
		for (size_t i = _InnerVector.size(); i > 0; i--)
			for (size_t j = _InnerVector.front().size(); j > 0; j--)
				if (_InnerVector[i - 1][j - 1] == value)
					return { i - 1, j - 1 };

		return { size_t(-1), size_t(-1) };
	}

	std::string toString(const short precision = 6, const std::string & separator = ", ") const {
		if (isEmpty())
			return "[]";
		
		std::ostringstream _oss;
		_oss << "[" << _vctToStr(_InnerVector.front(), precision, separator);
		
		for (size_t i = 1; i < _InnerVector.size(); i++)
			_oss << separator << _vctToStr(_InnerVector[i], precision, separator);
		
		_oss << ']';
		return _oss.str();
	}

	//Extracts a sub-matrix where all specified rows & columns appear
	Result_Mat extractSubMatrix_Indices(const size_t start_row_index, const size_t end_row_index, const size_t start_column_index, const size_t end_column_index) const;

	//The element at row_index & column_index will appear in the result
	Result_Mat extractSubMatrix_Dimensions(const size_t row_index, const size_t column_index, const size_t width, const size_t height) const;

	//This will exclude the row & column provided and return the rest of the matrix
	Result_Mat extractSubMatrix_Excluding(const size_t row, const size_t column) const;

	std::vector<T> flattenHorizontally() const {
		std::vector<T> _result;

		for (const auto & row : _InnerVector)
			for (const T & value : row)
				_result.push_back(value);

		return _result;
	}

	std::vector<T> flattenVertically() const {
		std::vector<T> _result;

		for (size_t j = 0; j < _InnerVector.front().size(); j++)
			for (size_t i = 0; i < _InnerVector.size(); i++)
				_result.push_back(_InnerVector[i][j]);

		return _result;
	}

	Result_Mat reshape(const size_t new_rows, const size_t new_columns) const;

	Result_Mat replaceRow(const size_t row_index, const std::vector<T> & new_row) const;

	Result_Mat replaceColumn(const size_t column_index, const std::vector<T> & new_column) const;

	//It will replace from the element specified going down-right
	Result_Mat replaceSubMatrix(const size_t row_index, const size_t column_index, const Matrix<T> & new_matrix) const;

	Result_Mat getTranspose() const;

	Result_Mat plus(const Matrix<T> & other_matrix) const;

	Result_Mat minus(const Matrix<T> & other_matrix) const;

	Result_Mat multiplyBy(const T & value) const;

	Result_Mat multiplyBy(const Matrix<T> & other_matrix) const;

	Result_Mat divideBy(const T & value) const;

	Result_Mat divideBy(const Matrix<T> & other_matrix) const;

	Result_Mat raiseToPowerOf(const unsigned short power) const;

	Result_Val getDeterminant() const;

	Result_Val getTrace() const;

	Result_Mat getInverse() const;

	std::vector<T> & operator[] (const size_t index) {
		return _InnerVector.at(index); //To provide exceptions
	}

	Matrix<T> operator+(const Matrix<T> &) const;

	Matrix<T> & operator+=(const Matrix<T> &);

	Matrix<T> operator-(const Matrix<T> &) const;

	Matrix<T> operator-() const;

	Matrix<T> & operator-=(const Matrix<T> &);

	Matrix<T> operator*(const T) const;

	Matrix<T> & operator*=(const T);

	Matrix<T> operator*(const Matrix<T> &) const;

	Matrix<T> & operator*=(const Matrix<T> &);

	Matrix<T> operator/(const T) const;

	Matrix<T> & operator/=(const T);

	Matrix<T> operator/(const Matrix<T> &) const;

	Matrix<T> & operator/=(const Matrix<T> &);

	Matrix<T> operator^(const unsigned short) const;

	Matrix<T> & operator^=(const unsigned short);

	Matrix<T> operator~() const;

	Matrix<T> operator!() const;

	bool operator==(const Matrix<T> &) const;

	bool operator!=(const Matrix<T> &) const;

	template <typename T>
	friend std::ostream & operator<<(std::ostream &, const Matrix<T> &);
};

template <typename T>
struct Matrix<T>::Result_Mat {
	Matrix<T> mat;
	ErrorCodes error_code;
};

template <typename T>
struct Matrix<T>::Result_Val {
	T val;
	Matrix<T>::ErrorCodes error_code;
};

template <typename T>
struct Matrix<T>::Result_Split {
	Matrix<T> mat1, mat2;
	Matrix<T>::ErrorCodes error_code;
};

template <typename T>
struct Matrix<T>::Result_Split_Quadrants {
	Matrix<T> mat1, mat2, mat3, mat4;
	Matrix<T>::ErrorCodes error_code;
};

template <typename T>
bool Matrix<T>::isSymmetricMatrix() const {
	if (isEmpty())
		return false;

	return isEqualTo(getTranspose().mat);
}

template <typename T>
bool Matrix<T>::isSkewSymmetricMatrix() const {
	static_assert(std::is_arithmetic_v<T>, "isSkewSymmetricMatrix() is not available for the specified type.");

	if (isEmpty())
		return false;

	return isEqualTo(getTranspose().mat.multiplyBy(-1).mat);
}

template <typename T>
bool Matrix<T>::isSingular() const {
	static_assert(std::is_arithmetic_v<T>, "isSingular() is not available for the specified type.");

	if (isEmpty())
		return true; //To be consistent with the class itself :-)

	return getDeterminant().val == 0;
}

template <typename T>
bool Matrix<T>::isInvertible() const {
	static_assert(std::is_arithmetic_v<T>, "isInvertible() is not available for the specified type.");

	return !isSingular();
}

template <typename T>
typename Matrix<T>::Result_Val Matrix<T>::getElement(const size_t row, const size_t column) const {
	if (_isMatEmpty(*this))
		return { 0, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (row >= _InnerVector.size() || column >= _InnerVector.front().size())
		return { 0, ErrorCodes::INVALID_INDICES };

	return { _InnerVector[row][column], ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::insertRow(const size_t position, const std::vector<T> & row) const {
	if (position > _InnerVector.size())
		return { {}, ErrorCodes::INVALID_POSITION };
	
	if (isEmpty()) //The position is zero automatically
		return { Matrix({ row }), ErrorCodes::SUCCESS };

	if (row.size() != _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	Matrix<T> _mat = *this;
	_mat._InnerVector.insert(_mat._InnerVector.begin() + position, row);
	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::insertColumn(const size_t position, const std::vector<T> & column) const {
	if (isEmpty()) {
		if (position == 0)
			return appendColumn(column);

		return { {}, ErrorCodes::INVALID_POSITION };
	}
	
	if (position > _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_POSITION };
	
	if (column.size() != _InnerVector.size())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	Matrix<T> _mat = *this;

	for (size_t i = 0; i < column.size(); i++)
		_mat._InnerVector[i].insert(_mat._InnerVector[i].begin() + position, column[i]);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::swapRows(const size_t first_row_index, const size_t second_row_index) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (first_row_index >= _InnerVector.size() || second_row_index >= _InnerVector.size())
		return { {}, ErrorCodes::INVALID_INDICES };

	Matrix<T> _mat = *this;
	_mat._InnerVector[first_row_index].swap(_mat._InnerVector[second_row_index]);
	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::swapColumns(const size_t first_column_index, const size_t second_column_index) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (first_column_index >= _InnerVector.front().size() || second_column_index >= _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_INDICES };

	Matrix<T> _mat = *this;

	for (auto & row : _mat._InnerVector)
		std::swap(row[first_column_index], row[second_column_index]);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::appendRow(const std::vector<T> & row) const {
	return insertRow(_InnerVector.size(), row);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::appendColumn(const std::vector<T> & column) const {
	return insertColumn(_InnerVector.front().size(), column);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::prependRow(const std::vector<T> & row) const {
	return insertRow(0, row);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::prependColumn(const std::vector<T> & column) const {
	return insertColumn(0, column);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::eraseRow(const size_t position) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (position >= _InnerVector.size())
		return { {}, ErrorCodes::INVALID_POSITION };

	Matrix<T> _mat = *this;
	_mat._InnerVector.erase(_mat._InnerVector.begin() + position);
	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::eraseColumn(const size_t position) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (position >= _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_POSITION };

	Matrix<T> _mat = *this;
	for (auto & row : _mat._InnerVector)
		row.erase(row.begin() + position);
	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::eraseSubMatrix_Indices(const size_t start_row_index, const size_t end_row_index, const size_t start_column_index, const size_t end_column_index) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (start_row_index >= _InnerVector.size() || end_row_index >= _InnerVector.size() || start_column_index >= _InnerVector.front().size() || end_column_index >= _InnerVector.front().size() || start_row_index > end_row_index || start_column_index > end_column_index)
		return { {}, ErrorCodes::INVALID_INDICES };

	const size_t _width_to_erase = end_column_index - start_column_index + 1;
	const size_t _height_to_erase = end_row_index - start_row_index + 1;

	if (_height_to_erase != _InnerVector.size() && _width_to_erase != _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	Matrix _mat;

	if (_width_to_erase == _InnerVector.front().size()) { //Erasing rows
		for (size_t i = 0; i < start_row_index; i++)
			_mat._InnerVector.push_back(_InnerVector[i]);

		for (size_t i = end_row_index + 1; i < _InnerVector.size(); i++)
			_mat._InnerVector.push_back(_InnerVector[i]);
	}
	else if (_height_to_erase == _InnerVector.size()) { //Erasing columns
		std::vector<T> _elements_to_keep;

		for (size_t i = 0; i < _height_to_erase; i++) {
			for (size_t j = 0; j < start_column_index; j++)
				_elements_to_keep.push_back(_InnerVector[i][j]);

			for (size_t j = end_column_index + 1; j < _InnerVector.front().size(); j++)
				_elements_to_keep.push_back(_InnerVector[i][j]);

			_mat._InnerVector.push_back(_elements_to_keep); //No need to check if the _elements_to_keep is empty here, because it will NEVER be!!
			_elements_to_keep.clear();
		}
	}

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::eraseSubMatrix_Dimensions(const size_t row_index, const size_t column_index, const size_t width, const size_t height) const {
	Result_Mat _result = eraseSubMatrix_Indices(row_index, row_index + height - 1, column_index, column_index + width - 1);

	if (_result.error_code == ErrorCodes::INVALID_EMPTY_MATRIX || _result.error_code == ErrorCodes::SUCCESS)
		return _result;

	return { {}, ErrorCodes::INVALID_DIMENSIONS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::insertMatrixHorizontally(const size_t column_position, const Matrix<T> & other_matrix) const {
	if (isEmpty()) {
		if (column_position == 0)
			return { other_matrix, ErrorCodes::SUCCESS };

		return { {}, ErrorCodes::INVALID_POSITION };
	}

	if (column_position > _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_POSITION };

	if (_InnerVector.size() != other_matrix._InnerVector.size())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	Matrix<T> _mat = *this;

	for (size_t i = 0; i < _InnerVector.size(); i++)
		_mat._InnerVector[i].insert(_mat._InnerVector[i].begin() + column_position, other_matrix._InnerVector[i].begin(), other_matrix._InnerVector[i].end());

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::insertMatrixVertically(const size_t row_position, const Matrix<T> & other_matrix) const {
	if (row_position > _InnerVector.size())
		return { {}, ErrorCodes::INVALID_POSITION };
	
	if (isEmpty()) //The position is zero automatically
		return { other_matrix, ErrorCodes::SUCCESS };

	if (_InnerVector.front().size() != other_matrix._InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	Matrix<T> _mat = *this;

	_mat._InnerVector.insert(_mat._InnerVector.begin() + row_position, other_matrix._InnerVector.begin(), other_matrix._InnerVector.end());

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::appendMatrixHorizontally(const Matrix<T> & other_matrix) const {
	return insertMatrixHorizontally(_InnerVector.front().size(), other_matrix);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::appendMatrixVertically(const Matrix<T> & other_matrix) const {
	return insertMatrixVertically(_InnerVector.size(), other_matrix);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::prependMatrixHorizontally(const Matrix<T> & other_matrix) const {
	return insertMatrixHorizontally(0, other_matrix);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::prependMatrixVertically(const Matrix<T> & other_matrix) const {
	return insertMatrixVertically(0, other_matrix);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::_concatenateMatrices(const std::vector<Matrix<T>> & _matrices, const bool & _horizontally_vertically) {
	size_t i = _getFirstNonEmptyMatrixPosition(_matrices); //This will also return size_t(-1) if the vector is empty

	if (i == size_t(-1))
		return { {}, ErrorCodes::SUCCESS };

	Result_Mat _append_result = { _matrices[i++] , ErrorCodes::SUCCESS };

	for (i; i < _matrices.size(); i++) {
		if (_matrices[i].isEmpty())
			continue;

		_append_result = _horizontally_vertically ? _append_result.mat.appendMatrixVertically(_matrices[i]) : _append_result.mat.appendMatrixHorizontally(_matrices[i]);

		if (_append_result.error_code != ErrorCodes::SUCCESS)
			return { {}, ErrorCodes::INVALID_DIMENSIONS };
	}

	return _append_result;
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::concatenateMatricesHorizontally(const std::vector<Matrix<T>> & _matrices) {
	return _concatenateMatrices(_matrices, 0);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::concatenateMatricesVertically(const std::vector<Matrix<T>> & _matrices) {
	return _concatenateMatrices(_matrices, 1);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::reverseVertically() const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	Matrix<T> _mat = *this;

	for (auto & row : _mat._InnerVector)
		for (size_t i = 0; i < row.size() / 2; i++)
			std::swap(row[i], row[row.size() - i - 1]);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::reverseHorizontally() const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	Matrix<T> _mat = *this;

	for (size_t i = 0; i < _mat._InnerVector.size() / 2; i++)
		_mat._InnerVector[i].swap(_mat._InnerVector[_mat._InnerVector.size() - i - 1]);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::reverse() const {
	return reverseHorizontally().mat.reverseVertically();
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::reverseRow(const size_t row_index) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (row_index >= _InnerVector.size())
		return { {}, ErrorCodes::INVALID_POSITION };

	Matrix<T> _mat = *this;

	for (size_t i = 0; i < _mat._InnerVector[row_index].size() / 2; i++)
		std::swap(_mat._InnerVector[row_index][i], _mat._InnerVector[row_index][_mat._InnerVector[row_index].size() - i - 1]);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::reverseColumn(const size_t column_index) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (column_index >= _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_POSITION };

	Matrix<T> _mat = *this;

	for (size_t i = 0; i < _mat._InnerVector.size() / 2; i++)
		std::swap(_mat._InnerVector[i][column_index], _mat._InnerVector[_mat._InnerVector.size() - i - 1][column_index]);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::_shiftRow(const size_t _row_index, const size_t _steps, const bool _direction) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (_row_index >= _InnerVector.size())
		return { {}, ErrorCodes::INVALID_POSITION };

	Matrix<T> _mat = *this;

	_rotateElements_Vct(_mat._InnerVector[_row_index], _steps, _direction);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::_shiftColumn(const size_t _column_index, const size_t _steps, const bool _direction) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (_column_index >= _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_POSITION };

	std::vector<T> _column = getColumn(_column_index);

	_rotateElements_Vct(_column, _steps, _direction);

	return replaceColumn(_column_index, _column);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::shiftRow_Left(const size_t row_index, const size_t steps) const {
	return _shiftRow(row_index, steps, 1);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::shiftRow_Right(const size_t row_index, const size_t steps) const {
	return _shiftRow(row_index, steps, 0);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::shiftColumn_Up(const size_t column_index, const size_t steps) const {
	return _shiftColumn(column_index, steps, 1);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::shiftColumn_Down(const size_t column_index, const size_t steps) const {
	return _shiftColumn(column_index, steps, 0);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::_shiftHorizontally(const size_t _steps, const bool _direction) const {
	Matrix<T> _mat = *this;

	for (auto & row : _mat._InnerVector)
		_rotateElements_Vct(row, _steps, _direction);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::shift_Left(const size_t steps) const {
	return _shiftHorizontally(steps, 1);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::shift_Right(const size_t steps) const {
	return _shiftHorizontally(steps, 0);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::_shiftVertically(const size_t _steps, const bool _direction) const {
	Result_Mat _result = { *this, ErrorCodes::SUCCESS };

	for (size_t i = 0; i < _result.mat._InnerVector.front().size(); i++) {
		_result = _result.mat._shiftColumn(i, _steps, _direction);

		if (_result.error_code == ErrorCodes::INVALID_EMPTY_MATRIX)
			return _result;
	}

	return _result;
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::shift_Up(const size_t steps) const {
	return _shiftVertically(steps, 1);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::shift_Down(const size_t steps) const {
	return _shiftVertically(steps, 0);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::sortRow(const size_t row_index, const bool reverse) const {
	static_assert(std::is_arithmetic_v<T>, "sortRow(const size_t, const bool) is not available for the specified type.");

	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (row_index >= _InnerVector.size())
		return { {}, ErrorCodes::INVALID_POSITION };

	Matrix<T> _mat = *this;

	_sortVct(_mat._InnerVector[row_index], reverse);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::sortColumn(const size_t column_index, const bool reverse) const {
	static_assert(std::is_arithmetic_v<T>, "sortColumn(const size_t, const bool) is not available for the specified type.");

	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (column_index >= _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_POSITION };

	std::vector<T> _column = getColumn(column_index).vct;

	_sortVct(_column, reverse);

	return replaceColumn(column_index, _column);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::sortHorizontally(const bool reverse) const {
	static_assert(std::is_arithmetic_v<T>, "sortHorizontally(const bool) is not available for the specified type.");

	Matrix<T> _mat = *this;

	for (auto & row : _mat._InnerVector)
		_sortVct(row, reverse);

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::sortVertically(const bool reverse) const {
	static_assert(std::is_arithmetic_v<T>, "sortVertically(const bool) is not available for the specified type.");

	Result_Mat _result = { *this, ErrorCodes::SUCCESS };

	for (size_t i = 0; i < _result.mat._InnerVector.front().size(); i++) {
		_result = _result.mat.sortColumn(i, reverse);

		if (_result.error_code == ErrorCodes::INVALID_EMPTY_MATRIX)
			return _result;
	}

	return _result;
}

template <typename T>
typename Matrix<T>::Result_Split Matrix<T>::splitHorizontally(const size_t row_index) const {
	if (isEmpty())
		return { {}, {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (row_index > _InnerVector.size())
		return { {}, {}, ErrorCodes::INVALID_POSITION };

	Matrix<T> _mat1, _mat2;

	for (size_t i = 0; i < row_index; i++)
		_mat1._InnerVector.push_back(_InnerVector[i]);

	for (size_t i = row_index; i < _InnerVector.size(); i++)
		_mat2._InnerVector.push_back(_InnerVector[i]);

	return { _mat1, _mat2, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Split Matrix<T>::splitVertically(const size_t column_index) const {
	if (isEmpty())
		return { {}, {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (column_index > _InnerVector.front().size())
		return { {}, {}, ErrorCodes::INVALID_POSITION };

	Matrix<T> _mat1, _mat2;

	for (size_t i = 0; i < _InnerVector.size(); i++) {
		_mat1._InnerVector.push_back(std::vector<T>(_InnerVector[i].begin(), _InnerVector[i].begin() + column_index));
		_mat2._InnerVector.push_back(std::vector<T>(_InnerVector[i].begin() + column_index, _InnerVector[i].end()));
	}

	return { _mat1, _mat2, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Split_Quadrants Matrix<T>::splitIntoQuadrants() const {
	if (isEmpty())
		return { {}, {}, {}, {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (_InnerVector.size() % 2 || _InnerVector.front().size() % 2)
		return { {}, {}, {}, {}, ErrorCodes::INVALID_DIMENSIONS };

	return {
		extractSubMatrix_Indices(0, _InnerVector.size() / 2 - 1, 0, _InnerVector.front().size() / 2 - 1).mat,
		extractSubMatrix_Indices(0, _InnerVector.size() / 2 - 1, _InnerVector.front().size() / 2, _InnerVector.front().size() - 1).mat,
		extractSubMatrix_Indices(_InnerVector.size() / 2, _InnerVector.size() - 1, 0, _InnerVector.front().size() / 2 - 1).mat,
		extractSubMatrix_Indices(_InnerVector.size() / 2, _InnerVector.size() - 1, _InnerVector.front().size() / 2, _InnerVector.front().size() - 1).mat,
		ErrorCodes::SUCCESS
	};
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::extractSubMatrix_Indices(const size_t start_row_index, const size_t end_row_index, const size_t start_column_index, const size_t end_column_index) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (start_row_index >= _InnerVector.size() || end_row_index >= _InnerVector.size() || start_column_index >= _InnerVector.front().size() || end_column_index >= _InnerVector.front().size() || start_row_index > end_row_index || start_column_index > end_column_index)
		return { {}, ErrorCodes::INVALID_INDICES };

	Matrix<T> _mat;

	for (size_t i = start_row_index; i <= end_row_index; i++)
		_mat._InnerVector.push_back(std::vector<T>(_InnerVector[i].begin() + start_column_index, _InnerVector[i].begin() + end_column_index + 1));
	
	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::extractSubMatrix_Dimensions(const size_t row_index, const size_t column_index, const size_t width, const size_t height) const {
	Result_Mat _result = extractSubMatrix_Indices(row_index, row_index + height - 1, column_index, column_index + width - 1);

	if (_result.error_code == ErrorCodes::INVALID_EMPTY_MATRIX || _result.error_code == ErrorCodes::SUCCESS)
		return _result;

	return { {}, ErrorCodes::INVALID_DIMENSIONS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::extractSubMatrix_Excluding(const size_t row, const size_t column) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (row >= _InnerVector.size() || column >= _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_INDICES };

	return { _getSubMatrix(*this, row, column), ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::replaceRow(const size_t row_index, const std::vector<T> & new_row) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (row_index >= _InnerVector.size())
		return { {}, ErrorCodes::INVALID_POSITION };

	if (new_row.size() != _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };
	
	Matrix _mat = *this;
	_mat._InnerVector[row_index] = new_row;
	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::replaceColumn(const size_t column_index, const std::vector<T> & new_column) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (column_index >= _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_POSITION };

	if (new_column.size() != _InnerVector.size())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	Matrix _mat = *this;

	for (size_t i = 0; i < _mat._InnerVector.size(); i++)
		_mat._InnerVector[i][column_index] = new_column[i];

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::replaceSubMatrix(const size_t row_index, const size_t column_index, const Matrix<T> & new_matrix) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (row_index >= _InnerVector.size() || column_index >= _InnerVector.front().size())
		return { {}, ErrorCodes::INVALID_INDICES };

	if (new_matrix._InnerVector.size() > _InnerVector.size() - row_index || new_matrix._InnerVector.front().size() > _InnerVector.front().size() - column_index)
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	Matrix _mat = *this;

	for (size_t i = 0; i < new_matrix._InnerVector.size(); i++)
		for (size_t j = 0; j < new_matrix._InnerVector.front().size(); j++)
			_mat._InnerVector[i + row_index][j + column_index] = new_matrix._InnerVector[i][j];

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::reshape(const size_t new_rows, const size_t new_columns) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (new_columns * new_rows != getSize())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	std::vector<T> _elements = flattenHorizontally();
	Matrix<T> _mat;

	for (size_t i = 0; i < new_rows; i++)
		_mat._InnerVector.push_back(std::vector<T>(_elements.begin() + i * new_columns, _elements.begin() + new_columns * (i + 1)));

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::permuteRows(const std::vector<size_t> & order) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (order.size() != _InnerVector.size() || _hasDuplicates(order))
		return { {}, ErrorCodes::INVALID_INDICES };

	Matrix<T> _mat;

	for (const size_t index : order) {
		if (index >= _InnerVector.size())
			return { {}, ErrorCodes::INVALID_INDICES };

		_mat._InnerVector.push_back(_InnerVector[index]);
	}

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::permuteColumns(const std::vector<size_t> & order) const {
	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };
	
	if (order.size() != _InnerVector.front().size() || _hasDuplicates(order))
		return { {}, ErrorCodes::INVALID_INDICES };

	Matrix<T> _mat;

	for (size_t i = 0; i < _InnerVector.size(); i++) {
		_mat._InnerVector.push_back({});
		_mat._InnerVector.back().reserve(order.size());

		for (const size_t index : order) {
			if (index >= _InnerVector.front().size())
				return { {}, ErrorCodes::INVALID_INDICES };

			_mat._InnerVector.back().push_back(_InnerVector[i][index]);
		}
	}

	return { _mat, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::getTranspose() const {
	if (_isMatEmpty(*this))
		return { {}, Matrix<T>::ErrorCodes::INVALID_EMPTY_MATRIX };

	Result_Mat _result = { Matrix(_InnerVector.front().size(), _InnerVector.size(), 0), ErrorCodes::SUCCESS };

	for (size_t i = 0; i < _InnerVector.size(); i++)
		for (size_t j = 0; j < _InnerVector.front().size(); j++)
			_result.mat._InnerVector[j][i] = _InnerVector[i][j];

	return _result;
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::_additionBase(const Matrix<T> & _other_matrix, const bool _add) const {
	if (!_checkMatricesDimensions_Same(*this, _other_matrix))
		return { {}, Matrix<T>::ErrorCodes::INVALID_DIMENSIONS };

	if (_isMatEmpty(*this) || _isMatEmpty(_other_matrix))
		return { {}, Matrix<T>::ErrorCodes::INVALID_EMPTY_MATRIX };

	Matrix<T>::Result_Mat _result = { Matrix(_InnerVector.size(), _InnerVector.front().size(), 0), Matrix<T>::ErrorCodes::SUCCESS };

	for (size_t i = 0; i < _result.mat._InnerVector.size(); i++)
		for (size_t j = 0; j < _result.mat._InnerVector.front().size(); j++)
			_result.mat._InnerVector[i][j] = _InnerVector[i][j] + _other_matrix._InnerVector[i][j] * (_add ? 1 : -1);

	return _result;
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::plus(const Matrix<T> & other_matrix) const {
	return Matrix<T>::_additionBase(other_matrix, true);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::minus(const Matrix<T> & other_matrix) const {
	static_assert(std::is_arithmetic_v<T>, "minus(const Matrix<T>&) is not available for the specified type.");

	return Matrix<T>::_additionBase(other_matrix, false);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::multiplyBy(const T & value) const {
	static_assert(std::is_arithmetic_v<T>, "multiplyBy(const T&) is not available for the specified type.");

	if (_isMatEmpty(*this))
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	Result_Mat _result = { *this, ErrorCodes::SUCCESS };

	for (auto & row : _result.mat._InnerVector)
		for (T & element : row)
			element *= value;

	return _result;
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::multiplyBy(const Matrix<T> & other_matrix) const {
	static_assert(std::is_arithmetic_v<T>, "multiplyBy(const Matrix<T>&) is not available for the specified type.");

	if (!_checkMatricesDimensions_Multiplication(*this, other_matrix))
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	if (_isMatEmpty(*this) || _isMatEmpty(other_matrix))
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };
	
	Result_Mat _result = { Matrix(_InnerVector.size(), other_matrix._InnerVector.front().size(), 0), ErrorCodes::SUCCESS };

	for (size_t i = 0; i < _InnerVector.size(); i++)
		for (size_t j = 0; j < other_matrix._InnerVector.front().size(); j++)
			for (size_t p = 0; p < other_matrix._InnerVector.size(); p++)
				_result.mat._InnerVector[i][j] += _InnerVector[i][p] * other_matrix._InnerVector[p][j];

	return _result;
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::divideBy(const T & value) const {
	static_assert(std::is_arithmetic_v<T>, "divideBy(const T&) is not available for the specified type.");

	return multiplyBy(1.0 / value);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::divideBy(const Matrix<T> & other_matrix) const {
	static_assert(std::is_arithmetic_v<T>, "divideBy(const Matrix<T>&) is not available for the specified type.");

	return multiplyBy(other_matrix.getInverse().mat);
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::raiseToPowerOf(unsigned short power) const {
	//DON'T FORGET TO CHANGE THE STATIC_ASSERT MESSAGE!
	static_assert(std::is_arithmetic_v<T>, "raiseToPowerOf(unsigned short) is not available for the specified type.");

	if (!isSquareMatrix())
		return { {}, ErrorCodes::INVALID_DIMENSIONS };

	if (_isMatEmpty(*this))
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX };

	Result_Mat _result { constructIdentityMatrix(_InnerVector.size()), ErrorCodes::SUCCESS };

	for (power; power > 0; power--)
		_result.mat = _result.mat.multiplyBy(*this).mat;

	return _result;
}

template <typename T>
typename Matrix<T>::Result_Val Matrix<T>::getDeterminant() const {
	static_assert(std::is_arithmetic_v<T>, "getDeterminant() is not available for the specified type.");

	if (isEmpty())
		return { 0, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (!isSquareMatrix())
		return { 0, ErrorCodes::INVALID_DIMENSIONS };

	return { _getMatrixDeterminant(*this), ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Val Matrix<T>::getTrace() const {
	static_assert(std::is_arithmetic_v<T>, "getTrace() is not available for the specified type.");

	if (isEmpty())
		return { 0, ErrorCodes::INVALID_EMPTY_MATRIX };

	if (!isSquareMatrix())
		return { 0, ErrorCodes::INVALID_DIMENSIONS };

	T _trace = 0;

	for (size_t i = 0; i < _InnerVector.size(); i++)
		_trace += _InnerVector[i][i];

	return { _trace, ErrorCodes::SUCCESS };
}

template <typename T>
typename Matrix<T>::Result_Mat Matrix<T>::getInverse() const {
	static_assert(std::is_arithmetic_v<T>, "getInverse() is not available for the specified type.");

	if (isEmpty())
		return { {}, ErrorCodes::INVALID_EMPTY_MATRIX};

	if (!isSquareMatrix())
		return { {}, ErrorCodes::INVALID_DIMENSIONS};

	const Result_Val _determinant = getDeterminant();

	if (_determinant.error_code != ErrorCodes::SUCCESS)
		return { {}, _determinant.error_code };

	if (_determinant.val == 0)
		return { {}, ErrorCodes::NO_INVERSE_EXISTS };

	if (_InnerVector.size() == 1)
		return Matrix({ {1} }).multiplyBy(1.0 / _determinant.val);

	if (_InnerVector.size() == 2)
		return Matrix({ {_InnerVector[1][1], -_InnerVector[0][1]}, {-_InnerVector[1][0], _InnerVector[0][0]} }).multiplyBy(1.0 / _determinant.val);

	Matrix<T> _adjointMatrix(_InnerVector.size(), _InnerVector.size(), 0);

	for (size_t i = 0; i < _InnerVector.size(); i++)
		for (size_t j = 0; j < _InnerVector.size(); j++)
			_adjointMatrix._InnerVector[i][j] = std::pow(-1, i + j) * _getMatrixDeterminant(_getSubMatrix(*this, i, j));

	return _adjointMatrix.getTranspose().mat.multiplyBy(1.0 / _determinant.val);
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> & other_matrix) const {
	return plus(other_matrix).mat;
}

template <typename T>
Matrix<T> & Matrix<T>::operator+=(const Matrix<T> & other_matrix) {
	*this = plus(other_matrix).mat;
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> & other_matrix) const {
	return minus(other_matrix).mat;
}

template <typename T>
Matrix<T> Matrix<T>::operator-() const {
	return multiplyBy(-1.0).mat;
}

template <typename T>
Matrix<T> & Matrix<T>::operator-=(const Matrix<T> & other_matrix) {
	*this = minus(other_matrix).mat;
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T value) const {
	return multiplyBy(value).mat;
}

template <typename T>
Matrix<T> operator*(const T value, const Matrix<T> & mat) {
	return mat.multiplyBy(value).mat;
}

template <typename T>
Matrix<T> & Matrix<T>::operator*=(const T value) {
	*this = multiplyBy(value).mat;
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> & other_matrix) const {
	return multiplyBy(other_matrix).mat;
}

template <typename T>
Matrix<T> & Matrix<T>::operator*=(const Matrix<T> & other_matrix) {
	*this = multiplyBy(other_matrix).mat;
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(const T value) const {
	return divideBy(value).mat;
}

template <typename T>
Matrix<T> operator/(const T value, const Matrix<T> & mat) {
	return mat.getInverse().mat.multiplyBy(value).mat;
}

template <typename T>
Matrix<T> & Matrix<T>::operator/=(const T value) {
	*this = divideBy(value).mat;
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(const Matrix<T> & other_matrix) const {
	return divideBy(other_matrix).mat;
}

template <typename T>
Matrix<T> & Matrix<T>::operator/=(const Matrix<T> & other_matrix) {
	*this = divideBy(other_matrix).mat;
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator^(const unsigned short power) const {
	return raiseToPowerOf(power).mat;
}

template <typename T>
Matrix<T> & Matrix<T>::operator^=(const unsigned short power) {
	*this = raiseToPowerOf(power).mat;
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator~() const {
	return getTranspose().mat;
}

template <typename T>
Matrix<T> Matrix<T>::operator!() const {
	return getInverse().mat;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> & other_matrix) const {
	return isEqualTo(other_matrix);
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T> & other_matrix) const {
	return !isEqualTo(other_matrix);
}

template <typename T>
std::ostream & operator<<(std::ostream & out, const Matrix<T> & matrix) {
	if (matrix._InnerVector.empty()) {
		out << "[]";
		return out;
	}
	
	for (const auto & row : matrix._InnerVector) {
		if (row.empty()) {
			out << "[]\n";
			continue;
		}

		out << '[';
	
		for (size_t i = 0; i < row.size() - 1; i++)
			out << row[i] << '\t';
	
		out << row.back() << "]\n";
	}
	
	return out;
}
