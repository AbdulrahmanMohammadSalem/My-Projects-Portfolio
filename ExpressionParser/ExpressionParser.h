// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include <sstream>
#include <iomanip>
#include <cmath>

class ExpressionParser {
public:
	enum enEvaluationResults : short { 
		eNoErrors, eInvalidExpression, eCannotDivideByZero, eEvaluationOutOfRange, eInvalidExponentiation,
		eInvalidPermutationsOperands, eInvalidCombinationsOperands
	};
	
	struct Result {
		double value = 0;
		enEvaluationResults evaluationResult = enEvaluationResults::eNoErrors;
	};

private:
	std::string _expression;
	short _precision;

	class CommonUtils {
	public:
		//Returns std::string::npos when failing
		static size_t findRespectiveBracketPos(const std::string & _exp, size_t _bracketPos) {
			if (_bracketPos >= _exp.length())
				return std::string::npos;

			short _netBrackets; //I think that using size_t here is overkill

			if (_exp[_bracketPos] == '(') { //We will move forward
				_netBrackets = 1;
				_bracketPos++;

				while (_netBrackets > 0 && _bracketPos < _exp.length()) {
					if (_exp[_bracketPos] == '(')
						_netBrackets++;
					else if (_exp[_bracketPos] == ')')
						_netBrackets--;

					_bracketPos++;
				}

				_bracketPos--;

			}
			else if (_exp[_bracketPos] == ')') { //We will move backwards
				_netBrackets = -1;
				_bracketPos--;

				while (_netBrackets < 0 && _bracketPos != std::string::npos) {
					if (_exp[_bracketPos] == '(')
						_netBrackets++;
					else if (_exp[_bracketPos] == ')')
						_netBrackets--;

					_bracketPos--;
				}

				_bracketPos++;
			}
			else
				return std::string::npos;

			if (_netBrackets != 0)
				return std::string::npos;

			return _bracketPos;
		}

		static std::string convertDoubleToStr(const double & d, const short & _precision, const bool & _insertSign = true) {
			std::string _result;

			{
				std::ostringstream _oss;
				_oss << std::fixed << std::setprecision(_precision) << d;
				_result = _oss.str();
			}

			size_t _firstNonZeroPos = _result.length() - 1;

			//To cut off trailing zeros after the decimal point:
			if (_result.find('.') != std::string::npos) { //If the number is decimal -- If _precision != 0
				while (_result[_firstNonZeroPos] == '0')
					_firstNonZeroPos--;

				_result.erase(_firstNonZeroPos + 1, _result.length() - _firstNonZeroPos);

				if (_result.back() == '.')
					_result.pop_back();
			}

			if (_insertSign)
				return d >= 0 ? '+' + _result : _result;

			return _result;
		}

		static std::string extractRightValue(const std::string & _exp, const size_t & _operationPos) {
			size_t _outerNumPos = _operationPos + 1;

			if (_exp[_outerNumPos] == '+' || _exp[_outerNumPos] == '-')
				_outerNumPos++;

			while (_outerNumPos < _exp.length() && (_exp[_outerNumPos] == '.' || isdigit(_exp[_outerNumPos])))
				_outerNumPos++;

			_outerNumPos--;

			return _exp.substr(_operationPos + 1, _outerNumPos - _operationPos);
		}
	};

	class ExpressionFormatter {
	private:
		short _precision;

		static bool _validatePi(const std::string & _exp) {
			size_t _pPos = 0;

			while ((_pPos = _exp.find('p', _pPos)) != std::string::npos) {
				if (_pPos == _exp.length() - 1 || _exp[_pPos + 1] != 'i')
					return false;
				else
					_pPos++;
			}

			return true;
		}

		static std::string _removeSpaces(const std::string & _exp) {
			std::string _result = "";

			for (const char & C : _exp)
				if (C != ' ')
					_result += C;

			return _result;
		}

