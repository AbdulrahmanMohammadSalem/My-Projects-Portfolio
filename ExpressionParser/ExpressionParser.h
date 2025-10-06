// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com                                                      

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      _____                              _             ____                               //
//     | ____|_  ___ __  _ __ ___  ___ ___(_) ___  _ __ |  _ \ __ _ _ __ ___  ___ _ __      //
//     |  _| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \| |_) / _` | '__/ __|/ _ \ '__|     //
//     | |___ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |  __/ (_| | |  \__ \  __/ |        //
//     |_____/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|_|   \__,_|_|  |___/\___|_|        //
//                |_|                                                                       //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <stack>
#include <vector>
#include <unordered_map>

class ExpressionParser {
public:
	enum ErrorCodes : short {
		NO_ERRORS, INVALID_EXPRESSION, INVALID_IMPLICIT_MULTIPLICATION, INVALID_DECIMAL_POINTS, INVALID_BRACKETS,
		INVALID_SCIENTIFIC_NOTATION, INVALID_OPERATOR_PLACEMENT, INVALID_FUNCTION_ARGUMENTS, INVALID_FUNCTION_CALL,
		INVALID_ABS_POLES, CANNOT_DIVIDE_BY_ZERO, INVALID_EXPONENTIATION, EVALUATION_OUT_OF_RANGE, INVALID_PERMUTATIONS_OPERANDS,
		INVALID_COMBINATIONS_OPERANDS, INVALID_BITWISE_LEFT_SHIFT_OPERANDS, INVALID_BITWISE_RIGHT_SHIFT_OPERANDS,
		INVALID_BITWISE_NOT_OPERAND, INVALID_BITWISE_XNOR_OPERANDS, INVALID_BITWISE_NAND_OPERANDS, INVALID_BITWISE_NOR_OPERANDS,
		INVALID_BITWISE_XOR_OPERANDS, INVALID_BITWISE_AND_OPERANDS, INVALID_BITWISE_OR_OPERANDS, OUT_OF_DOMAIN_ACOSH,
		OUT_OF_DOMAIN_ATANH, OUT_OF_DOMAIN_ACSCH, OUT_OF_DOMAIN_ASECH, OUT_OF_DOMAIN_ACOTH, OUT_OF_DOMAIN_CSCH, OUT_OF_DOMAIN_COTH,
		OUT_OF_DOMAIN_ASIN, OUT_OF_DOMAIN_ACOS, OUT_OF_DOMAIN_ACSC, OUT_OF_DOMAIN_ASEC, OUT_OF_DOMAIN_ACOT, OUT_OF_DOMAIN_TAN,
		OUT_OF_DOMAIN_CSC, OUT_OF_DOMAIN_SEC, OUT_OF_DOMAIN_COT, OUT_OF_DOMAIN_LOG, OUT_OF_DOMAIN_LOG10, OUT_OF_DOMAIN_LN,
		OUT_OF_DOMAIN_SQRT, OUT_OF_DOMAIN_NTHRT, OUT_OF_DOMAIN_MOD, INVALID_FACTORIAL_INPUT, INVALID_SUM_LIMITS, INVALID_PROD_LIMITS
	};

	enum AngleUnits : short {
		DEGREE, RADIAN, GRADIAN
	};

	struct EvaluationResult {
		double value = 0;
		ErrorCodes errorCode = ErrorCodes::NO_ERRORS;
	};

private:
	enum BracketTypes { REGULAR = 0, SQUARE = 1 };

	std::string _expression;
	short _precision;
	AngleUnits _angleUnit;
	bool _implicitMultHighPrec, _forceBitwiseLogic;
	std::pair<std::string, ErrorCodes> _formattingResult;
	std::unordered_map<char, std::string> _operatorNames;