		static std::string _collapseAddSubSigns(const std::string & _exp) {
			std::string _result = "";
			size_t _minusCounter = 0;

			for (size_t i = 0; i < _exp.length(); i++) {
				if (_exp[i] != '+' && _exp[i] != '-')
					_result += _exp[i];
				else {
					for (i; _exp[i] == '+' || _exp[i] == '-'; i++)
						if (_exp[i] == '-')
							_minusCounter++;

					if (_minusCounter % 2) //If odd
						_result += '-';
					else
						_result += '+';

					_minusCounter = 0;
					i--;
				}
			}

			return _result;
		}

		static std::string _formatDecimalPoints(const std::string & _exp) {
			if (_exp.length() == 1) {
				if (_exp.front() == '.')
					return "0";
				else
					return _exp;
			}

			std::string _result = "";

			if (_exp.front() == '.') {
				if (isdigit(_exp[1]))
					_result += '.';
				else if (_exp[1] == '.')
					return ""; //This will cease any future processes, and force exit with _eInvalidExpression result
				else
					_result += '0';
			}
			else
				_result += _exp.front();

			for (size_t i = 1; i < _exp.length() - 1; i++) {
				if (_exp[i] == '.') {
					if (!(isdigit(_exp[i - 1]) || _exp[i - 1] == '.' || isdigit(_exp[i + 1]) || _exp[i + 1] == '.'))
						_result += '0';
					else
						_result += '.'; //Might be rejected later
				}
				else
					_result += _exp[i];
			}

			if (_exp.back() == '.') {
				if (!isdigit(_exp[_exp.length() - 2]) && _exp[_exp.length() - 2] != '.')
					_result += '0';
				else
					return ""; //This will cease any future processes, and force exit with _eInvalidExpression result
			}
			else
				_result += _exp.back();

			return _result;
		}

		static std::string _replaceAll(std::string _str, const std::string & _replaceFrom, const char & _replaceTo) {
			size_t _firstPos = 0;

			while ((_firstPos = _str.find(_replaceFrom, _firstPos)) != std::string::npos) {
				_str.replace(_firstPos, _replaceFrom.length(), std::string() + _replaceTo);
				_firstPos++;
			}

			return _str;
		}

		std::string _formatConstantsAndScientificNotation(const std::string & _exp) {
			const double _pi = 3.14159265358979324;
			const double _e = 2.71828182845904524;

			std::string _result, _rightVal;

			for (size_t i = 0; i < _exp.length(); i++) {
				if (_exp[i] == 'p') {
					if (i > 0 && (isdigit(_exp[i - 1]) || std::string(").ep").find(_exp[i - 1]) != std::string::npos)) //Implicit multiplication
						_result += '*' + CommonUtils::convertDoubleToStr(_pi, _precision, false);
					else
						_result += CommonUtils::convertDoubleToStr(_pi, _precision, false);

					if (i < _exp.length() - 1 && (isdigit(_exp[i + 1]) || _exp[i + 1] == '.'))
						return ""; //This will cease any future processes, and force exit with _eInvalidExpression result
				}
				else if (_exp[i] == 'e') {
					if (i < _exp.length() - 1 && (isdigit(_exp[i + 1]) || _exp[i + 1] == '.')) { //Scientific notation
						_rightVal = CommonUtils::extractRightValue(_exp, i);
						_result += "*10^" + _rightVal;

						i += _rightVal.length(); //The for loop will further increase the i by 1
					}
					else { //Euler's numbers
						if (i > 0 && (isdigit(_exp[i - 1]) || std::string(").ep").find(_exp[i - 1]) != std::string::npos)) //Implicit multiplication
							_result += '*' + CommonUtils::convertDoubleToStr(_e, _precision, false);
						else
							_result += CommonUtils::convertDoubleToStr(_e, _precision, false);
					}
				}
				else
					_result += _exp[i];
			}

			return _result;
		}

		static std::string _formatImplicitMultiplication(const std::string & _exp, const bool & _implicitMultHighPrec) {
			std::string _result;

			if (_implicitMultHighPrec) { //Giving higher precedence:
				_result = _exp;
				size_t _openBracketPos = 0, _clonePos, _partialExpressionPos, _partialExpressionLength;
				std::string _partialExpression;

				while ((_openBracketPos = _result.find('(', _openBracketPos)) != std::string::npos) {
					//Identifying implicit multiplication:
					if (_openBracketPos > 0 && (isdigit(_result[_openBracketPos - 1]) || std::string(").").find(_result[_openBracketPos - 1]) != std::string::npos || (_result[_openBracketPos - 1] == '-' && !(isdigit(_exp[_openBracketPos - 1]) || std::string(").").find(_exp[_openBracketPos - 1]) != std::string::npos)))) {
						//Inserting the open bracket:
						_clonePos = _openBracketPos;

						do {
							_clonePos--;

							if (_result[_clonePos] == ')') {
								size_t _temp = CommonUtils::findRespectiveBracketPos(_result, _clonePos);

								if (_temp != std::string::npos)
									_clonePos = _temp;
								else
									return ""; //This will cease any future processes, and force exit with _eInvalidExpression result
							}
							else if (!isdigit(_result[_clonePos]) && std::string(".^").find(_result[_clonePos]) == std::string::npos) //DON'T PUT P&C IN THE TEMP STRING!
								break;
						} while (_clonePos > 0);

						if (_clonePos == 0)
							_result.insert(_result.begin(), '(');
						else
							_result.insert(_result.begin() + _clonePos + 1, '(');

						//Inserting the closed bracket:
						_clonePos = (++_openBracketPos); //Because we inserted the open bracket above, _openBracketPos shifted back by 1

						do {
							if (_result[_clonePos] == '(') {
								size_t _temp = CommonUtils::findRespectiveBracketPos(_result, _clonePos);

								if (_temp != std::string::npos) {
									_partialExpressionPos = _clonePos + 1;
									_clonePos = _temp;

									_partialExpression = _result.substr(_partialExpressionPos, _clonePos - _partialExpressionPos);
									_partialExpressionLength = _partialExpression.length();
									_partialExpression = _formatImplicitMultiplication(_partialExpression, true);

									if (_partialExpression.empty())
										return _partialExpression; //This will cease any future processes, and force exit with _eInvalidExpression result

									_result.replace(_partialExpressionPos, _clonePos - _partialExpressionPos, _partialExpression); //To format inner expressions containing implicit multiplication
									_clonePos += 1 + _partialExpression.length() - _partialExpressionLength; //To correctly move the cursor after insertion
								}
								else
									return "";
							}
							else if (isdigit(_result[_clonePos]) || std::string(".^").find(_result[_clonePos]) != std::string::npos) //DON'T PUT P&C IN THE TEMP STRING!
								_clonePos++;
							else
								break;
						} while (_openBracketPos < _result.length() - 1);

						// The _clonePos is now on the correct character
						_result.insert(_result.begin() + _clonePos, ')');

						_openBracketPos = (_clonePos += 2); //To save a fraction of a second in the find method in the upper while loop, by skiping one iteration in it
					}
					else
						_openBracketPos++;
				}

				return _formatImplicitMultiplication(_result, false);
			}
			else { //Giving normal precendence && multiplication signs
				for (size_t i = 0; i < _exp.length(); i++) {
					if (_exp[i] != '(')
						_result += _exp[i];
					else if (i > 0 && (isdigit(_exp[i - 1]) || std::string(").").find(_exp[i - 1]) != std::string::npos))
						_result += "*(";
					else
						_result += '(';
				}

				return _result;
			}
		}