	class CommonUtils {
	public:
		//Returns std::string::npos when failing
		static size_t findRespectiveBracketPos(const std::string & _exp, size_t _bracketPos, const BracketTypes _bracketType) {
			if (_bracketPos >= _exp.length())
				return std::string::npos;

			const char * _brackets[2] = { "()", "[]" };
			int _netBrackets; //I think that using size_t here is overkill

			if (_exp[_bracketPos] == _brackets[_bracketType][0]) { //We will move forward
				_netBrackets = 1;
				_bracketPos++;

				while (_netBrackets > 0 && _bracketPos < _exp.length()) {
					if (_exp[_bracketPos] == _brackets[_bracketType][0])
						_netBrackets++;
					else if (_exp[_bracketPos] == _brackets[_bracketType][1])
						_netBrackets--;

					_bracketPos++;
				}

				_bracketPos--;

			}
			else if (_exp[_bracketPos] == _brackets[_bracketType][1]) { //We will move backwards
				_netBrackets = -1;
				_bracketPos--;

				while (_netBrackets < 0 && _bracketPos != std::string::npos) {
					if (_exp[_bracketPos] == _brackets[_bracketType][0])
						_netBrackets++;
					else if (_exp[_bracketPos] == _brackets[_bracketType][1])
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

		static std::string convertDoubleToStr(const double d, const short _precision, const bool _insertSign = true) {
			std::string _result;

			{
				std::ostringstream _oss;
				_oss << std::fixed << std::setprecision(_precision) << d;
				_result = _oss.str();
			}

			if (stod(_result) == 0)
				return _insertSign ? "+0" : "0";

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

		static std::string extractRightValue(const std::string & _exp, const size_t _operationPos) {
			if (_operationPos == _exp.length() - 1)
				return "";

			size_t _outerNumPos = _operationPos + 1;

			if (_exp[_outerNumPos] == '+' || _exp[_outerNumPos] == '-')
				_outerNumPos++;

			while (_outerNumPos < _exp.length() && (_exp[_outerNumPos] == '.' || std::isdigit(_exp[_outerNumPos])))
				_outerNumPos++;

			_outerNumPos--;

			return _exp.substr(_operationPos + 1, _outerNumPos - _operationPos);
		}

		//This will extract the value with the sign, unless it's ^PC!% with no magic chars
		static std::string extractLeftValue(const std::string & _exp, const size_t _operatorPos) {
			if (_operatorPos == 0)
				return "";

			size_t _outerNumPos = _operatorPos - 1;

			while (_outerNumPos > 0 && (_exp[_outerNumPos] == '.' || std::isdigit(_exp[_outerNumPos]) || std::string("`;:\'j").find(_exp[_outerNumPos - 1]) != std::string::npos))
				_outerNumPos--;

			if (std::string("`;:\'j*/^PCiygmubdlpr<=>").find(_exp[_outerNumPos]) != std::string::npos)
				_outerNumPos++;

			if (std::string("^PC!%").find(_exp[_operatorPos]) != std::string::npos && std::string("+-").find(_exp[_outerNumPos]) != std::string::npos && (_outerNumPos == 0 || std::string("`;:\'j").find(_exp[_outerNumPos - 1]) == std::string::npos))
				_outerNumPos++;

			return _exp.substr(_outerNumPos, _operatorPos - _outerNumPos);
		}

		static char getMagicCharacter(const char _operator) {
			switch (_operator) {
				case '^': return '`';
				case 'P': return ';';
				case 'C': return ':';
				case '!': return '\'';
				case '%': return 'j';
			}
		}

		//Returns std::string::npos on failure. It will search from the _startPos forward
		static size_t findFirstCommaPos(const std::string & _exp, const size_t _startPos = 0) {
			size_t _commaPos = _startPos;

			while (_commaPos < _exp.length() - 1 && _exp[_commaPos] != ',') {
				if (_exp[_commaPos] == '(')
					_commaPos = CommonUtils::findRespectiveBracketPos(_exp, _commaPos, BracketTypes::REGULAR) + 1;
				else
					_commaPos++;
			}

			if (_commaPos >= _exp.length() - 1) //DON'T REPLACE WITH   _commaPos == _exp.length() - 1
				return std::string::npos;

			return _commaPos;
		}

		static size_t getCharCount(const std::string & _str, const char _chr) {
			size_t _counter = 0;

			for (const char C : _str)
				if (C == _chr)
					_counter++;

			return _counter;
		}

		static ErrorCodes validateBrackets(const std::string & _exp, const BracketTypes _bracketType) {
			const char * _brackets[2] = { "()", "[]" };
			size_t _openBrackets = getCharCount(_exp, _brackets[_bracketType][0]);
			size_t _closedBrackets = getCharCount(_exp, _brackets[_bracketType][1]);

			if (_openBrackets == 0 && _closedBrackets == 0)
				return ErrorCodes::NO_ERRORS;

			//Validation level 1:
			if (_openBrackets != _closedBrackets)
				return ErrorCodes::INVALID_BRACKETS;

			//Validation level 2:
			if (_exp.find(_brackets[_bracketType]) != std::string::npos)
				return ErrorCodes::INVALID_BRACKETS;

			//Validation level 3:
			size_t _pos = _exp.find(_brackets[_bracketType][0]);

			if (_exp.substr(0, _pos).find(_brackets[_bracketType][1]) != std::string::npos)
				return ErrorCodes::INVALID_BRACKETS;

			short _netBrackets = 1; //Because _pos is already standing on 

			do {
				_pos++;

				if (_exp[_pos] == _brackets[_bracketType][0])
					_netBrackets++;
				else if (_exp[_pos] == _brackets[_bracketType][1])
					_netBrackets--;

				if (_netBrackets < 0)
					return ErrorCodes::INVALID_BRACKETS;
			} while (_pos < _exp.length());

			if (_bracketType == BracketTypes::REGULAR) { //Square brackets are already valid at this aspect
				//Validation level 4:
				std::string _twoOperandOperators = "*/^PCiygmubdlpr<=>";
				for (size_t i = 0; i < _exp.length(); i++) { //DO NOT OMIT THE FIRST & LAST CHARACTERS HERE!
					if (_exp[i] == '(' && _twoOperandOperators.find(_exp[i + 1]) != std::string::npos)
						return ErrorCodes::INVALID_OPERATOR_PLACEMENT;

					else if (_exp[i] == ')') {
						if (_twoOperandOperators.find(_exp[i - 1]) != std::string::npos || std::string("+-`;:\'j").find(_exp[i - 1]) != std::string::npos)
							return ErrorCodes::INVALID_OPERATOR_PLACEMENT;

						if (std::isdigit(_exp[i + 1]) || _exp[i + 1] == '.')
							return ErrorCodes::INVALID_IMPLICIT_MULTIPLICATION;
					}
				}
			}

			return ErrorCodes::NO_ERRORS;
		}

		//This method is used for internal use to format each evaluation step.
		static std::string formatExpression(const std::string & _exp, const std::unordered_map<char, std::string> & _operatorNames, const bool _forceDecimalPoints, const bool _forceAsterisks, const bool _useAbsPoles, const bool _useSpaces) {
			const std::string _funChars = "ABDFGHJKLMNOQRSTUVWXYZ~@{$&_?}\"\\ qonkvh", _magicChars = "`;:\'j", _twoOperandOperators = "*/^PCiygmubdlpr<=>"; //We will not put <=> because they are standalone (they are not associated with a string)
			std::string _result, _substr;
			size_t _substrLength;

			for (size_t i = 0; i < _exp.length(); i++) {
				if (_magicChars.find(_exp[i]) != std::string::npos) {
					if (_exp[i + 1] == '+') {
						i++; //To skip that positive sign in the next iteration
						continue;
					}
					else if (_exp[i + 1] == '-') {
						_substr = extractRightValue(_exp, i + 1); //This will NOT extract the negative sign.
						_substrLength = _substr.length();
						_substr = formatExpression(_substr, _operatorNames, _forceDecimalPoints, _forceAsterisks, _useAbsPoles, _useSpaces);
						_result += "(-" + _substr + ')'; //We add the negative sign here.

						i += _substrLength + 1;
					}
					else
						continue;
				}

				else if (std::isdigit(_exp[i])) {
					if (_forceDecimalPoints) {
						if ((i == _exp.length() - 1 || (!std::isdigit(_exp[i + 1]) && _exp[i + 1] != '.')) && extractLeftValue(_exp, i).find('.') == std::string::npos)
							_result += std::string(1, _exp[i]) + ".0";
						else
							_result += _exp[i];
					}
					else
						_result += _exp[i];
				}

				else if (std::string("()PC!%").find(_exp[i]) != std::string::npos)
					_result += _exp[i];

				else if (_exp[i] == '+') {
					if (i != 0 && _twoOperandOperators.find(_exp[i - 1]) == std::string::npos && _exp[i - 1] != '(' && _exp[i - 1] != ',') {
						if (_useSpaces)
							_result += " + ";
						else
							_result += '+';
					}

					//Otherwise, we omit the redundant plus sign and continue
				}

				else if (_exp[i] == '-') {
					if (i > 0 && i < _exp.length() - 1 && _twoOperandOperators.find(_exp[i - 1]) != std::string::npos && (std::isdigit(_exp[i + 1]) || _exp[i + 1] == '.')) {
						_substr = extractRightValue(_exp, i); //This will NOT extract the negative sign.
						_substrLength = _substr.length();
						_substr = formatExpression(_substr, _operatorNames, _forceDecimalPoints, _forceAsterisks, _useAbsPoles, _useSpaces);
						_result += "(-" + _substr + ')'; //We add the negative sign here.

						i += _substrLength;
					}
					else if (i == 0 || _exp[i - 1] == '(' || _exp[i - 1] == ',' || ((_exp[i + 1] == '(' || _exp[i + 1] == 'w') && !std::isdigit(_exp[i - 1]) && std::string(".!%w)").find(_exp[i - 1]) == std::string::npos)) //No need to check if the '-' is not the last char here.
						_result += '-';
					else {
						if (_useSpaces)
							_result += " - ";
						else
							_result += '-';
					}
				}

				else if (std::string("/^").find(_exp[i]) != std::string::npos) {
					if (_useSpaces)
						_result += std::string(1, ' ') + _exp[i] + ' ';
					else
						_result += _exp[i];
				}

				else if (_exp[i] == '*') {
					if (_forceAsterisks) {
						if (_useSpaces)
							_result += " * ";
						else
							_result += '*';
					}
					else {
						//Implicit multiplication: (decimal points are already formatted before)
						if (!(_exp[i + 1] == '(' && (std::isdigit(_exp[i - 1]) || _exp[i - 1] == ')' || _exp[i - 1] == 'w')) && !(_exp[i + 1] == 'w' && (std::isdigit(_exp[i - 1]) || _exp[i - 1] == ')'))) {
							if (_useSpaces)
								_result += " * ";
							else
								_result += '*';
						}
					}
				}

				else if (_exp[i] == '.') {
					if (i == 0 || !std::isdigit(_exp[i - 1]))
						_result += '0'; //This is a must in all cases

					if (_forceDecimalPoints && (i == _exp.length() - 1 || !std::isdigit(_exp[i + 1])))
						_result += ".0";
					else if (i < _exp.length() - 1 && std::isdigit(_exp[i + 1]))
						_result += '.';

					//Otherwise, we continue.
				}

				else if (_exp[i] == '}') { //Absolute value
					if (_useAbsPoles) {
						_substr = _exp.substr(i + 2, findRespectiveBracketPos(_exp, i + 1, BracketTypes::REGULAR) - i - 2);
						_substrLength = _substr.length();
						_substr = formatExpression(_substr, _operatorNames, _forceDecimalPoints, _forceAsterisks, true, _useSpaces);

						_result += std::string(1, '|') + _substr + '|';
						i += _substrLength + 2; //The for loop will then increase the i by one
					}
					else
						_result += "abs";
				}

				else if (_funChars.find(_exp[i]) != std::string::npos)
					_result += _operatorNames.at(_exp[i]);

				else if (_exp[i] == 'w')
					_result += 'x';

				else if (_exp[i] == ',') {
					if (_useSpaces)
						_result += ", ";
					else
						_result += ',';
				}

				else if (_exp[i] == '<' || _exp[i] == '=' || _exp[i] == '>') {
					if (_useSpaces)
						_result += std::string(1, ' ') + _exp[i] + ' ';
					else
						_result += _exp[i];
				}

				else if (_twoOperandOperators.find(_exp[i]) != std::string::npos) { //Other logical operators
					if (_useSpaces)
						_result += ' ' + _operatorNames.at(_exp[i]) + ' ';
					else
						_result += _operatorNames.at(_exp[i]);
				}

				else
					_result += '?'; //For debugging -- Unknown case
			}

			return _result;
		}

		//This avoids the need to collapse signs after plugging in
		static std::string plugInVal(const std::string & _exp, const double _val, const short _precision) {
			std::string _result;

			if (_exp.front() == 'w')
				_result += CommonUtils::convertDoubleToStr(_val, _precision);
			else
				_result += _exp.front();

			for (size_t i = 1; i < _exp.length(); i++) {
				if (_exp[i] == 'w') {
					if (_exp[i - 1] == '+') {
						_result.pop_back();
						_result += CommonUtils::convertDoubleToStr(_val, _precision);
					}
					else if (_exp[i - 1] == '-') {
						if (_val >= 0)
							_result += CommonUtils::convertDoubleToStr(_val, _precision, false);
						else {
							_result.pop_back();
							_result += CommonUtils::convertDoubleToStr(-_val, _precision);
						}
					}
					else
						_result += CommonUtils::convertDoubleToStr(_val, _precision);
				}
				else
					_result += _exp[i];
			}

			return _result;
		}

	};

	class ExpressionFormatter {
	private:
		short _precision;
		const std::string _funChars = "ABDFGHJKLMNOQRSTUVWXYZ~@{$&_?}\"\\ qonkvh";
		const std::unordered_map<char, std::string> & _operatorNames; //This must be a reference

		static std::string _removeSpaces(const std::string & _exp) {
			std::string _result = "";

			for (const char C : _exp)
				if (C != ' ')
					_result += C;

			return _result;
		}

		void _replaceTokens_lowercase(std::string & _exp) {
			for (auto it = _operatorNames.find('i'); it != _operatorNames.end(); it++)
				_exp = _replaceAll(_exp, it->second, std::string(1, ' ') + it->first + ' ');
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
				if (std::isdigit(_exp[1]))
					_result += '.';
				else if (_exp[1] == '.' || std::string("efzx").find(_exp[1]) != std::string::npos)
					return "";
				else
					_result += '0';
			}
			else
				_result += _exp.front();

			for (size_t i = 1; i < _exp.length() - 1; i++) {
				if (_exp[i] == '.') {
					if (std::string("efzx)!%").find(_exp[i - 1]) != std::string::npos || std::string("efzx").find(_exp[i + 1]) != std::string::npos)
						return "";
					if (!(std::isdigit(_exp[i - 1]) || _exp[i - 1] == '.' || std::isdigit(_exp[i + 1]) || _exp[i + 1] == '.'))
						_result += '0';
					else
						_result += '.'; //Might be rejected later
				}
				else
					_result += _exp[i];
			}

			if (_exp.back() == '.') {
				if (!std::isdigit(_exp[_exp.length() - 2]) && _exp[_exp.length() - 2] != '.')
					_result += '0';
				else
					return "";
			}
			else
				_result += _exp.back();

			return _result;
		}

		static std::string _replaceAll(std::string _str, const std::string & _replaceFrom, const std::string & _replaceTo) {
			size_t _firstPos = 0;

			while ((_firstPos = _str.find(_replaceFrom, _firstPos)) != std::string::npos) {
				_str.replace(_firstPos, _replaceFrom.length(), _replaceTo);
				_firstPos++;
			}

			return _str;
		}

		static int _getNetSquareBracketsBefore(const std::string & _exp, const size_t _upUntil) {
			int _netBrackets = 0;

			for (size_t i = 0; i < _upUntil; i++) {
				if (_exp[i] == '[')
					_netBrackets++;
				else if (_exp[i] == ']')
					_netBrackets--;
			}

			return _netBrackets;
		}

		static void _fillAmbiguousPoles(std::string & _exp, const std::vector<size_t> & _vPositions, const char _bracket) {
			for (const size_t P : _vPositions)
				_exp[P] = _bracket;
		}

		static std::pair<std::string, ErrorCodes> _bruteForceAmbiguousPoles(std::string _exp, const std::vector<size_t> & _vPositions, const size_t _ambiguousIndex) {
			if (_ambiguousIndex == _vPositions.size()) { //No more poles
				if (CommonUtils::validateBrackets(_exp, BracketTypes::SQUARE) == ErrorCodes::INVALID_BRACKETS)
					return { "", ErrorCodes::INVALID_ABS_POLES };

				return { _exp, ErrorCodes::NO_ERRORS };
			}

			std::pair<std::string, ErrorCodes> _firstBranch, _secondBranch, _bruteForceResult;

			//First branch:
			_exp[_vPositions[_ambiguousIndex]] = '[';

			_bruteForceResult = _bruteForceAmbiguousPoles(_exp, _vPositions, _ambiguousIndex + 1);

			if (_bruteForceResult.second == ErrorCodes::NO_ERRORS)
				return _bruteForceResult;

			//Second branch:
			_exp[_vPositions[_ambiguousIndex]] = ']';

			return _bruteForceAmbiguousPoles(_exp, _vPositions, _ambiguousIndex + 1);
		}

		std::pair<std::string, ErrorCodes> _formatAbsPoles(std::string & _exp) {
			//We start by filling the definite brackets, then brute force the ambiguous ones.
			size_t i = _exp.find('|');

			if (i == std::string::npos)
				return { _exp, ErrorCodes::NO_ERRORS };

			if (CommonUtils::getCharCount(_exp, '|') % 2) //If odd
				return { _exp, ErrorCodes::INVALID_ABS_POLES };

			const std::string _forbiddenChars = "*/^PCiygmubdlpr<=>,";

			if (_exp.length() == 1 || _forbiddenChars.find(_exp[i + 1]) != std::string::npos) //Must NOT be opened
				return { _exp, ErrorCodes::INVALID_EXPRESSION };

			_exp[i] = '[';

			size_t j = _exp.rfind('|');
			std::vector<size_t> _vAmbiguous;

			while ((i = _exp.find('|', i + 1)) != std::string::npos && i < j) {
				if (_exp[i - 1] == '[' || _forbiddenChars.find(_exp[i - 1]) != std::string::npos || std::string("+-").find(_exp[i - 1]) != std::string::npos || _funChars.find(_exp[i - 1]) != std::string::npos) { //Must NOT be closed
					if (_forbiddenChars.find(_exp[i + 1]) != std::string::npos) {//Must NOT be opened
						if (_exp[i + 1] == ',')
							return { "", ErrorCodes::INVALID_EXPRESSION };

						return { "", ErrorCodes::INVALID_OPERATOR_PLACEMENT };
					}

					_exp[i] = '[';
				}

				else if (_forbiddenChars.find(_exp[i + 1]) != std::string::npos) { //Must NOT be opened
					if (_exp[i - 1] == '[')
						return { "", ErrorCodes::INVALID_ABS_POLES };
					else
						_exp[i] = ']'; //We would have already checked "Must NOT be closed";
				}
				else { //Might be open or closed
					if (std::isdigit(_exp[i + 1]) || _exp[i + 1] == '.' || (_getNetSquareBracketsBefore(_exp, i) == 0 && CommonUtils::getCharCount(_exp.substr(0, i), '|') == 0)) //Must NOT be closed
						_exp[i] = '[';
					else //Ambiguous case: the pole is followed by a sign, and before it is a digit/decimal point. Brackets are not balanced before.
						_vAmbiguous.push_back(i);
				}

			}

			if (_exp[j - 1] == '[' || _forbiddenChars.find(_exp[j - 1]) != std::string::npos || std::string("+-").find(_exp[j - 1]) != std::string::npos || _funChars.find(_exp[j - 1]) != std::string::npos) //Must NOT be closed
				return { "", ErrorCodes::INVALID_EXPRESSION };

			_exp[j] = ']';

			if (!_vAmbiguous.empty()) {
				//A last resort before brute forcing the correct arrangement:
				i = CommonUtils::getCharCount(_exp, '[');
				j = CommonUtils::getCharCount(_exp, ']');

				if (i != j && std::max(i, j) - std::min(i, j) == _vAmbiguous.size())
					_fillAmbiguousPoles(_exp, _vAmbiguous, i > j ? ']' : '[');
				else
					//Brute forcing:
					return _bruteForceAmbiguousPoles(_exp, _vAmbiguous, 0);
			}

			return { _exp, ErrorCodes::NO_ERRORS };
		}

		static void _formatSquareBracketsToFuns(std::string & _exp) {
			size_t _openBracketPos;
			std::string _subString;

			while ((_openBracketPos = _exp.find('[')) != std::string::npos) {
				_subString = _exp.substr(_openBracketPos + 1, CommonUtils::findRespectiveBracketPos(_exp, _openBracketPos, BracketTypes::SQUARE) - _openBracketPos - 1);

				_exp.replace(_openBracketPos, _subString.length() + 2, std::string("}(") + _subString + ')');
			}
		}

		std::pair<std::string, ErrorCodes> _formatAbsoluteValues(std::string & _exp) {
			std::pair<std::string, ErrorCodes> _result = _formatAbsPoles(_exp);

			if (_result.second != ErrorCodes::NO_ERRORS)
				return _result;

			_formatSquareBracketsToFuns(_result.first);
			return _result;
		}

		void _replaceTokens_uppercase(std::string & _exp) {
			for (auto it = _operatorNames.begin(); it->first != 'i'; it++)
				_exp = _replaceAll(_exp, it->second, std::string(1, it->first));
		}

		//This only formats the most inner function
		std::string _formatImplicitBrackets_Funs(std::string _exp) {
			size_t _mostInnerFunPos = 0, _clonePos, _partialExpressionLength;
			std::string _partialExpression;

			while ((_mostInnerFunPos = _exp.find_first_of(_funChars, _mostInnerFunPos)) != std::string::npos) {
				do _mostInnerFunPos++;
				while (_funChars.find(_exp[_mostInnerFunPos]) != std::string::npos);

				_clonePos = _mostInnerFunPos--; //Don't replace with - 1

				if (_clonePos == _exp.length())
					return "";

				//The _clonePos is now on the chracter directly after the fun char
				if (_exp[_clonePos] == '(') { //If the inner function has its own open bracket
					if ((_partialExpressionLength = CommonUtils::findRespectiveBracketPos(_exp, _clonePos, BracketTypes::REGULAR)) == std::string::npos)
						return "";

					_partialExpression = _exp.substr(_clonePos + 1, _partialExpressionLength - _clonePos - 1);
					_partialExpressionLength = _partialExpression.length(); //This is the original length
					_partialExpression = _formatImplicitBrackets_Funs(_partialExpression);

					if (_partialExpression.empty())
						return "";

					_exp.replace(_clonePos + 1, _partialExpressionLength, _partialExpression);

					_mostInnerFunPos = CommonUtils::findRespectiveBracketPos(_exp, _clonePos, BracketTypes::REGULAR) + 1; //We need to re-find the respective position because _exp has been modified
				}
				else if (_exp[_mostInnerFunPos] == '?' || _exp[_mostInnerFunPos] == '\"') //Those require an open bracket anyways
					return "";
				else if (std::isdigit(_exp[_clonePos]) || std::string(".+-efzxE").find(_exp[_clonePos]) != std::string::npos) {
					_exp.insert(_exp.begin() + _clonePos, '(');
					_clonePos++; //The _clonePos is now on the character directly after the open bracket of the function

					if (_exp[_clonePos] == '+' || _exp[_clonePos] == '-')
						_clonePos++;

					while (_clonePos < _exp.length() && (std::isdigit(_exp[_clonePos]) || std::string(".efzxE^!%").find(_exp[_clonePos]) != std::string::npos)) {
						_clonePos++;

						if (_exp[_clonePos] == 'E' && std::string("+-").find(_exp[_clonePos + 1]) != std::string::npos)
							_clonePos += 2; //To skip the sign
					}

					_partialExpressionLength = _clonePos; //I used this variable as a dummy variable for later
					if (_exp[_clonePos] != '(')
						_clonePos++; //To make the _clonePos on the character after the last character, we check for power tower.

					//To include towers of expontents:
					if (_clonePos < _exp.length() && _exp[_clonePos - 1] == '^') {
						do {
							if (_exp[_clonePos] == '(' && _exp[_clonePos - 1] == '^') {
								if ((_clonePos = CommonUtils::findRespectiveBracketPos(_exp, _clonePos, BracketTypes::REGULAR)) == std::string::npos)
									return "";

								_clonePos += 2;
							}
							else if ((std::isdigit(_exp[_clonePos]) || std::string(".efzxE").find(_exp[_clonePos]) != std::string::npos) && _exp[_clonePos - 1] == '^') {
								while (std::isdigit(_exp[_clonePos]) || std::string(".efzxE").find(_exp[_clonePos]) != std::string::npos)
									_clonePos++;

								_clonePos++;
							}
							else {
								_clonePos--; //This is very important
								break;
							}
						} while (_clonePos < _exp.length() - 1);

						if (_clonePos >= _exp.length() - 1)
							_clonePos--; //This is very important
					}
					else if (_clonePos - 1 == _partialExpressionLength)
						_clonePos--; //To revert the effect of increasing it before this if statement

					_exp.insert(_exp.begin() + _clonePos, ')');
					_mostInnerFunPos = _clonePos + 1; //For the next iteration
				}
				else
					return "";
			}

			return _exp;
		}

		//This only formats nested functions
		std::pair<std::string, ErrorCodes> _formatNestedFuns(const std::string & _exp) {
			size_t _firstFunPos = _exp.find_first_of(_funChars);

			while (_firstFunPos < _exp.length() - 1 && _funChars.find(_exp[_firstFunPos + 1]) == std::string::npos)
				_firstFunPos = _exp.find_first_of(_funChars, _firstFunPos + 1);

			if (_firstFunPos != std::string::npos && _firstFunPos < _exp.length() - 1) {
				size_t _clonePos = _firstFunPos + 1;
				std::pair<std::string, ErrorCodes> _partialExpressionPair;

				if (std::isdigit(_exp[_clonePos]) || _funChars.find(_exp[_clonePos]) != std::string::npos || std::string(".+-(").find(_exp[_clonePos]) != std::string::npos) {
					//Extracting _partialExpression
					while (true) { //This will get the correct value of _clonePos to get the length of _partialExpression
						if (_funChars.find(_exp[_clonePos]) != std::string::npos)
							_clonePos++;

						else if (_exp[_clonePos] == '(') {
							_clonePos = CommonUtils::findRespectiveBracketPos(_exp, _clonePos, BracketTypes::REGULAR);

							if (_clonePos == std::string::npos)
								return { "", ErrorCodes::INVALID_BRACKETS };
							else
								break;
						}

						else if (std::isdigit(_exp[_clonePos]) || std::string(".+-").find(_exp[_clonePos]) != std::string::npos) {
							_clonePos += CommonUtils::extractRightValue(_exp, _clonePos - 1).length() - 1;
							break;
						}

						else
							return { "", ErrorCodes::INVALID_FUNCTION_CALL };
					}

					_partialExpressionPair.first = _exp.substr(_firstFunPos + 1, _clonePos - _firstFunPos);
					_clonePos = _partialExpressionPair.first.length(); //I needed to save the length of _partialExpression for later, so I used this variable instead of creating a new one and consume more memory :-)
					_partialExpressionPair = _formatNestedFuns(_partialExpressionPair.first);

					if (_partialExpressionPair.second != ErrorCodes::NO_ERRORS)
						return _partialExpressionPair;

					if (_funChars.find(_partialExpressionPair.first.front()) != std::string::npos)
						_partialExpressionPair.first = '(' + _partialExpressionPair.first + ')';

					std::string _result = _exp;
					_result.replace(_firstFunPos + 1, _clonePos, _partialExpressionPair.first);

					return _formatNestedFuns(_result);
				}
				else
					return { "", ErrorCodes::INVALID_FUNCTION_CALL };
			}
			else
				return { _exp, ErrorCodes::NO_ERRORS };
		}

		//To format the expression in (sum,prod)=(k,v) {x->w}
		std::pair<std::string, ErrorCodes> _formatExperssions_SumProd(const std::string & _exp) {
			std::pair<std::string, ErrorCodes> _resultPair = { _exp, ErrorCodes::NO_ERRORS };
			size_t _funPos = 0, _commaPos;

			while ((_funPos = _resultPair.first.find_first_of("kv", _funPos)) != std::string::npos) {
				if (_funPos == _resultPair.first.length() - 1 || _resultPair.first[_funPos + 1] != '(')
					return { "", ErrorCodes::INVALID_FUNCTION_CALL };

				_commaPos = CommonUtils::findFirstCommaPos(_resultPair.first, _funPos + 2);

				if (_commaPos == _resultPair.first.length() - 1)
					return { "", ErrorCodes::INVALID_FUNCTION_ARGUMENTS };

				//Now the _commaPos is on the correct/first comma in the function
				const std::string _partialExpression = _resultPair.first.substr(_funPos + 2, _commaPos - _funPos - 2);
				std::string _partialResult;

				for (size_t i = 0; i < _partialExpression.length(); i++) {
					if (_partialExpression[i] == 'x')
						_partialResult += 'w';
					else
						_partialResult += _partialExpression[i];
				}

				_resultPair.first.replace(_funPos + 2, _partialExpression.length(), _partialResult);

				_funPos++; //For the next iteration
			}

			return _resultPair;
		}

		std::pair<std::string, ErrorCodes> _formatFunctions(const std::string & _exp, const bool _implicitMultHighPrec) {
			std::pair<std::string, ErrorCodes> _resultPair = { _exp, ErrorCodes::NO_ERRORS };

			_resultPair.first = _formatImplicitBrackets_Funs(_resultPair.first); //This method supports early exit

			if (_resultPair.first.empty())
				return { "", ErrorCodes::INVALID_FUNCTION_CALL };

			_resultPair = _formatNestedFuns(_resultPair.first); //This method supports early exit

			if (_resultPair.second != ErrorCodes::NO_ERRORS)
				return _resultPair;

			return _formatExperssions_SumProd(_resultPair.first);
		}

		static std::string _replaceXs(const std::string & _exp) {
			std::string _result;

			for (const char C : _exp) {
				if (C == 'x')
					_result += 'w';
				else
					_result += C;
			}
				
			return _result;
		}

		std::pair<std::string, ErrorCodes> _formatConstants_ScientificNotation(std::string & _exp) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dist(0.0, 1.0);

			const double _pi = 3.14159265358979324;
			const double _e = 2.71828182845904524;
			std::string _rightVal;
			std::pair<std::string, ErrorCodes> _resultPair = { "", ErrorCodes::NO_ERRORS };

			if (_exp[0] == 'E')
				return { "", ErrorCodes::INVALID_SCIENTIFIC_NOTATION };

			for (size_t i = 0; i < _exp.length(); i++) {
				switch (_exp[i]) {
					case 'f': { //Pi
						_resultPair.first += CommonUtils::convertDoubleToStr(_pi, _precision, false);

						if (i < _exp.length() - 1) {
							if (std::isdigit(_exp[i + 1]) || _exp[i + 1] == '.')
								return { "", ErrorCodes::INVALID_IMPLICIT_MULTIPLICATION };

							if (_funChars.find(_exp[i + 1]) != std::string::npos) //Implicit multiplication with functions
								_resultPair.first += '*';
						}

						break;
					}
					case 'e': { //Euler's number/Scientific notation
						if (i < _exp.length() - 1 && (std::isdigit(_exp[i + 1]) || _exp[i + 1] == '.')) { //Scientific notation
							_rightVal = CommonUtils::extractRightValue(_exp, i);
							_resultPair.first += "10^" + _rightVal; //The multiplication sign is already inserted in _formatImplicitMultiplication

							i += _rightVal.length(); //The for loop will further increase the i by 1
						}
						else { //Euler's number
							_resultPair.first += CommonUtils::convertDoubleToStr(_e, _precision, false);

							if (i < _exp.length() - 1 && _funChars.find(_exp[i + 1]) != std::string::npos) //Implicit multiplication with functions
								_resultPair.first += '*';
						}

						break;
					}
					case 'E': { //Scientific notation
						if (!std::isdigit(_exp[i - 1]) && std::string(").efzw").find(_exp[i - 1]) == std::string::npos)
							return { "", ErrorCodes::INVALID_SCIENTIFIC_NOTATION };

						_resultPair.first += "*10^" + _rightVal; //The multiplication sign is already inserted in _formatImplicitMultiplication

						break;
					}
					case 'z': { //Random number
						_resultPair.first += CommonUtils::convertDoubleToStr(dist(gen), _precision, false);

						if (i < _exp.length() - 1) {
							if (std::isdigit(_exp[i + 1]) || _exp[i + 1] == '.')
								return { "", ErrorCodes::INVALID_IMPLICIT_MULTIPLICATION };

							if (_funChars.find(_exp[i + 1]) != std::string::npos) //Implicit multiplication with functions
								_resultPair.first += '*';
						}

						break;
					}
					default: {
						_resultPair.first += _exp[i];
						break;
					}
				}
			}

			return _resultPair;
		}

		std::pair<std::string, ErrorCodes> _formatUnaryMinus_PC(std::string & _exp) {
			size_t _operatorPos = _exp.find_first_of("PC"), _leftOperandStartPos;

			if (_operatorPos == std::string::npos)
				return { _exp, ErrorCodes::NO_ERRORS };

			if (_operatorPos == 0 || _operatorPos == _exp.length() - 1)
				return { "", ErrorCodes::INVALID_EXPRESSION };

			do {
				if (_exp[_operatorPos - 1] == ')') {
					if ((_leftOperandStartPos = CommonUtils::findRespectiveBracketPos(_exp, _operatorPos - 1, BracketTypes::REGULAR)) == std::string::npos)
						return { "", ErrorCodes::INVALID_BRACKETS };

					if (_leftOperandStartPos > 0) {
						if (_funChars.find(_exp[_leftOperandStartPos - 1]) != std::string::npos) //If it's a function call
							_leftOperandStartPos--;

						//Identifying unary minus before the left operand:
						if (_leftOperandStartPos > 0 && _exp[_leftOperandStartPos - 1] == '-' && (_leftOperandStartPos == 1 || !(std::isdigit(_exp[_leftOperandStartPos - 2]) || std::string(").!%").find(_exp[_leftOperandStartPos - 2]) != std::string::npos))) {
							_exp.insert(_exp.begin() + _leftOperandStartPos - 1, '(');
							_exp.insert(_exp.begin() + ++_operatorPos, ')');
						}
					}
				}

				_operatorPos += 2; //For the next iteration
			} while ((_operatorPos = _exp.find_first_of("PC", _operatorPos)) != std::string::npos);

			return { _exp, ErrorCodes::NO_ERRORS };
		}

		std::pair<std::string, ErrorCodes> _formatImplicitMultiplication(const std::string & _exp, const bool _implicitMultHighPrec) {
			std::string _result;

			if (_implicitMultHighPrec) { //Giving higher precedence:
				_result = _exp;
				size_t _implicitMultCharPos = 0, _clonePos, _partialExpressionPos, _partialExpressionLength;
				std::pair<std::string, ErrorCodes> _partialExpressionPair;

				while ((_implicitMultCharPos = _result.find_first_of("(efzw}ABDFGHJKLMNOQRSTUVWXYZ~@{$&_?}\"\\ qonkvh", _implicitMultCharPos)) != std::string::npos) {
					//Identifying implicit multiplication:
					if (_implicitMultCharPos > 0 && (std::isdigit(_result[_implicitMultCharPos - 1]) || std::string(").efzw").find(_result[_implicitMultCharPos - 1]) != std::string::npos || (_result[_implicitMultCharPos - 1] == '-' && !(std::isdigit(_exp[_implicitMultCharPos - 1]) || std::string(").!%").find(_exp[_implicitMultCharPos - 1]) != std::string::npos)))) {
						//Inserting the open bracket:
						_clonePos = _implicitMultCharPos;

						do {
							_clonePos--;

							if (_result[_clonePos] == ')') {
								size_t _temp = CommonUtils::findRespectiveBracketPos(_result, _clonePos, BracketTypes::REGULAR);

								if (_temp != std::string::npos)
									_clonePos = _temp;
								else
									return { "", ErrorCodes::INVALID_BRACKETS };
							}
							else if (!std::isdigit(_result[_clonePos]) && std::string(".^efzwE}!%").find(_result[_clonePos]) == std::string::npos && _funChars.find(_result[_clonePos]) == std::string::npos) //DON'T PUT P&C IN THE TEMP STRING!
								break;
						} while (_clonePos > 0);

						//The cherry on top:
						if (_clonePos == 0 || _result[_clonePos] != '/') {
							_implicitMultCharPos++;
							continue; //No need for extra brackets here
						}
						else
							_result.insert(_result.begin() + _clonePos + 1, '(');

						//Inserting the closed bracket:
						_clonePos = (++_implicitMultCharPos); //Because we inserted the open bracket above, _implicitMultCharPos shifted back by 1

						do {
							if (_result[_clonePos] == '(') {
								size_t _temp = CommonUtils::findRespectiveBracketPos(_result, _clonePos, BracketTypes::REGULAR);

								if (_temp != std::string::npos) {
									_partialExpressionPos = _clonePos + 1;
									_clonePos = _temp;

									_partialExpressionPair.first = _result.substr(_partialExpressionPos, _clonePos - _partialExpressionPos);
									_partialExpressionLength = _partialExpressionPair.first.length();
									_partialExpressionPair = _formatImplicitMultiplication(_partialExpressionPair.first, true);

									if (_partialExpressionPair.second != ErrorCodes::NO_ERRORS)
										return _partialExpressionPair;

									_result.replace(_partialExpressionPos, _clonePos - _partialExpressionPos, _partialExpressionPair.first); //To format inner expressions containing implicit multiplication
									_clonePos += 1 + _partialExpressionPair.first.length() - _partialExpressionLength; //To correctly move the cursor after insertion
								}
								else
									return { "", ErrorCodes::INVALID_BRACKETS };
							}
							else if (std::isdigit(_result[_clonePos]) || std::string(".^efzwE}!%").find(_result[_clonePos]) != std::string::npos || _funChars.find(_result[_clonePos]) != std::string::npos) //DON'T PUT P&C IN THE TEMP STRING!
								_clonePos++;
							else
								break;
						} while (_implicitMultCharPos < _result.length() - 1);

						// The _clonePos is now on the correct character
						_result.insert(_result.begin() + _clonePos, ')');

						_implicitMultCharPos = (_clonePos += 2); //To save a fraction of a second in the find method in the upper while loop, by skiping one iteration in it
					}
					else
						_implicitMultCharPos++;
				}

				return _formatImplicitMultiplication(_result, false);
			}
			else { //Giving normal precendence && multiplication signs
				for (size_t i = 0; i < _exp.length(); i++) {
					if (std::string("(efzw}").find(_exp[i]) == std::string::npos && _funChars.find(_exp[i]) == std::string::npos)
						_result += _exp[i];
					else if (i > 0 && (std::isdigit(_exp[i - 1]) || std::string(").efzw!%").find(_exp[i - 1]) != std::string::npos))
						_result += std::string(1, '*') + _exp[i];
					else
						_result += _exp[i];
				}

				return { _result, ErrorCodes::NO_ERRORS };
			}
		}

		static void _insertBracketsWithNegatives_PC(std::string & _exp) {
			size_t _operatorPos = 0, _clonePos;

			while ((_operatorPos = _exp.find_first_of("PC", _operatorPos)) != std::string::npos) {
				_clonePos = _operatorPos - 1;

				//Inserting the open bracket:
				while (_clonePos > 0 && (std::isdigit(_exp[_clonePos]) || _exp[_clonePos] == '.'))
					_clonePos--;

				if (_exp[_clonePos] == '-' && (_clonePos == 0 || !(std::isdigit(_exp[_clonePos - 1]) || std::string(").!%").find(_exp[_clonePos - 1]) != std::string::npos))) {
					_exp.insert(_exp.begin() + _clonePos, '(');
					_exp.insert(_exp.begin() + _operatorPos + 1, ')'); //We add 1 because of the open bracket inserted

					_operatorPos += 3; //Because we inserted 2 characters + shifting it one more character
				}
				else
					_operatorPos += 2; //To step after the operator found by 2 steps
			}
		}

		//Magic chars are inserted with functions in the evaluation process for convenience
		ErrorCodes _insertMagicCharacters_FreeBrackets(std::string & _exp) {
			size_t _nextOperatorPos = 0;
			size_t _openBracketPos = 0;

			while ((_nextOperatorPos = _exp.find_first_of("^PC!%", _nextOperatorPos)) != std::string::npos) {
				if (_exp[_nextOperatorPos - 1] == ')' && _exp.find('(') != std::string::npos) {
					if ((_openBracketPos = CommonUtils::findRespectiveBracketPos(_exp, _nextOperatorPos - 1, BracketTypes::REGULAR)) == std::string::npos)
						return ErrorCodes::INVALID_BRACKETS;

					if (_openBracketPos == 0 || (_funChars.find(_exp[_openBracketPos - 1]) == std::string::npos && std::string("PC").find(_exp[_openBracketPos - 1]) == std::string::npos)) {
						_exp.insert(_exp.begin() + _openBracketPos, CommonUtils::getMagicCharacter(_exp[_nextOperatorPos]));

						_nextOperatorPos += 2;
					}
					else
						_nextOperatorPos++;
				}
				else
					_nextOperatorPos++;
			}

			return ErrorCodes::NO_ERRORS;
		}

	public:
		ExpressionFormatter(const std::unordered_map<char, std::string> & operatorNames, const short precision) : _operatorNames(operatorNames) { //To intialize the reference type
			_precision = precision;
		}

		//This method is used for internal use only
		std::pair<std::string, ErrorCodes> formatExpression(const std::string & exp, const bool implicitMultHighPrec, const bool allowX = false) {
			if (exp == "0")
				return { "0", ErrorCodes::NO_ERRORS };

			std::pair<std::string, ErrorCodes> _resultPair = { exp, ErrorCodes::NO_ERRORS };

			_resultPair.first = _removeSpaces(_resultPair.first);

			_replaceTokens_lowercase(_resultPair.first);
			_replaceTokens_uppercase(_resultPair.first);

			_resultPair.first = _removeSpaces(_resultPair.first);

			_resultPair.first = _collapseAddSubSigns(_resultPair.first);

			_resultPair.first = _formatDecimalPoints(_resultPair.first); //This method supports early exit

			if (_resultPair.first.empty())
				return { "", ErrorCodes::INVALID_DECIMAL_POINTS };

			_resultPair.first = _replaceAll(_resultPair.first, "pi", "f");
			_resultPair.first = _replaceAll(_resultPair.first, "rnd#", "z");

			_resultPair = _formatAbsoluteValues(_resultPair.first); //This method supports early exit

			if (_resultPair.first.empty())
				return { "", ErrorCodes::INVALID_ABS_POLES };

			_resultPair = _formatFunctions(_resultPair.first, implicitMultHighPrec); //This method supports early exit

			if (_resultPair.second != ErrorCodes::NO_ERRORS)
				return _resultPair;

			if (allowX)
				_resultPair.first = _replaceXs(_resultPair.first);

			//Formatting unary minus with brackets with PC when implicitMultHighPrec is false:
			_resultPair = _formatUnaryMinus_PC(_resultPair.first); //This method supports early exit

			if (_resultPair.second != ErrorCodes::NO_ERRORS)
				return _resultPair;

			_resultPair = _formatImplicitMultiplication(_resultPair.first, implicitMultHighPrec); //This method supports early exit

			if (_resultPair.second != ErrorCodes::NO_ERRORS)
				return _resultPair;

			if (_resultPair.first.find_first_of("efzE") != std::string::npos) {
				_resultPair = _formatConstants_ScientificNotation(_resultPair.first); //This method supports early exit

				if (_resultPair.second != ErrorCodes::NO_ERRORS)
					return _resultPair;
			}

			//Formatting loose negative numbers with P & C:
			_insertBracketsWithNegatives_PC(_resultPair.first);

			//Inserting magic characters with "^PC!%" with brackets
			//(^ -> `)   (P ->  ;)   (C -> :)   (! -> ')   (% -> j)
			return { _resultPair.first, _insertMagicCharacters_FreeBrackets(_resultPair.first) }; //This method supports early exit
		}
	};

	class ExpressionValidator {
	private:
		static size_t getCharCount(const std::string & _str, const char _chr) {
			size_t _counter = 0;

			for (const char C : _str)
				if (C == _chr)
					_counter++;

			return _counter;
		}

		static bool _validateCharacters(const std::string & _exp) {
			const std::string _funChars = "ABDFGHJKLMNOQRSTUVWXYZ~@{$&_?}\"\\ qonkvh";

			//Validating all characters:
			std::string _allowedChars = "`;:\'j.()+-*/^PC,!%w<=>iygmubdlpr"; //These are the non-digit characters allowed to be in an expression (excluding _funChars)
			for (const char C : _exp)
				if (!std::isdigit(C) && _allowedChars.find(C) == std::string::npos && _funChars.find(C) == std::string::npos)
					return false;

			//Validating first character:
			_allowedChars = ".(+-`;:\'jw"; //These are the non-digit characters allowed to be as the first character in an expression (excluding _funChars)
			if (!std::isdigit(_exp.front()) && _allowedChars.find(_exp.front()) == std::string::npos && _funChars.find(_exp.front()) == std::string::npos)
				return false;

			//Validating last character:
			_allowedChars = ".)!%w"; //These are the non-digit characters allowed to be as the last character in an expression
			if (!std::isdigit(_exp.back()) && _allowedChars.find(_exp.back()) == std::string::npos)
				return false;

			return true;
		}

		static bool _validateFactorialPercentSign(const std::string & _exp) {
			size_t _signPos = 0;

			while ((_signPos = _exp.find_first_of("!%", _signPos)) != std::string::npos) {
				if (_signPos < _exp.length() - 1 && (std::isdigit(_exp[_signPos + 1]) || _exp[_signPos + 1] == '.'))
					return false;

				_signPos++;
			}

			return true;
		}

		static bool _validateMagicChars(const std::string & _exp) {
			size_t _magicCharPos = 0;

			while ((_magicCharPos = _exp.find_first_of("`;:\'j", _magicCharPos)) != std::string::npos) {
				if (_magicCharPos == _exp.length() - 1 || _exp[_magicCharPos + 1] != '(')
					return false;

				_magicCharPos++;
			}

			return true;
		}

		static bool _validateDecimalPoints(const std::string & _exp) {
			size_t _pointPos = 0, _clonePos;

			while ((_clonePos = _pointPos = _exp.find('.', _pointPos)) != std::string::npos) {
				for (_clonePos; std::isdigit(_exp[_clonePos]) || _exp[_clonePos] == '.'; _clonePos++)
					if (std::isdigit(_exp[_clonePos]) && _exp[_clonePos + 1] == '.')
						return false;

				_pointPos++;
			}

			return true;
		}

		static ErrorCodes _validateMultiArgumentFuns(std::string & _partialExpression, const char _outerFunChar = ' ') {
			size_t _firstFunPos = _partialExpression.find_first_of("{?\"kvh");
			size_t _respectiveBracketPos;

			if (_firstFunPos == std::string::npos) {
				if (_outerFunChar == ' ') {
					if (getCharCount(_partialExpression, ',') > 0)
						return ErrorCodes::INVALID_EXPRESSION;
					else
						return ErrorCodes::NO_ERRORS;
				}

				if (_partialExpression.front() == ',' || _partialExpression.back() == ',')
					return ErrorCodes::INVALID_FUNCTION_ARGUMENTS;

				if (_outerFunChar == '{') { //log -- Can have either one comma or no commas
					if (getCharCount(_partialExpression, ',') > 1)
						return ErrorCodes::INVALID_FUNCTION_ARGUMENTS;
				}
				else if (_outerFunChar == '?' || _outerFunChar == '\"' || _outerFunChar == 'h') { //Must have ONE comma
					if (getCharCount(_partialExpression, ',') != 1)
						return ErrorCodes::INVALID_FUNCTION_ARGUMENTS;
				}
				else { //Must have TWO commas
					if (getCharCount(_partialExpression, ',') != 2)
						return ErrorCodes::INVALID_FUNCTION_ARGUMENTS;
				}

				_partialExpression = "0";
				return ErrorCodes::NO_ERRORS;
			}
			else if (_firstFunPos == _partialExpression.length() - 1)
				return ErrorCodes::INVALID_FUNCTION_CALL;
			else {
				if (_partialExpression[_firstFunPos + 1] != '(')
					return ErrorCodes::INVALID_FUNCTION_CALL;

				if ((_respectiveBracketPos = CommonUtils::findRespectiveBracketPos(_partialExpression, _firstFunPos + 1, BracketTypes::REGULAR)) == std::string::npos)
					return ErrorCodes::INVALID_BRACKETS;

				std::string _subPartialExpression = _partialExpression.substr(_firstFunPos + 2, _respectiveBracketPos - _firstFunPos - 2);
				size_t _originalSubPartialExpressionLength = _subPartialExpression.length();

				ErrorCodes _subResult = _validateMultiArgumentFuns(_subPartialExpression, _partialExpression[_firstFunPos]);
				if (_subResult != ErrorCodes::NO_ERRORS)
					return _subResult;

				_partialExpression.replace(_firstFunPos, _originalSubPartialExpressionLength + 3, _subPartialExpression);

				if (_partialExpression == "0,0" || _partialExpression == "0,0,0") {
					_partialExpression = "0";
					return ErrorCodes::NO_ERRORS;
				}

				_subResult = _validateMultiArgumentFuns(_partialExpression, _outerFunChar);
				if (_subResult != ErrorCodes::NO_ERRORS)
					return _subResult;

				return ErrorCodes::NO_ERRORS;
			}
		}

		static ErrorCodes _validateSequentialChars(const std::string & _exp) {
			std::string _allowedChars = "()+-!%"; //These are the non-digit characters allowed to repeat sequentially
			std::string _twoOperandOperators = "*/^PCiygmubdlpr<=>"; //Basic operators + logical operators

			for (size_t i = 0; i < _exp.length() - 1; i++) {
				if (_exp[i] == _exp[i + 1]) {
					if (!std::isdigit(_exp[i]) && _allowedChars.find(_exp[i]) == std::string::npos) {
						if (_exp[i] == '.')
							return ErrorCodes::INVALID_DECIMAL_POINTS;
						else if (_exp[i] == ',')
							return ErrorCodes::INVALID_EXPRESSION;
						else
							return ErrorCodes::INVALID_OPERATOR_PLACEMENT;
					}
				}
				else {
					if (_twoOperandOperators.find(_exp[i]) != std::string::npos) {
						if (_twoOperandOperators.find(_exp[i + 1]) != std::string::npos || _exp[i + 1] == '!' || _exp[i + 1] == '%')
							return ErrorCodes::INVALID_OPERATOR_PLACEMENT;
					}
					else if ((_exp[i] == '+' || _exp[i] == '-') && (_twoOperandOperators.find(_exp[i + 1]) != std::string::npos || _exp[i + 1] == '!' || _exp[i + 1] == '%')) {
						return ErrorCodes::INVALID_OPERATOR_PLACEMENT;
					}
				}
			}

			return ErrorCodes::NO_ERRORS;
		}

	public:
		static ErrorCodes validateExpression(const std::string & _exp) {
			if (_exp == "0")
				return ErrorCodes::NO_ERRORS;

			if (!_validateCharacters(_exp))
				return ErrorCodes::INVALID_EXPRESSION;

			if (!_validateFactorialPercentSign(_exp))
				return ErrorCodes::INVALID_IMPLICIT_MULTIPLICATION;

			if (!_validateMagicChars(_exp))
				return ErrorCodes::INVALID_EXPRESSION;

			if (!_validateDecimalPoints(_exp))
				return ErrorCodes::INVALID_DECIMAL_POINTS;

			ErrorCodes _errCode = CommonUtils::validateBrackets(_exp, BracketTypes::REGULAR);

			if (_errCode != ErrorCodes::NO_ERRORS)
				return _errCode;

			std::string _temp = _exp;
			_errCode = _validateMultiArgumentFuns(_temp);

			if (_errCode != ErrorCodes::NO_ERRORS) //This is recursion-based -- It modifies the parameter
				return _errCode;

			_errCode = _validateSequentialChars(_exp);

			if (_errCode != ErrorCodes::NO_ERRORS)
				return _errCode;

			return ErrorCodes::NO_ERRORS;
		}
	};

	class ExpressionEvaluator {
	private:
		enum ReplacementTypes { OPERATION, BRACKET };
		short _precision;
		double _pi;
		ErrorCodes _evaluationResult;
		AngleUnits _angleUnit;
		bool _forceBitwiseLogic;
		std::string _fullExpression;
		std::vector<std::pair<std::string, bool>> _evaluationSteps; //This will store the actual string step along with a flag to whether to keep it or not after finishing
		std::stack<size_t> _overallOperatorPositions; //Stacks enforce nesting naturally
		ReplacementTypes _overallReplacementType = ReplacementTypes::OPERATION; //This must be static

		static bool _isNumericalValue(const std::string & str) {
			size_t i = 0;

			if (str.front() == '-' || str.front() == '+')
				i++;

			for (i; i < str.length(); i++)
				if (!std::isdigit(str[i]) && str[i] != '.')
					return false;

			return true;
		}

		static size_t _findCorrectExponentPos(const std::string & _partialExpression) {
			size_t _firstPos;

			if ((_firstPos = _partialExpression.find('^')) != std::string::npos) {
				size_t _clonePos = _firstPos;

				do {
					_clonePos++;

					while (std::isdigit(_partialExpression[_clonePos]) || std::string("+-.`;:\'j").find(_partialExpression[_clonePos]) != std::string::npos)
						_clonePos++;

					if (_partialExpression[_clonePos] == '^')
						_firstPos = _clonePos;
					else
						break;
				} while (true);
			}

			return _firstPos;
		}

		//With brackets -- This avoids the need to collapse signs after insertion
		//This doesn't deal with magic chars when normal evaluatiot. But when recording steps, we must handle them externally
		void _insertSubPartialResult(std::string & _partialExpression, const size_t _openBracketPos, const size_t _subPartialExpLength, const double & _subPartialResult) {
			if (_openBracketPos > 0) { //If the expression doesn't start with the open bracket
				if (_partialExpression[_openBracketPos - 1] == '+') //If the character before the open bracket is a positive sign
					_partialExpression.replace(_openBracketPos - 1, _subPartialExpLength + 3, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
				else if (_partialExpression[_openBracketPos - 1] == '-') { //If the character before the open bracket is a negative sign
					if (_subPartialResult >= 0) //If the result is positive or zero
						_partialExpression.replace(_openBracketPos, _subPartialExpLength + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision, false));
					else //If the result is negative
						_partialExpression.replace(_openBracketPos - 1, _subPartialExpLength + 3, CommonUtils::convertDoubleToStr(-_subPartialResult, _precision));
				}
				else //For any other character types before the open bracket
					_partialExpression.replace(_openBracketPos, _subPartialExpLength + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
			}
			else //If the expression starts with the open bracket
				_partialExpression.replace(0, _subPartialExpLength + 2, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
		}

		//Without brackets -- This avoids the need to collapse signs after insertion
		void _insertSubPartialResult(std::string & _partialExpression, const size_t _operationPos, const std::string & _num1, const std::string & _num2, const double _subPartialResult) {
			if (_operationPos > _num1.length()) { //If the expression doesn't start with _num1
				if (std::string("`;:\'j").find(_partialExpression[_operationPos - _num1.length() - 1]) != std::string::npos) { //If there's a magic character directly before _num1
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
					if (_num1.front() == '+' || _num1.front() == '-') { //If _num1 was extracted with the sign
						if (std::string("*/+-").find(_partialExpression[_operationPos]) != std::string::npos) //If the operation is +-*/
							_partialExpression.replace(_operationPos - _num1.length(), _num1.length() + _num2.length() + 1, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
						else { //If the operation is other than +-*/
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
					else { //If _num1 was extracted without the sign: ^PC with no magic chars or another reason. The result will always be positive here.
						if (_partialExpression[_operationPos - _num1.length() - 1] == '+' || _partialExpression[_operationPos - _num1.length() - 1] == '-') //If there's +- before _num1 -- Keep in mind that the result will always be positive here.
							_partialExpression.replace(_operationPos - _num1.length(), _num1.length() + _num2.length() + 1, CommonUtils::convertDoubleToStr(_subPartialResult, _precision, false));
						else //For any other character before _num1
							_partialExpression.replace(_operationPos - _num1.length(), _num1.length() + _num2.length() + 1, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
					}
				}
			}
			else //If the expression starts with _num1
				_partialExpression.replace(0, _num1.length() + _num2.length() + 1, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
		}

		//With function calls with evaluation steps -- This avoids the need to collapse signs after insertion
		void _insertSubPartialResult_funs(std::string & _partialExpression, const size_t _openBracketPos, const size_t _subPartialExpLength, const double _subPartialResult) {
			if (_openBracketPos > 1) { //If _partialExpression doesn't start with the function call
				if (_partialExpression[_openBracketPos - 2] == '+') //We will also replace the positive sign
					_partialExpression.replace(_openBracketPos - 2, _subPartialExpLength + 4, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
				else if (_partialExpression[_openBracketPos - 2] == '-') {
					if (_subPartialResult >= 0) //We will keep the negative sign, and insert the result without the positive sign
						_partialExpression.replace(_openBracketPos - 1, _subPartialExpLength + 3, CommonUtils::convertDoubleToStr(_subPartialResult, _precision, false));
					else //We will replace the negative sign, and insert the result with a positive sign
						_partialExpression.replace(_openBracketPos - 2, _subPartialExpLength + 4, CommonUtils::convertDoubleToStr(-_subPartialResult, _precision));
				}
				else //For any other character before the function call
					_partialExpression.replace(_openBracketPos - 1, _subPartialExpLength + 3, CommonUtils::convertDoubleToStr(_subPartialResult, _precision));
			}
			else //If _partialExpression starts with the function call
				_partialExpression.replace(0, _subPartialExpLength + 3, CommonUtils::convertDoubleToStr(_subPartialResult, _precision)); //We must keep the sign to be consistent with _evaluationSteps
		}

		static bool _isWhole(const double d) {
			return std::floor(d) == d;
		}

		double _convertToRadian(const double _angle) {
			switch (_angleUnit) {
				case AngleUnits::RADIAN: return _angle;
				case AngleUnits::DEGREE: return _angle * _pi / 180.0;
				case AngleUnits::GRADIAN: return _angle * _pi / 200.0;
					return 0;
			}
		}

		double _convertFromRadian(const double _angle) {
			switch (_angleUnit) {
				case AngleUnits::RADIAN: return _angle;
				case AngleUnits::DEGREE: return _angle * 180.0 / _pi;
				case AngleUnits::GRADIAN: return _angle * 200.0 / _pi;
				default: return 0;
			}
		}

		static double _factorial(double n) {
			double _result = 1;

			for (n; n > 1; n--)
				_result *= n;

			return _result;
		}

		static double _permutations(const double _num1, const double _num2) {
			double result = 1;

			for (int i = 0; i < _num2; i++)
				result *= (_num1 - i);

			return result;
		}

		static double _combinations(const double _num1, const double _num2) {
			double result = 1;

			for (int i = 1; i <= _num2; i++) {
				result *= (_num1 - _num2 + i);
				result /= i;
			}

			return result;
		}

		//0 -> 0.5pi    1 -> 1.0pi
		bool _inDomain_trigFuns(double _val, const bool _b) {
			while (_val >= 2 * _pi)
				_val -= 2 * _pi;

			while (_val < 0)
				_val += 2 * _pi;

			double _tenPower = std::pow(10, _precision);
			return _b ? (_val > 0 && static_cast<size_t>(_val * _tenPower) != static_cast<size_t>(_pi * _tenPower)) : (static_cast<size_t>(_val * _tenPower) != static_cast<size_t>((_pi / 2) * _tenPower) && static_cast<size_t>(_val * _tenPower) != static_cast<size_t>((3 * _pi / 2) * _tenPower));
		}

		double _sum(const std::string & _exp, double & _firstLimit, const double _secondLimit) {
			double _result = 0;
			std::string _intermediateExp;

			for (_firstLimit; _firstLimit <= _secondLimit; _firstLimit++) {
				_intermediateExp = CommonUtils::plugInVal(_exp, _firstLimit, _precision);
				_result += _evaluatePartial(_intermediateExp, false);

				if (_evaluationResult != ErrorCodes::NO_ERRORS)
					return 0;
			}

			return _result;
		}

		double _prod(const std::string & _exp, double & _firstLimit, const double _secondLimit) {
			double _result = 1;
			std::string _intermediateExp;

			for (_firstLimit; _firstLimit <= _secondLimit; _firstLimit++) {
				_intermediateExp = CommonUtils::plugInVal(_exp, _firstLimit, _precision);
				_result *= _evaluatePartial(_intermediateExp, false);

				if (_evaluationResult != ErrorCodes::NO_ERRORS)
					return 0;
			}

			return _result;
		}

		bool _isPrime(const size_t _n) {
			const unsigned short divisionLimit = std::sqrt(_n);

			for (unsigned short i = 2; i <= divisionLimit; i++)
				if (_n % i == 0)
					return false;

			return true;
		}

		//For one-argument functions:
		double _calculate_OneArgument(const double _val, const char _funChar) {
			if (_evaluationResult != ErrorCodes::NO_ERRORS)
				return 0;

			double _result = 0;

			switch (_funChar) {
				case 'A': { //asinh
					_result = std::asinh(_val);
					break;
				}
				case 'B': { //acosh
					if (_val < 1) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ACOSH;
						return 0;
					}

					_result = std::acosh(_val);
					break;
				}
				case 'D': { //atanh
					if (_val <= -1 || _val >= 1) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ATANH;
						return 0;
					}

					_result = std::atanh(_val);
					break;
				}
				case 'F': { //acsch
					if (_val == 0) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ACSCH;
						return 0;
					}

					_result = std::asinh(1 / _val);
					break;
				}
				case 'G': { //asech
					if (_val > 1 || _val <= 0) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ASECH;
						return 0;
					}

					_result = std::acosh(1 / _val);
					break;
				}
				case 'H': { //acoth
					if (_val >= -1 && _val <= 1) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ACOTH;
						return 0;
					}

					_result = std::atanh(1 / _val);
					break;
				}
				case 'J': { //sinh
					_result = std::sinh(_val);
					break;
				}
				case 'K': { //cosh
					_result = std::cosh(_val);
					break;
				}
				case 'L': { //tanh
					_result = std::tanh(_val);
					break;
				}
				case 'M': { //csch
					if (_val == 0) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_CSCH;
						return 0;
					}

					_result = 1 / std::sinh(_val);
					break;
				}
				case 'N': { //sech
					_result = 1 / std::cosh(_val);
					break;
				}
				case 'O': { //coth
					if (_val == 0) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_COTH;
						return 0;
					}

					_result = 1 / std::tanh(_val);
					break;
				}
				case 'Q': { //asin
					if (_val < -1 || _val > 1) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ASIN;
						return 0;
					}

					_result = _convertFromRadian(std::asin(_val));
					break;
				}
				case 'R': { //acos
					if (_val < -1 || _val > 1) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ACOS;
						return 0;
					}

					_result = _convertFromRadian(std::acos(_val));
					break;
				}
				case 'S': { //atan
					_result = _convertFromRadian(std::atan(_val));
					break;
				}
				case 'T': { //acsc
					if (_val > -1 && _val < 1) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ACSC;
						return 0;
					}

					_result = _convertFromRadian(std::asin(1 / _val));
					break;
				}
				case 'U': { //asec
					if (_val > -1 && _val < 1) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ASEC;
						return 0;
					}

					_result = _convertFromRadian(std::acos(1 / _val));
					break;
				}
				case 'V': { //acot
					if (_val == 0) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_ACOT;
						return 0;
					}

					_result = _convertFromRadian(std::atan(1 / _val));
					break;
				}
				case 'W': { //sin
					_result = std::sin(_val);
					break;
				}
				case 'X': { //cos
					_result = std::cos(_val);
					break;
				}
				case 'Y': { //tan
					if (!_inDomain_trigFuns(_val, 0)) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_TAN;
						return 0;
					}