		static void _insertBrackets_PC(std::string & _exp) {
			size_t _operatorPos = 0, _clonePos;

			while ((_operatorPos = _exp.find_first_of("PC", _operatorPos)) != std::string::npos) {
				_clonePos = _operatorPos - 1;

				//Inserting the open bracket:
				while (_clonePos > 0 && (isdigit(_exp[_clonePos]) || _exp[_clonePos] == '.'))
					_clonePos--;
				
				if (_exp[_clonePos] == '-' && (_clonePos == 0 || !(isdigit(_exp[_clonePos - 1]) || std::string(").").find(_exp[_clonePos - 1]) != std::string::npos))) {
					_exp.insert(_exp.begin() + _clonePos, '(');
					_exp.insert(_exp.begin() + _operatorPos + 1, ')'); //We add 1 because of the open bracket inserted
				
					_operatorPos += 3; //Because we inserted 2 characters + shifting it one more character
				}
				else
					_operatorPos += 2; //To step after the operator found by 2 steps
			}
		}

		static char _getMagicCharacter(const char & _operator) {
			switch (_operator) {
				case '^': return '`';
				case 'P': return ';';
				case 'C': return ':';
			}
		}
		
		static void _insertMagicCharacters(std::string & _exp) {
			size_t _nextOperatorPos = 0;
			size_t _openBracketPos = 0;

			while ((_nextOperatorPos = _exp.find_first_of("^PC", _nextOperatorPos)) != std::string::npos) {
				if (_exp[_nextOperatorPos - 1] == ')' && _exp.find('(') != std::string::npos) {
					_openBracketPos = CommonUtils::findRespectiveBracketPos(_exp, _nextOperatorPos - 1);

					if (_openBracketPos == 0 || std::string("PC").find(_exp[_openBracketPos - 1]) == std::string::npos) {
						_exp.insert(_exp.begin() + _openBracketPos, _getMagicCharacter(_exp[_nextOperatorPos]));
						
						_nextOperatorPos += 2;
					}
					else
						_nextOperatorPos++;
				}
				else
					_nextOperatorPos++;
			}
		}

	public:
		ExpressionFormatter(const short & precision) {
			_precision = precision;
		}

		//This function early returns with "" in case of the expression is not valid
		std::string formatExpression(const std::string & exp, const bool & implicitMultHighPrec) {
			if (exp.empty())
				return "0";

			std::string _result = exp;
			
			//Removing spaces:
			_result = _removeSpaces(_result);

			//Formatting ++/--/+-/-+:
			_result = _collapseAddSubSigns(_result);

			//Format decimal points:    5.+2   -->   5+2    /   5*.   -->   5*0
			_result = _formatDecimalPoints(_result); //This method supports early exit

			if (_result.empty())
				return ""; //This will cease any future processes, and force exit with _eInvalidExpression result

			//To replace e & pi with their respective values, and format scientific notation with e
			if (!_validatePi(_result))
				return ""; //This will cease any future processes, and force exit with _eInvalidExpression result
			
			_result = _replaceAll(_result, "pi", 'p');
			
			if (_result.find_first_of("ep") != std::string::npos)
				_result = _formatConstantsAndScientificNotation(_result); //This method supports early exit

			if (_result.empty())
				return _result; //This will cease any future processes, and force exit with _eInvalidExpression result

			_result = _formatImplicitMultiplication(_result, implicitMultHighPrec); //This method supports early exit

			if (_result.empty())
				return _result; //This will cease any future processes, and force exit with _eInvalidExpression result

			//Formatting loose negative numbers with P & C:
			_insertBrackets_PC(_result);

			//Inserting magic characters with "^PC" with brackets
			_insertMagicCharacters(_result); //(^ -> `)   (P ->  ;)   (C -> :)

			return _result;
		}
	};

	class ExpressionValidator {
	private:
		static bool _validateCharacters(const std::string & _exp) {
			//Validating all characters:
			std::string _allowedChars = "`;:.()+-*/^PC"; //These are the non-digit characters allowed to be in an expression
			for (const char & C : _exp)
				if (!isdigit(C) && _allowedChars.find(C) == std::string::npos)
					return false;

			//Validating first character:
			_allowedChars = ".(+-`;:"; //These are the non-digit characters allowed to be as the first character in an expression
			if (!isdigit(_exp.front()) && _allowedChars.find(_exp.front()) == std::string::npos)
				return false;

			//Validating last character:
			_allowedChars = ".)"; //These are the non-digit characters allowed to be as the last character in an expression
			if (!isdigit(_exp.back()) && _allowedChars.find(_exp.back()) == std::string::npos)
				return false;

			return true;
		}

		static bool _validateMagicChars(const std::string & _exp) {
			size_t _magicCharPos = 0;

			while ((_magicCharPos = _exp.find_first_of("`;:", _magicCharPos)) != std::string::npos) {
				if (_magicCharPos == _exp.length() - 1 || _exp[_magicCharPos + 1] != '(')
					return false;

				_magicCharPos++;
			}

			return true;
		}

		static bool _validateDecimalPoints(const std::string & _exp) {
			size_t _posCounter = 0, _pointPos = 0;

			while ((_pointPos = _posCounter = _exp.find('.', _pointPos)) != std::string::npos) {
				for (_posCounter; isdigit(_exp[_posCounter]) || _exp[_posCounter] == '.'; _posCounter++)
					if (isdigit(_exp[_posCounter]) && _exp[_posCounter + 1] == '.')
						return false;

				_pointPos++;
			}

			return true;
		}

		static bool _validateBrackets(const std::string & _exp) {
			//Validation level 1:
			if (_exp.find("()") != std::string::npos)
				return false;

			//Validation level 2:
			size_t _firstOpenBracketPos = _exp.find('(');
			if (_firstOpenBracketPos != std::string::npos && CommonUtils::findRespectiveBracketPos(_exp, _firstOpenBracketPos) == std::string::npos)
				return false;

			//Validation level 3:
			for (size_t i = 0; i < _exp.length(); i++) { //DO NOT OMIT THE FIRST & LAST CHARACTERS HERE!
				if (_exp[i] == '(' && std::string("*/^PC").find(_exp[i + 1]) != std::string::npos)
					return false;

				else if (_exp[i] == ')') {
					if (std::string("+-*/^PC`;:").find(_exp[i - 1]) != std::string::npos)
						return false;

					if (isdigit(_exp[i + 1]) || _exp[i + 1] == '.')
						return false;
				}
			}

			return true;
		}

		static bool _validateSequentialChars(const std::string & _exp) {
			std::string _allowedChars = "()+-"; //These are the non-digit characters allowed to repeat sequentially

			for (size_t i = 0; i < _exp.length() - 1; i++) {
				if (_exp[i] == _exp[i + 1]) {
					if (!isdigit(_exp[i]) && _allowedChars.find(_exp[i]) == std::string::npos)
						return false;
				}
				else {
					if (std::string("*/^PC").find(_exp[i]) != std::string::npos) {
						if (std::string("*/^PC").find(_exp[i + 1]) != std::string::npos)
							return false;
					}
					else if ((_exp[i] == '+' || _exp[i] == '-') && std::string("*/^PC").find(_exp[i + 1]) != std::string::npos) {
						return false;
					}
				}
			}

			return true;
		}

	public:
		static bool isExpressionValid(const std::string & _exp) {
			if (_exp.empty())
				return true; //The value is zero

			if (!_validateCharacters(_exp))
				return false;

			if (!_validateMagicChars(_exp))
				return false;
			
			if (!_validateDecimalPoints(_exp))
				return false;

			if (!_validateBrackets(_exp))
				return false;

			if (!_validateSequentialChars(_exp))
				return false;

			return true;
		}
	};

	class ExpressionEvaluator {
	private:
		enEvaluationResults _evaluationResult;
		short _precision;

		static bool _isNumericValue(const std::string & str) {
			size_t i = 0;

			if (str.front() == '-' || str.front() == '+')
				i++;

			for (i; i < str.length(); i++)
				if (!isdigit(str[i]) && str[i] != '.')
					return false;

			return true;
		}