					_result = std::tan(_val);
					break;
				}
				case 'Z': { //csc
					if (!_inDomain_trigFuns(_val, 1)) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_CSC;
						return 0;
					}

					_result = 1 / std::sin(_val);
					break;
				}
				case '~': { //sec
					if (!_inDomain_trigFuns(_val, 0)) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_SEC;
						return 0;
					}

					_result = 1 / std::cos(_val);
					break;
				}
				case '@': { //cot
					if (!_inDomain_trigFuns(_val, 1)) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_COT;
						return 0;
					}

					_result = 1 / std::tan(_val);
					break;
				}
				case '{': { //log10
					if (_val <= 0) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_LOG10;
						return 0;
					}

					_result = std::log10(_val);
					break;
				}
				case '$': { //ln
					if (_val <= 0) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_LN;
						return 0;
					}

					_result = std::log(_val);
					break;
				}
				case '&': { //sqrt
					if (_val < 0) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_SQRT;
						return 0;
					}

					_result = std::sqrt(_val);
					break;
				}
				case '_': { //cbrt
					_result = std::cbrt(_val);
					break;
				}
				case '}': { //abs
					_result = std::fabs(_val);
					break;
				}
				case '\\': { //ispr
					if (!_isWhole(_val) || _val < 2)
						return 0;

					return _isPrime(static_cast<size_t>(_val));
				}
				case ' ': { //issqr
					if (_val < 0)
						return 0;

					return _isWhole(std::sqrt(static_cast<size_t>(_val)));
				}
				case 'q': { //NOT
					if (_forceBitwiseLogic) {
						if (_isWhole(_val))
							return ~static_cast<long long>(_val);

						_evaluationResult = ErrorCodes::INVALID_BITWISE_NOT_OPERAND;
						return 0;
					}

					return _val == 0;
				}
				case 'o': { //isodd
					if (_isWhole(_val))
						return static_cast<long long>(_val) % 2;

					return 0;
				}
				case 'n': { //iseven
					if (_isWhole(_val))
						return !(static_cast<long long>(_val) % 2);

					return 0;
				}
				case '!': { //factorial
					if (_val < 0 || !_isWhole(_val)) {
						_evaluationResult = ErrorCodes::INVALID_FACTORIAL_INPUT;
						return 0;
					}

					_result = _factorial(_val);
					break;
				}
				case '%': { //percent
					_result = _val * 0.01;
					break;
				}
			}

			if (_result == INFINITY || _result == -INFINITY) {
				_evaluationResult = ErrorCodes::EVALUATION_OUT_OF_RANGE;
				return 0;
			}

			return _result == 0 ? 0 : _result; //To normalize -0.0
		}

		//For two-argument functions + Other Operators
		double _calculate_TwoArguments(double _val1, double _val2, const char _funChar) {
			if (_evaluationResult != ErrorCodes::NO_ERRORS)
				return 0;

			double _result = 0;

			switch (_funChar) {
				case '+': {
					_result = _val1 + _val2;
					break;
				}
				case '-': {
					_result = _val1 - _val2;
					break;
				}
				case '*': {
					_result = _val1 * _val2;
					break;
				}
				case '/': {
					if (_val2 == 0) {
						_evaluationResult = ErrorCodes::CANNOT_DIVIDE_BY_ZERO;
						return 0;
					}

					_result = _val1 / _val2;
					break;
				}
				case '^': {
					if (_val1 == 0 && _val2 == 0) {
						_evaluationResult = ErrorCodes::INVALID_EXPONENTIATION;
						return 0;
					}

					_result = pow(_val1, _val2);
					break;
				}
				case 'P': {
					if (_val1 < 0 || _val2 < 0 || !_isWhole(_val1) || !_isWhole(_val2) || _val2 > _val1) {
						_evaluationResult = ErrorCodes::INVALID_PERMUTATIONS_OPERANDS;
						return 0;
					}

					_result = _permutations(_val1, _val2);
					break;
				}
				case 'C': {
					if (_val1 < 0 || _val2 < 0 || !_isWhole(_val1) || !_isWhole(_val2) || _val2 > _val1) {
						_evaluationResult = ErrorCodes::INVALID_COMBINATIONS_OPERANDS;
						return 0;
					}

					_result = _combinations(_val1, _val2);
					break;
				}
				case '{': { //log
					if (_val2 <= 0 || _val1 <= 0 || _val1 == 1) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_LOG;
						return 0;
					}

					_result = std::log(_val2) / std::log(_val1);
					break;
				}
				case '?': { //nthrt
					if (_val2 == 0 || (_isWhole(_val2) && _val1 < 0 && static_cast<long>(_val2) % 2 == 0)) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_NTHRT;
						return 0;
					}

					_result = std::pow(_val1, 1.0 / _val2);
					break;
				}
				case '\"': { //rndInt
					if (_val1 > _val2)
						std::swap(_val1, _val2);

					std::random_device rd;
					std::mt19937 gen(rd());

					std::uniform_int_distribution<long> dist(static_cast<long>(_val1), static_cast<long>(_val2));
					_result = dist(gen);
					break;
				}
				case 'h': { //mod
					if (!(_isWhole(_val1) && _isWhole(_val2))) {
						_evaluationResult = ErrorCodes::OUT_OF_DOMAIN_MOD;
						return 0;
					}

					_result = static_cast<long long>(_val1) % static_cast<long long>(_val2);
					break;
				}
				case '<': return _val1 < _val2;
				case '>': return _val1 > _val2;
				case '=': return _val1 == _val2;
				case 'g': return _val1 <= _val2; //<=
				case 'm': return _val1 >= _val2; //>=
				case 'i': { //<<
					if (_isWhole(_val1) && _isWhole(_val2))
						return static_cast<long long>(_val1) << static_cast<long long>(_val2);

					_evaluationResult = ErrorCodes::INVALID_BITWISE_LEFT_SHIFT_OPERANDS;
					return 0;
				}
				case 'y': { //>>
					if (_isWhole(_val1) && _isWhole(_val2))
						return static_cast<long long>(_val1) >> static_cast<long long>(_val2);

					_evaluationResult = ErrorCodes::INVALID_BITWISE_RIGHT_SHIFT_OPERANDS;
					return 0;
				}
				case 'u': { //XNOR
					if (_forceBitwiseLogic) {
						if (_isWhole(_val1) && _isWhole(_val2))
							return ~(static_cast<long long>(_val1) ^ static_cast<long long>(_val2));

						_evaluationResult = ErrorCodes::INVALID_BITWISE_XNOR_OPERANDS;
						return 0;
					}

					return static_cast<bool>(_val1) == static_cast<bool>(_val2);
				}
				case 'b': { //NAND
					if (_forceBitwiseLogic) {
						if (_isWhole(_val1) && _isWhole(_val2))
							return ~(static_cast<long long>(_val1) & static_cast<long long>(_val2));

						_evaluationResult = ErrorCodes::INVALID_BITWISE_NAND_OPERANDS;
						return 0;
					}

					return !(static_cast<bool>(_val1) && static_cast<bool>(_val2));
				}
				case 'd': { //NOR
					if (_forceBitwiseLogic) {
						if (_isWhole(_val1) && _isWhole(_val2))
							return ~(static_cast<long long>(_val1) | static_cast<long long>(_val2));

						_evaluationResult = ErrorCodes::INVALID_BITWISE_NOR_OPERANDS;
						return 0;
					}

					return !(static_cast<bool>(_val1) || static_cast<bool>(_val2));
				}
				case 'l': { //XOR
					if (_forceBitwiseLogic) {
						if (_isWhole(_val1) && _isWhole(_val2))
							return static_cast<long long>(_val1) ^ static_cast<long long>(_val2);

						_evaluationResult = ErrorCodes::INVALID_BITWISE_XOR_OPERANDS;
						return 0;
					}

					return static_cast<bool>(_val1) != static_cast<bool>(_val2);
				}
				case 'p': { //AND
					if (_forceBitwiseLogic) {
						if (_isWhole(_val1) && _isWhole(_val2))
							return static_cast<long long>(_val1) & static_cast<long long>(_val2);

						_evaluationResult = ErrorCodes::INVALID_BITWISE_AND_OPERANDS;
						return 0;
					}

					return static_cast<bool>(_val1) && static_cast<bool>(_val2);
				}
				case 'r': { //OR
					if (_forceBitwiseLogic) {
						if (_isWhole(_val1) && _isWhole(_val2))
							return static_cast<long long>(_val1) | static_cast<long long>(_val2);

						_evaluationResult = ErrorCodes::INVALID_BITWISE_OR_OPERANDS;
						return 0;
					}

					return static_cast<bool>(_val1) || static_cast<bool>(_val2);
				}
			}

			if (_result == INFINITY || _result == -INFINITY) {
				_evaluationResult = ErrorCodes::EVALUATION_OUT_OF_RANGE;
				return 0;
			}
			else if (isnan(_result)) {
				_evaluationResult = ErrorCodes::INVALID_EXPONENTIATION;
				return 0;
			}

			return _result == 0 ? 0 : _result; //To normalize -0.0
		}

		//For three-argument functions:
		double _calculate_ThreeArguments(const std::string & _exp, double & _firstLimit, const double _secondLimit, const char _funChar) {
			double _result = 0;

			switch (_funChar) {
				case 'k': { //sum
					if (!_isWhole(_firstLimit) || !_isWhole(_secondLimit) || _firstLimit > _secondLimit) {
						_evaluationResult = ErrorCodes::INVALID_SUM_LIMITS;
						return 0;
					}

					_result = _sum(_exp, _firstLimit, _secondLimit);
					break;
				}
				case 'v': { //prod
					if (!_isWhole(_firstLimit) || !_isWhole(_secondLimit) || _firstLimit > _secondLimit) {
						_evaluationResult = ErrorCodes::INVALID_PROD_LIMITS;
						return 0;
					}

					_result = _prod(_exp, _firstLimit, _secondLimit);
					break;
				}
			}

			if (_evaluationResult != ErrorCodes::NO_ERRORS)
				return 0;

			if (_result == INFINITY || _result == -INFINITY) {
				_evaluationResult = ErrorCodes::EVALUATION_OUT_OF_RANGE;
				return 0;
			}

			return _result;
		}

		static bool _doesExpContainOneOp(const std::string & _exp) {
			std::string _operations = "+-!%`;:\'j*/^PCiygmubdlpr<=>";
			std::string _funChars = "ABDFGHJKLMNOQRSTUVWXYZ~@{$&_?}\"\\ qonkvh";
			bool _hasFound = false;
			size_t i = 0;

			if ((_exp.front() == '+' || _exp.front() == '-') && i < _exp.length() - 1 && std::string("`;:\'j").find(_exp[i + 1]) == std::string::npos)
				i++;

			for (i; i < _exp.length(); i++) {
				if (_operations.find(_exp[i]) != std::string::npos || _funChars.find(_exp[i]) != std::string::npos || (i > 0 && _exp[i] == '(' && _funChars.find(_exp[i - 1]) == std::string::npos)) {
					if (i < _exp.length() - 1) {
						if (std::string("`;:\'j").find(_exp[i]) != std::string::npos && (_exp[i + 1] == '+' || _exp[i + 1] == '-')) { //Skip the magic character and the sign after it
							i++;
							continue;
						}

						if (_operations.substr(9).find(_exp[i]) != std::string::npos && (_exp[i + 1] == '+' || _exp[i + 1] == '-')) //Skip the sign after it but not the actual operation
							i++;
					}

					if (_hasFound)
						return false;
					else
						_hasFound = true;
				}
			}

			return true;
		}

		double _evaluatePartial(std::string & _partialExpression, const bool _recordSteps, const bool _funCall = false) {
			if (_evaluationResult != ErrorCodes::NO_ERRORS) //Force exit
				return 0;

			if (_isNumericalValue(_partialExpression)) { //Recursion limit
				double _val = stod(_partialExpression);
				return _val == 0 ? 0 : _val; //To normalize -0.0
			}

			double _subPartialResult = 0;

			//Free Brackets (No functions):
			size_t _operatorPos = 0;
			static std::string _funChars = "ABDFGHJKLMNOQRSTUVWXYZ~@{$&_?}\"\\ qonkvh";

			while (true) {
				while ((_operatorPos = _partialExpression.find('(', _operatorPos)) != std::string::npos && _operatorPos > 0 && _funChars.find(_partialExpression[_operatorPos - 1]) != std::string::npos)
					_operatorPos++;

				if (_operatorPos == std::string::npos)
					break;

				else {
					size_t _closedBracketPos = CommonUtils::findRespectiveBracketPos(_partialExpression, _operatorPos, BracketTypes::REGULAR);
					std::string _subPartialExpression = _partialExpression.substr(_operatorPos + 1, _closedBracketPos - _operatorPos - 1);

					if (_subPartialExpression.find('w') != std::string::npos)
						_operatorPos++;

					else { //Actual evaluation:
						if (_recordSteps)
							_overallOperatorPositions.push(_overallOperatorPositions.top() + _operatorPos + 1);

						_closedBracketPos = _subPartialExpression.length(); //I needed to save the length of _subPartialExpression for later, so I used this variable instead of creating a new one and consume more memory :-)

						const bool _updateLastExp = _isNumericalValue(_subPartialExpression); //Because _evaluatePartial will not touch _evaluationSteps in case of _subPartialResult is a numerical value, we will update it here explicity

						if (_recordSteps)
							_subPartialResult = _evaluatePartial(_subPartialExpression, _recordSteps, _overallOperatorPositions.top() != std::string::npos && _overallOperatorPositions.top() > 0 && _funChars.find(_evaluationSteps.back().first[_overallOperatorPositions.top() - 1]) != std::string::npos);
						else
							_subPartialResult = _evaluatePartial(_subPartialExpression, false);

						_insertSubPartialResult(_partialExpression, _operatorPos, _closedBracketPos, _subPartialResult);

						if (_recordSteps && _evaluationResult == ErrorCodes::NO_ERRORS && _overallOperatorPositions.size() > 1) {
							std::string _lastExp = _evaluationSteps.back().first;
							_subPartialExpression = _lastExp.substr(_overallOperatorPositions.top() + 1, CommonUtils::findRespectiveBracketPos(_lastExp, _overallOperatorPositions.top(), BracketTypes::REGULAR) - _overallOperatorPositions.top() - 1);

							//We don't need to check the evaluation result here, because the bracket contains only one value -- To also deal with redundant brackets:
							if (_updateLastExp || _isNumericalValue(_subPartialExpression)) { //We must re-check that for specific cases
								_insertSubPartialResult(_lastExp, _overallOperatorPositions.top(), _subPartialExpression.length(), _subPartialResult);
								_evaluationSteps.push_back({ _lastExp, false }); //To not show this redundant step in the final vector, but still be consistent with the internal processes
								_overallOperatorPositions.pop(); //To return to the previous level of nesting
							}
						}

						if (_recordSteps)
							return _evaluatePartial(_partialExpression, _recordSteps, _overallOperatorPositions.top() != std::string::npos && _overallOperatorPositions.top() > 0 && _funChars.find(_evaluationSteps.back().first[_overallOperatorPositions.top() - 1]) != std::string::npos);
						else
							return _evaluatePartial(_partialExpression, false);
					}
				}
			}

			//Functions:
			if ((_operatorPos = _partialExpression.find_first_of(_funChars)) != std::string::npos) {
				size_t _closedBracketPos = CommonUtils::findRespectiveBracketPos(_partialExpression, _operatorPos + 1, BracketTypes::REGULAR);

				if (_recordSteps)
					_overallOperatorPositions.push(_overallOperatorPositions.top() + _operatorPos + 2); //To stand on the open bracket of the function

				//To insert magic characters with ^PC!% before the function call
				if (_closedBracketPos < _partialExpression.length() - 1 && std::string("^PC!%").find(_partialExpression[_closedBracketPos + 1]) != std::string::npos) {
					_partialExpression.insert(_partialExpression.begin() + _operatorPos, CommonUtils::getMagicCharacter(_partialExpression[_closedBracketPos + 1]));

					if (_recordSteps) { //To update the _partialExpression
						_evaluationSteps.back().first.insert(_evaluationSteps.back().first.begin() + _overallOperatorPositions.top() - 1, CommonUtils::getMagicCharacter(_evaluationSteps.back().first[CommonUtils::findRespectiveBracketPos(_evaluationSteps.back().first, _overallOperatorPositions.top(), BracketTypes::REGULAR) + 1]));
						_overallOperatorPositions.top()++; //Because the positions are shifted
					}

					//Because the positions are shifted:
					_operatorPos++;
					_closedBracketPos++;
				}

				std::string _subPartialExpression = _partialExpression.substr(_operatorPos + 2, _closedBracketPos - _operatorPos - 2);
				_closedBracketPos = _subPartialExpression.length(); //I needed to save the length of _subPartialExpression for later, so I used this variable instead of creating a new one and consume more memory :-)

				//Trigonometric functions:
				if (std::string("WXYZ~@").find(_partialExpression[_operatorPos]) != std::string::npos)
					_subPartialResult = _calculate_OneArgument(_convertToRadian(_evaluatePartial(_subPartialExpression, _recordSteps, true)), _partialExpression[_operatorPos]);

				//Two-argument functions (nthrt,ranInt,log,mod):
				else if (std::string("?\"{h").find(_partialExpression[_operatorPos]) != std::string::npos) {
					size_t _commaPos = CommonUtils::findFirstCommaPos(_subPartialExpression);

					//One-argument log:
					if (_commaPos == std::string::npos)
						_subPartialResult = _calculate_OneArgument(_evaluatePartial(_subPartialExpression, _recordSteps, true), _partialExpression[_operatorPos]);

					//Two-argument log + nthrt,ranInt,mod:
					else {
						std::string _leftExpression = _subPartialExpression.substr(0, _commaPos);
						std::string _rightExpression = _subPartialExpression.substr(_commaPos + 1);

						double _leftExpressionResult = _evaluatePartial(_leftExpression, _recordSteps, true);
						if (_evaluationResult != ErrorCodes::NO_ERRORS)
							return 0;
						if (_recordSteps)
							_overallOperatorPositions.top() += _leftExpression.length() + 1;

						double _rightExpressionResult = _evaluatePartial(_rightExpression, _recordSteps, true);
						if (_evaluationResult != ErrorCodes::NO_ERRORS)
							return 0;
						if (_recordSteps)
							_overallOperatorPositions.top() -= _leftExpression.length() + 1;

						_subPartialResult = _calculate_TwoArguments(_leftExpressionResult, _rightExpressionResult, _partialExpression[_operatorPos]);
					}
				}

				//Three-argument functions (sum,prod):
				else if (std::string("kv").find(_partialExpression[_operatorPos]) != std::string::npos) {
					size_t _commasPos[2];

					_commasPos[0] = CommonUtils::findFirstCommaPos(_subPartialExpression);
					_commasPos[1] = CommonUtils::findFirstCommaPos(_subPartialExpression, _commasPos[0] + 1);

					std::string _leftExpression = _subPartialExpression.substr(0, _commasPos[0]);
					std::string _middleExpression = _subPartialExpression.substr(_commasPos[0] + 1, _commasPos[1] - _commasPos[0] - 1);
					std::string _rightExpression = _subPartialExpression.substr(_commasPos[1] + 1, _subPartialExpression.length() - _commasPos[1] - 1);

					if (_recordSteps)
						_overallOperatorPositions.top() += _leftExpression.length() + 1;
					double _middleExpressionResult = _evaluatePartial(_middleExpression, _recordSteps, true);
					if (_evaluationResult != ErrorCodes::NO_ERRORS)
						return 0;

					if (_recordSteps)
						_overallOperatorPositions.top() += _middleExpression.length() + 1;
					double _rightExpressionResult = _evaluatePartial(_rightExpression, _recordSteps, true);
					if (_evaluationResult != ErrorCodes::NO_ERRORS)
						return 0;

					if (_recordSteps)
						_overallOperatorPositions.top() -= _leftExpression.length() + _middleExpression.length() + 2;

					_subPartialResult = _calculate_ThreeArguments(_leftExpression, _middleExpressionResult, _rightExpressionResult, _partialExpression[_operatorPos]);
				}

				//Other functions
				else
					_subPartialResult = _calculate_OneArgument(_evaluatePartial(_subPartialExpression, _recordSteps, true), _partialExpression[_operatorPos]);

				_insertSubPartialResult_funs(_partialExpression, _operatorPos + 1, _closedBracketPos, _subPartialResult);

				if (_recordSteps && _evaluationResult == ErrorCodes::NO_ERRORS) { //We must re-extract _subPartialExpression from _lastExp because there might be incosistencies
					std::string _lastExp = _evaluationSteps.back().first;
					_subPartialExpression = _lastExp.substr(_overallOperatorPositions.top() + 1, CommonUtils::findRespectiveBracketPos(_lastExp, _overallOperatorPositions.top(), BracketTypes::REGULAR) - _overallOperatorPositions.top() - 1);
					_insertSubPartialResult_funs(_lastExp, _overallOperatorPositions.top(), _subPartialExpression.length(), _subPartialResult);

					_evaluationSteps.push_back({ _lastExp, true });
					_overallOperatorPositions.pop();
				}

				if (_recordSteps)
					return _evaluatePartial(_partialExpression, _recordSteps, _overallOperatorPositions.top() != std::string::npos && _overallOperatorPositions.top() > 0 && _funChars.find(_evaluationSteps.back().first[_overallOperatorPositions.top() - 1]) != std::string::npos);
				else
					return _evaluatePartial(_partialExpression, _recordSteps);
			}

			//Factorials/Percent
			if ((_operatorPos = _partialExpression.find_first_of("!%")) != std::string::npos) {
				if (_recordSteps)
					_overallOperatorPositions.push(_overallOperatorPositions.top() + _operatorPos + 1);

				std::string _num = CommonUtils::extractLeftValue(_partialExpression, _operatorPos); //This will extract the value with the sign, unless there's no magic chars

				_subPartialResult = _calculate_OneArgument(stod(_num), _partialExpression[_operatorPos]);

				_insertSubPartialResult(_partialExpression, _operatorPos, _num, "", _subPartialResult);

				if (_recordSteps && _evaluationResult == ErrorCodes::NO_ERRORS) {
					std::string _lastExp = _evaluationSteps.back().first;
					_insertSubPartialResult(_lastExp, _overallOperatorPositions.top(), _num, "", _subPartialResult);
					_evaluationSteps.push_back({ _lastExp, true });
					_overallOperatorPositions.pop(); //To return to the previous level of nesting
				}

				if (_recordSteps)
					return _evaluatePartial(_partialExpression, _recordSteps, _overallOperatorPositions.top() != std::string::npos && _overallOperatorPositions.top() > 0 && _funChars.find(_evaluationSteps.back().first[_overallOperatorPositions.top() - 1]) != std::string::npos);
				else
					return _evaluatePartial(_partialExpression, _recordSteps);
			}

			//Exponents:
			else if (_partialExpression.find('^') != std::string::npos)
				_operatorPos = _findCorrectExponentPos(_partialExpression); //Because 2^3^4 = 2^(3^4) NOT (2^3)^4

			//Permutations/Combinations:
			if (_operatorPos == std::string::npos)
				_operatorPos = _partialExpression.find_first_of("PC");

			//Multiplication/Division:
			if (_operatorPos == std::string::npos) //BE CAREFUL NOT TO ADD `else`
				_operatorPos = _partialExpression.find_first_of("*/");

			//Addition/Subtraction:
			if (_operatorPos == std::string::npos) { //BE CAREFUL NOT TO ADD `else`
				size_t _searchFrom = 0;

				if (std::string("+-").find(_partialExpression.front()) != std::string::npos)
					_searchFrom++;

				//The validation below is special only for +- because they can follow any other operator directly
				//We will check only for logical operators because they are what remains at this point
				do {
					_operatorPos = _partialExpression.find_first_of("+-", _searchFrom);
					_searchFrom += _operatorPos + 1;
				} while (_operatorPos != std::string::npos && std::string("iygmubdlpr<=>").find(_partialExpression[_operatorPos - 1]) != std::string::npos);
			}

			//Bitwise Shift Operators:
			if (_operatorPos == std::string::npos)
				_operatorPos = _partialExpression.find_first_of("iy");

			//Logical Operators:
			if (_operatorPos == std::string::npos)
				_operatorPos = _partialExpression.find_first_of("gm<=>");

			//AND,NAND Logic Gate:
			if (_operatorPos == std::string::npos)
				_operatorPos = _partialExpression.find_first_of("bp");;

			//Other Logic Gates:
			if (_operatorPos == std::string::npos)
				_operatorPos = _partialExpression.find_first_of("udlr");

			if (_recordSteps) {
				_overallReplacementType = _doesExpContainOneOp(_partialExpression) ? ReplacementTypes::BRACKET : ReplacementTypes::OPERATION;

				//If dealing with a two-argument function:
				if (_overallOperatorPositions.top() < _evaluationSteps.back().first.length() && _evaluationSteps.back().first[_overallOperatorPositions.top()] == ',')
					_overallReplacementType = ReplacementTypes::OPERATION; //To use the correct _insertSubPartialResult method

				if (_overallReplacementType == ReplacementTypes::OPERATION)
					_overallOperatorPositions.push(_overallOperatorPositions.top() + _operatorPos + 1);
			}

			//Extracting _num1:
			std::string _num1 = CommonUtils::extractLeftValue(_partialExpression, _operatorPos); //This will extract the value with the sign, unless it's ^PC with no magic chars

			//Extracting _num2:
			std::string _num2 = CommonUtils::extractRightValue(_partialExpression, _operatorPos);

			_subPartialResult = _calculate_TwoArguments(stod(_num1), stod(_num2), _partialExpression[_operatorPos]);

			_insertSubPartialResult(_partialExpression, _operatorPos, _num1, _num2, _subPartialResult);

			if (_recordSteps && _evaluationResult == ErrorCodes::NO_ERRORS) {
				std::string _lastExp = _evaluationSteps.back().first;

				if (_overallOperatorPositions.size() > 1) {
					if (_overallReplacementType == ReplacementTypes::BRACKET) {
						if (_funCall) //If it is a function call, we keep the brackets.
							_lastExp.replace(_overallOperatorPositions.top() + 1, _num1.length() + _num2.length() + (std::string("`;:\'j").find(_lastExp[_overallOperatorPositions.top() + 1]) != std::string::npos ? 2 : 1), CommonUtils::convertDoubleToStr(_subPartialResult, _precision)); //We must keep the sign to be consistent with _partialExpression because it'll be the vallue of _subPartialExpression later
						else
							_insertSubPartialResult(_lastExp, _overallOperatorPositions.top(), _num1.length() + _num2.length() + (std::string("`;:\'j").find(_lastExp[_overallOperatorPositions.top() + 1]) != std::string::npos ? 2 : 1), _subPartialResult);
					}
					else
						_insertSubPartialResult(_lastExp, _overallOperatorPositions.top(), _num1, _num2, _subPartialResult);
				}
				else
					_lastExp = _partialExpression;

				_evaluationSteps.push_back({ _lastExp, true });

				if (_overallOperatorPositions.size() > 1 && !(_funCall && _overallReplacementType == ReplacementTypes::BRACKET)) //If it is a function call, we don't pop just yet.
					_overallOperatorPositions.pop(); //To return to the previous level of nesting
			}

			if (_recordSteps)
				return _evaluatePartial(_partialExpression, _recordSteps, _overallOperatorPositions.top() != std::string::npos && _overallOperatorPositions.top() > 0 && _funChars.find(_evaluationSteps.back().first[_overallOperatorPositions.top() - 1]) != std::string::npos);
			else
				return _evaluatePartial(_partialExpression, _recordSteps);
		}

	public:
		ExpressionEvaluator(const short precision, const AngleUnits angleUnit, const bool forceBitwiseLogic, const std::string & fullExpression = "0") {
			_reset();
			_precision = precision;
			_angleUnit = angleUnit;
			_forceBitwiseLogic = forceBitwiseLogic;
			_fullExpression = fullExpression;

			std::ostringstream _oss;
			_oss << std::fixed << std::setprecision(_precision) << 3.14159265358979324;
			_pi = stod(_oss.str()); //To be consistent with inner calculations
		}

		void _reset() {
			_evaluationResult = ErrorCodes::NO_ERRORS;

			if (!_evaluationSteps.empty())
				_evaluationSteps.clear();

			if (_overallOperatorPositions.size() != 1) {
				std::stack<size_t>().swap(_overallOperatorPositions); //To instantly clear the stack
				_overallOperatorPositions.push(std::string::npos);
			}
		}

		void setFullExpression(const std::string & fullExpression) {
			_fullExpression = fullExpression;
		}

		EvaluationResult initiateEvaluation() {
			const EvaluationResult _result = { _evaluatePartial(_fullExpression, false, false) , _evaluationResult };
			
			_reset();
			return _result;
		}

		std::vector<std::string> getEvaluationSteps(const std::unordered_map<char, std::string> & operatorNames, const bool forceDecimalPoints, const bool forceAsterisks, const bool useAbsPoles, const bool useSpaces) {
			_evaluationSteps.push_back({ _fullExpression, true });

			_evaluatePartial(_fullExpression, true, false);

			if (!_evaluationSteps.back().second && _evaluationResult == ErrorCodes::NO_ERRORS)
				_evaluationSteps.back().second = true;

			std::vector<std::string> _finalResult;

			//To format all the non-redundant steps, and store them cleanly in _finalResult:
			for (const auto & Exp : _evaluationSteps)
				if (Exp.second)
					_finalResult.push_back(CommonUtils::formatExpression(Exp.first, operatorNames, forceDecimalPoints, forceAsterisks, useAbsPoles, useSpaces));

			_reset();
			return _finalResult;
		}
	};

	static short _clampNumberBetween(const short _num, const short _from, const short _to) {
		if (_num > _to)
			return _to;

		if (_num < _from)
			return _from;

		return _num;
	}

	std::string _validate_ReturningVector() { //To avoid code duplication
		std::pair<std::string, ErrorCodes> _formatting_result = ExpressionFormatter(_operatorNames, _precision).formatExpression(_expression, _implicitMultHighPrec, true);

		if (_formatting_result.second != ErrorCodes::NO_ERRORS)
			return "";

		_formatting_result.second = ExpressionValidator().validateExpression(_formatting_result.first);

		if (_formatting_result.second != ErrorCodes::NO_ERRORS)
			return "";

		return _formatting_result.first;
	}