		static size_t _findCorrectExponentPos(const std::string & _partialExpression) {
			size_t _firstPos;
			
			if ((_firstPos = _partialExpression.find('^')) != std::string::npos) {
				size_t _clonePos = _firstPos;

				do {
					_clonePos++;
					
					while (isdigit(_partialExpression[_clonePos]) || std::string("+-.`;:").find(_partialExpression[_clonePos]) != std::string::npos)
						_clonePos++;

					if (_partialExpression[_clonePos] == '^')
						_firstPos = _clonePos;
					else
						break;
				} while (true);
			}

			return _firstPos;
		}

		//This extracts the value with its sign if possible
		static std::string _extractLeftValue(const std::string & _exp, const size_t & _operationPos) {
			size_t _outerNumPos = _operationPos - 1;

			while (_outerNumPos > 0 && (_exp[_outerNumPos] == '.' || isdigit(_exp[_outerNumPos]) || std::string("`;:").find(_exp[_outerNumPos - 1]) != std::string::npos))
				_outerNumPos--;

			if (std::string("`;:*/^PC").find(_exp[_outerNumPos]) != std::string::npos)
				_outerNumPos++;

			return _exp.substr(_outerNumPos, _operationPos - _outerNumPos);
		}

		//With brackets -- This avoids the need to collapse signs after insertion
		void _insertSubPartialResult(std::string & _partialExpression, const size_t & _openBracketPos, const size_t & _closedBracketPos, const double & _subPartialResult) {
			if (_openBracketPos > 0) { //If the expression doesn't start with the open bracket
				if (_partialExpression[_openBracketPos - 1] == '+') //If the character before the open bracket is a positive sign
					_partialExpression.replace(_openBracketPos - 1, _closedBracketPos + 3, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
				else if (_partialExpression[_openBracketPos - 1] == '-') { //If the character before the open bracket is a negative sign
					if (_subPartialResult >= 0) //If the result is positive or zero
						_partialExpression.replace(_openBracketPos, _closedBracketPos + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision, false));
					else //If the result is negative
						_partialExpression.replace(_openBracketPos - 1, _closedBracketPos + 3, CommonUtils::convertDoubleToStr(-_subPartialResult, _precision));
				}
				else //For any other character types before the open bracket
					_partialExpression.replace(_openBracketPos, _closedBracketPos + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
			}
			else //If the expression starts with the open bracket
				_partialExpression.replace(0, _closedBracketPos + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
		}

		//Without brackets -- This avoids the need to collapse signs after insertion
		void _insertSubPartialResult(std::string & _partialExpression, const size_t & _operationPos, const std::string & _num1, const std::string & _num2, const double & _subPartialResult) {
			if (_operationPos > _num1.length()) { //If the expression doesn't start with _num1
				if (std::string("`;:").find(_partialExpression[_operationPos - _num1.length() - 1]) != std::string::npos) { //If there's a magic character directly before _num1
					if (_operationPos - _num1.length() >= 2) { //If there is a character before the magic character
						if (_partialExpression[_operationPos - _num1.length() - 2] == '+') //If there's a positive sign before the magic character
							_partialExpression.replace(_operationPos - _num1.length() - 2, _num1.length() + _num2.length() + 3, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
						else if (_partialExpression[_operationPos - _num1.length() - 2] == '-') { //If there's a nagative sign before the magic character
							if (_subPartialResult >= 0) //If the result is positive
								_partialExpression.replace(_operationPos - _num1.length() - 1, _num1.length() + _num2.length() + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision, false));
							else //If the result is negative
								_partialExpression.replace(_operationPos - _num1.length() - 2, _num1.length() + _num2.length() + 3, CommonUtils::convertDoubleToStr(-_subPartialResult, _precision));
						}
						else //For any other character before the magic character
							_partialExpression.replace(_operationPos - _num1.length() - 1, _num1.length() + _num2.length() + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
					}
					else //If there's not a character before the magic character
						_partialExpression.replace(0, _num1.length() + _num2.length() + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
				}
				else { //If there's not a magic character directly before _num1
					//We check like this because the expression does NOT start with _num1 as seen above
					if (_num1.front() == '-') { //If _num1 is negative
						if (_subPartialResult >= 0) //If the result is positive
							_partialExpression.replace(_operationPos - _num1.length(), _num1.length() + _num2.length() + 1, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
						else //If the result is negative
							_partialExpression.replace(_operationPos - _num1.length(), _num1.length() + _num2.length() + 1, CommonUtils::convertDoubleToStr(-_subPartialResult, _precision));
					}
					else //If _num1 is positive
						_partialExpression.replace(_operationPos - _num1.length(), _num1.length() + _num2.length() + 1, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
				}
			}
			else //If the expression starts with _num1
				_partialExpression.replace(0, _num1.length() + _num2.length() + 1, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
		}

		bool _isWhole(const double & d) {
			return std::floor(d) == d;
		}

		double _factorial(double n) {
			double _result = 1;

			for (n; n > 1; n--)
				_result *= n;

			return _result;
		}

		double _permutations(const double & _num1, const double & _num2) {
			double result = 1;

			for (int i = 0; i < _num2; i++)
				result *= (_num1 - i);
			
			return result;
		}

		double _combinations(const double & _num1, const double & _num2) {
			double result = 1;

			for (int i = 1; i <= _num2; i++) {
				result *= (_num1 - _num2 + i);
				result /= i;
			}

			return result;
		}

		double _calculate(const double & _num1, const double & _num2, const char & _operation) {
			double _result = 0;

			switch (_operation) {
				case '+':
					_result = _num1 + _num2;
					break;
				case '-':
					_result = _num1 - _num2;
					break;
				case '*':
					_result = _num1 * _num2;
					break;
				case '/':
					if (_num2 == 0) {
						_evaluationResult = enEvaluationResults::eCannotDivideByZero; //This will cease any future processes, and force exit with eCannotDivideByZero result
						return 0; //This is important to skip the OutOfRange validation below
					}

					_result = _num1 / _num2;
					break;
				case '^':
					if (_num1 == 0 && _num2 == 0) {
						_evaluationResult = enEvaluationResults::eInvalidExponentiation; //This will cease any future processes, and force exit with eInvalidExponentiation result
						return 0;
					}

					_result = pow(_num1, _num2);
					break;
				case 'P':
					if (_num1 < 0 || _num2 < 0 || !_isWhole(_num1) || !_isWhole(_num2) || _num2 > _num1) {
						_evaluationResult = enEvaluationResults::eInvalidPermutationsOperands; //This will cease any future processes, and force exit with eInvalidPermutationsOperands result
						return 0;
					}

					_result = _permutations(_num1, _num2);
					break;
				case 'C':
					if (_num1 < 0 || _num2 < 0 || !_isWhole(_num1) || !_isWhole(_num2) || _num2 > _num1) {
						_evaluationResult = enEvaluationResults::eInvalidCombinationsOperands; //This will cease any future processes, and force exit with eInvalidCombinationsOperands result
						return 0;
					}

					_result = _combinations(_num1, _num2);
					break;
			}

			if (_result == INFINITY || _result == -INFINITY) {
				_evaluationResult = enEvaluationResults::eEvaluationOutOfRange; //This will cease any future processes, and force exit with eEvaluationOutOfRange result
				return 0;
			}
			else if (isnan(_result)) {
				_evaluationResult = enEvaluationResults::eInvalidExponentiation; //This will cease any future processes, and force exit with eInvalidExponentiation result
				return 0;
			}

			return _result == 0 ? 0 : _result; //To normalize -0.0
		}

		double _evaluatePartial(std::string & _partialExpression) {
			if (_evaluationResult != enEvaluationResults::eNoErrors)
				return 0;

			if (_partialExpression.empty())
				return 0;

			if (_isNumericValue(_partialExpression)) {
				double _val = stod(_partialExpression);
				return _val == 0 ? 0 : _val; //To normalize -0.0
			}
			
			double _subPartialResult = 0;
			size_t _operatorPos;

			//Brackets:
			if ((_operatorPos = _partialExpression.find('(')) != std::string::npos) {
				size_t _lastPos = CommonUtils::findRespectiveBracketPos(_partialExpression, _operatorPos); //Closed Bracket
				std::string _subPartialExpression = _partialExpression.substr(_operatorPos + 1, _lastPos - _operatorPos - 1);

				_lastPos = _subPartialExpression.length(); //I needed to save the length of the sub-partial expression for later, so I used this variable instead of creating a new one and consume more memory :-)
				_subPartialResult = _evaluatePartial(_subPartialExpression);

				_insertSubPartialResult(_partialExpression, _operatorPos, _lastPos, _subPartialResult);

				return _evaluatePartial(_partialExpression);
			}

			//Exponents:
			if (_partialExpression.find('^') != std::string::npos)
				_operatorPos = _findCorrectExponentPos(_partialExpression); //Because 2^3^4 = 2^(3^4) NOT (2^3)^4

			//Permutations/Combinations:
			else _operatorPos = _partialExpression.find_first_of("PC");

			//Multiplication/Division:
			if (_operatorPos == std::string::npos) //BE CAREFUL NOT TO ADD `else`
				_operatorPos = _partialExpression.find_first_of("*/");

			//Addition/Subtraction:
			if (_operatorPos == std::string::npos) { //BE CAREFUL NOT TO ADD `else`
				if (std::string("+-").find(_partialExpression.front()) != std::string::npos) 
					_operatorPos = _partialExpression.find_first_of("+-", 1);
				else
					_operatorPos = _partialExpression.find_first_of("+-");
			}

			//Extracting _num1:
			std::string _num1 = _extractLeftValue(_partialExpression, _operatorPos);

			//Extracting _num2:
			std::string _num2 = CommonUtils::extractRightValue(_partialExpression, _operatorPos);

			_subPartialResult = _calculate(stod(_num1), stod(_num2), _partialExpression[_operatorPos]);

			_insertSubPartialResult(_partialExpression, _operatorPos, _num1, _num2, _subPartialResult);

			return _evaluatePartial(_partialExpression);
		}

	public:
		ExpressionEvaluator(const short & precision) {
			_evaluationResult = enEvaluationResults::eNoErrors;
			_precision = precision;
		}

		Result initiateEvaluation(std::string _fullExpression) {
			double _value = _evaluatePartial(_fullExpression);
			
			return { _value == 0 ? 0 : _value, _evaluationResult }; //To normalize -0.0
		}
	};

	static short _clampNumberBetween(const short & _num, const short & _from, const short & _to) {
		if (_num > _to)
			return _to;

		if (_num < _from)
			return _from;

		return _num;
	}

public:
	ExpressionParser(const std::string & expression = "0", const short & precision = 6) {
		_expression = expression;
		_precision = _clampNumberBetween(precision, 0, 17);
	}

	std::string getExpression() const {
		return _expression;
	}

	void setExpression(const std::string & expression) {
		_expression = expression;
	}
	
	short getPrecision() const {
		return _precision;
	}

	//* This controls the precision of the result as well as the internal calculations.
	//* Default value is 6 digits after the decimal point
	//* It will get clamped between [0,17]
	void setPrecision(const short & precision) {
		_precision = _clampNumberBetween(precision, 0, 17);
	}

	//* The parameter controls whether implicit multiplication (juxtaposition) binds more tightly than standard left-to-right evaluation.
	//* If true, it will have higher precedence. Otherwise, it will not.
	Result evaluate(const bool implicitMultHighPrec) {
		std::string _formattedExpression = ExpressionFormatter(_precision).formatExpression(_expression, implicitMultHighPrec);

		if (!_formattedExpression.empty() && ExpressionValidator::isExpressionValid(_formattedExpression))
			return ExpressionEvaluator(_precision).initiateEvaluation(_formattedExpression);

		return { 0, enEvaluationResults::eInvalidExpression };
	}
};