public:
	ExpressionParser(const std::string & expression = "0", const short precision = 6, const AngleUnits angleUnit = AngleUnits::RADIAN, const bool implicitMultHighPrec = false, const bool forceBitwiseLogic = false) {
		_implicitMultHighPrec = implicitMultHighPrec;
		_forceBitwiseLogic = forceBitwiseLogic;
		_angleUnit = angleUnit;
		_precision = _clampNumberBetween(precision, 0, 17);
		//DON'T FORGET TO CHANGE THE FOR LOOPS WITH ITERATORS
		_operatorNames = {
			{'A', "asinh"}, {'B', "acosh"}, {'D', "atanh"}, {'F', "acsch"}, {'G', "asech"}, {'H', "acoth"}, {'J', "sinh"},
			{'K', "cosh"}, {'L', "tanh"}, {'M', "csch"}, {'N', "sech"}, {'O', "coth"}, {'Q', "asin"}, {'R', "acos"},
			{'S', "atan"}, {'T', "acsc"}, {'U', "asec"}, {'V', "acot"}, {'W', "sin"}, {'X', "cos"}, {'Y', "tan"}, {'Z', "csc"},
			{'~', "sec"}, {'@', "cot"}, {'{', "log"}, {'$', "ln"}, {'&', "sqrt"}, {'_', "cbrt"}, {'?', "nthrt"}, {'}', "abs"},
			{'\"', "rndInt"}, {'\\', "ispr"}, {' ', "issqr"}, { 'k', "sum" }, {'v', "prod"}, {'i', "<<"}, {'y', ">>"},
			{ 'g', "<=" }, {'m', ">="}, {'u', "XNOR"}, {'b', "NAND"}, {'d', "NOR"}, {'l', "XOR"}, {'p', "AND"},
			{'q', "NOT"}, {'r', "OR"}, {'o', "isodd"}, {'n', "iseven"}, {'h', "mod"}
		};

		setExpression(expression);
	}

	std::string getExpression() const {
		return _expression;
	}

	//* If an empty string is passed, it will be turned into "0"
	void setExpression(const std::string & expression) {
		_expression = expression == "" ? "0" : expression;

		if (_expression != "0") {
			_formattingResult = ExpressionFormatter(_operatorNames, _precision).formatExpression(_expression, _implicitMultHighPrec);

			if (_formattingResult.second == ErrorCodes::NO_ERRORS) { //If the format succeeded
				_formattingResult.second = ExpressionValidator::validateExpression(_formattingResult.first);

				if (_formattingResult.second != ErrorCodes::NO_ERRORS)
					_formattingResult.first = "";
			}
		}
	}

	short getPrecision() const {
		return _precision;
	}

	//* This controls the precision of the results, internal calculations, and formatted expressions.
	//* It will get clamped between [0,17]
	//* Default value: 6 digits after the decimal point
	void setPrecision(const short precision) {
		_precision = _clampNumberBetween(precision, 0, 17);

		//To refresh the formatted expression:
		if ((_expression.find('e') != std::string::npos || _expression.find("pi") != std::string::npos || _expression.find("rnd#") != std::string::npos) && _formattingResult.second == ErrorCodes::NO_ERRORS)
			_formattingResult = ExpressionFormatter(_operatorNames, _precision).formatExpression(_expression, _implicitMultHighPrec);
	}

	AngleUnits getAngleUnit() const {
		return _angleUnit;
	}

	//* This is used when calculating trigonometric functions.
	//* Default value: RADIAN.
	void setAngleUnit(const AngleUnits angleUnit) {
		_angleUnit = angleUnit;
	}

	bool getImplicitMultHighPrec() const {
		return _implicitMultHighPrec;
	}

	//* This controls whether implicit multiplication (juxtaposition) binds more tightly than standard left-to-right evaluation.
	//* If true, it will give higher precedence over division. Otherwise, it will not.
	//* This doesn't affect functions with implicit brackets, they have their own rules.
	//* Defualt value: false
	void setImplicitMultHighPrec(const bool implicitMultHighPrec) {
		_implicitMultHighPrec = implicitMultHighPrec;

		//To refresh the formatted expression
		if (_formattingResult.second == ErrorCodes::NO_ERRORS)
			_formattingResult = ExpressionFormatter(_operatorNames, _precision).formatExpression(_expression, _implicitMultHighPrec);
	}

	bool getForceBitwiseOperations() const {
		return _forceBitwiseLogic;
	}

	//* This controls the behavior of logical operators.
	//* If true, logical operators will behave as bitwise operators, producing error codes if used with non-integer values.
	//* If false, logical operators will use standard logical evaluation, treating any non-zero value as `true`.
	//* Default value: false
	void setForceBitwiseLogic(const bool forceBitwiseLogic) {
		_forceBitwiseLogic = forceBitwiseLogic;
	}

	//* This will return a struct, containing the value and the evaluation result (error code).
	EvaluationResult evaluate() const {
		if (_formattingResult.second != ErrorCodes::NO_ERRORS)
			return { 0, _formattingResult.second };

		return ExpressionEvaluator(_precision, _angleUnit, _forceBitwiseLogic, _formattingResult.first).initiateEvaluation();
	}

	//* This method will return an empty string, should the expression be invalid.
	std::string formatExpression(const bool forceDecimalPoints, const bool forceAsterisks, const bool useAbsPoles, const bool useSpaces) const {
		if (_formattingResult.second != ErrorCodes::NO_ERRORS)
			return "";

		return CommonUtils::formatExpression(_formattingResult.first, _operatorNames, forceDecimalPoints, forceAsterisks, useAbsPoles, useSpaces);
	}

	//* This method will return an empty vector, should the expression be invalid.
	//* If a calculation goes wrong (i.e sqrt(-2)), the vector will stop at the last successful step.
	std::vector<std::string> generateEvaluationSteps(const bool forceDecimalPoints, const bool forceAsterisks, const bool useAbsPoles, const bool useSpaces) const {
		if (_formattingResult.second != ErrorCodes::NO_ERRORS)
			return {};

		return ExpressionEvaluator(_precision, _angleUnit, _forceBitwiseLogic, _formattingResult.first).getEvaluationSteps(_operatorNames, forceDecimalPoints, forceAsterisks, useAbsPoles, useSpaces);
	}

	//* The expression should be a function of x
	//* This method will substitute in the expression and return the result
	EvaluationResult evaluateAt(const double value) {
		std::pair<std::string, ErrorCodes> _formatting_result = ExpressionFormatter(_operatorNames, _precision).formatExpression(_expression, _implicitMultHighPrec, true);
		
		if (_formatting_result.second != ErrorCodes::NO_ERRORS)
			return { 0, _formatting_result.second };
		
		_formatting_result.second = ExpressionValidator().validateExpression(_formatting_result.first);

		if (_formatting_result.second != ErrorCodes::NO_ERRORS)
			return { 0, _formatting_result.second };

		return ExpressionEvaluator(_precision, _angleUnit, _forceBitwiseLogic, CommonUtils::plugInVal(_formatting_result.first, value, _precision)).initiateEvaluation();
	}

	//* The expression should be a function of x
	//* This method will substitute in the expression and return the result
	std::vector<EvaluationResult> evaluateAt(const std::vector<double> & values) {
		std::pair<std::string, ErrorCodes> _formatting_result = ExpressionFormatter(_operatorNames, _precision).formatExpression(_expression, _implicitMultHighPrec, true);

		if (_formatting_result.second != ErrorCodes::NO_ERRORS)
			return { { 0, _formatting_result.second } };

		_formatting_result.second = ExpressionValidator().validateExpression(_formatting_result.first);

		if (_formatting_result.second != ErrorCodes::NO_ERRORS)
			return { { 0, _formatting_result.second } };

		std::vector<EvaluationResult> _vResult;
		ExpressionEvaluator _evaluator(_precision, _angleUnit, _forceBitwiseLogic);

		for (const double V : values) {
			_evaluator.setFullExpression(CommonUtils::plugInVal(_formatting_result.first, V, _precision));
			_vResult.push_back(_evaluator.initiateEvaluation());
		}

		return _vResult;
	}

	//* The expression should be a function of x
	//* This method will substitute in the expression and return the result
	std::vector<std::string> generateEvaluationStepsAt(const double value, const bool forceDecimalPoints, const bool forceAsterisks, const bool useAbsPoles, const bool useSpaces) {
		std::string _validating_result = _validate_ReturningVector();
		
		if (_validating_result == "")
			return {};

		return ExpressionEvaluator(_precision, _angleUnit, _forceBitwiseLogic, CommonUtils::plugInVal(_validating_result, value, _precision)).getEvaluationSteps(_operatorNames, forceDecimalPoints, forceAsterisks, useAbsPoles, useSpaces);
	}

	//* The expression should be a function of x
	//* This method will substitute in the expression and return the result
	std::vector<std::vector<std::string>> generateEvaluationStepsAt(const std::vector<double> & values, const bool forceDecimalPoints, const bool forceAsterisks, const bool useAbsPoles, const bool useSpaces) {
		std::string _expAfterValidation = _validate_ReturningVector();

		if (_expAfterValidation == "")
			return {};

		std::vector<std::vector<std::string>> _vResult;
		ExpressionEvaluator _evaluator(_precision, _angleUnit, _forceBitwiseLogic);

		for (const double V : values) {
			_evaluator.setFullExpression(CommonUtils::plugInVal(_expAfterValidation, V, _precision));
			_vResult.push_back(_evaluator.getEvaluationSteps(_operatorNames, forceDecimalPoints, forceAsterisks, useAbsPoles, useSpaces));
		}

		return _vResult;
	}

};
