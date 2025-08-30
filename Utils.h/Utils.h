// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "B:\PROGRAMMING\Abu-Hadhoud\My Libraries\Strings.h"

using namespace std;

namespace Utils {
	namespace Maths {
		template <typename Number>
		Number specialRound(const Number & number, const short & roundPrecision) {
			Number tenPower = static_cast<Number>(pow(10, roundPrecision));
			return round(number * tenPower) / tenPower;
		}

		struct sNumberParts {
			unsigned int integerPart = 0;
			unsigned int decimalPart = 0;
			bool isPositive = true;
		};

		sNumberParts getNumberParts(double number, const short & roundPrecision) {
			sNumberParts parts;

			parts.isPositive = number > 0;

			number = abs(number);
			number = specialRound(number, roundPrecision);

			parts.integerPart = static_cast<unsigned int>(number);
			parts.decimalPart = static_cast<unsigned int>(round((number - parts.integerPart) * pow(10, roundPrecision)));

			return parts;
		}

		template <typename Number>
		bool isNumberBetween(const Number & number, Number value1, Number value2) {
			if (value1 > value2)
				swap(value1, value2);

			return number >= value1 && number <= value2;
		}

		template <typename Integer>
		bool isEven(const Integer & n) {
			return n % 2 == 0;
		}

		template <typename Integer>
		bool isPrime(const Integer & number) {
			if (number <= 1)
				return false;

			const unsigned short & divisionLimit = static_cast<Integer>(sqrt(number));

			for (unsigned short i = 2; i <= divisionLimit; i++) {
				if (number % i == 0)
					return false;
			}

			return true;
		}

		template <typename Number>
		Number getMax(const Number & num1, const Number & num2) {
			return num1 > num2 ? num1 : num2;
		}

		template <typename Number>
		Number getMax(const Number & num1, const Number & num2, const Number & num3) {
			if (num1 > num2)
				return num1 > num3 ? num1 : num3;

			return num2 > num3 ? num2 : num3;
		}

		template <typename Number>
		Number getMin(const Number & num1, const Number & num2) {
			return num1 < num2 ? num1 : num2;
		}

		template <typename Number>
		Number getMin(const Number & num1, const Number & num2, const Number & num3) {
			if (num1 < num2)
				return num1 < num3 ? num1 : num3;

			return num2 < num3 ? num2 : num3;
		}

		template <typename Integer>
		vector<Integer> getVectorOfPrimeNumbers(Integer start, Integer end) {
			if (start > end)
				swap(start, end);

			vector<Integer> vPrimeNumbers;

			if (start <= 2) {
				if (end >= 2) {
					vPrimeNumbers.push_back(2);
					start = 3;
				}
				else
					return vPrimeNumbers;
			}
			else if (start % 2 == 0)
				start++;

			for (start; start <= end; start += 2)
				if (isPrime<Integer>(start))
					vPrimeNumbers.push_back(start);

			return vPrimeNumbers;
		}

		template <typename Integer>
		vector<Integer> getFibonacciSequence(const short & numberOfElements) {
			vector<Integer> sequence;
			Integer prelast = 0;
			Integer last = 1;
			Integer current = 1;

			if (numberOfElements >= 1) sequence.push_back(0);
			if (numberOfElements >= 2) sequence.push_back(1);

			for (short i = 2; i < numberOfElements; i++) {
				current = last + prelast;
				prelast = last;
				last = current;

				sequence.push_back(current);
			}

			return sequence;
		}

		template <typename Integer>
		Integer getFactorial(const short & n) {
			if (n < 0)
				return 0;

			Integer factorial = 1;

			for (short i = 2; i <= n; i++)
				factorial *= i;

			return factorial;
		}

		template <typename Integer>
		Integer getPermutations(const unsigned short & n, const unsigned short & r) {
			return getFactorial<Integer>(n) / getFactorial<Integer>(n - r);
		}

		template <typename Integer>
		Integer getCombinations(const unsigned short & n, const unsigned short & r) {
			return getPermutations<Integer>(n, r) / getFactorial<Integer>(r);
		}

		template <typename Number>
		void clampNumberBetween(Number & number, Number value1, Number value2) {
			if (value1 > value2)
				swap(value1, value2);

			if (number < value1)
				number = value1;
			else if (number > value2)
				number = value2;
		}

		template <typename Integer>
		string getOrdinalSuffix(const Integer & number) {
			string suffixes[4] = { "th", "st", "nd", "rd" };

			return suffixes[number % 10 > 3 || number / 10 == 1 ? 0 : number % 10];
		}
	}

	namespace Generators {
		enum enCharacterTypes {
			eCapitalLetters, eSmallLetter, eSpecialCharacter, eDigit, eNonSpecial, eAll
		};

		void intializeRandomizer() {
			srand((unsigned int) time(NULL));
		}

		template <typename Integer>
		Integer getRandomNumber() {
			return rand();
		}

		//No need to call intializeRandomizer() when using this.
		template <typename Integer>
		Integer getRandomNumber(Integer from, Integer to) {
			random_device rd;
			mt19937 gen(rd()); // Mersenne Twister generator

			uniform_int_distribution<Integer> dist(from, to);
			return dist(gen);
		}

		//50% true, 50% false
		bool getRandomBoolean() {
			return getRandomNumber<short>(0, 1);
		}

		// WARNING: Only use integer values.
		// The probability value will be clamped between [0,outOf]
		// For example: 18 out of 63 -> around 28.571% probability of returning true
		template <typename Integer>
		bool getRandomBoolean(Integer probability, Integer outOf) {
			Maths::clampNumberBetween<Integer>(probability, 0, outOf);

			if (probability == 0 || probability == outOf)
				return static_cast<bool>(probability);

			return getRandomNumber<Integer>(1, outOf) <= probability;
		}

		char getRandomCharacter(const enCharacterTypes & type) {
			switch (type) {
				case enCharacterTypes::eCapitalLetters:
					return char(getRandomNumber(65, 90));
				case enCharacterTypes::eSmallLetter:
					return char(getRandomNumber(97, 122));
				case enCharacterTypes::eSpecialCharacter: {
					short n = getRandomNumber(1, 4);

					if (n == 1)
						return char(getRandomNumber(33, 47));
					if (n == 2)
						return char(getRandomNumber(58, 64));
					if (n == 3)
						return char(getRandomNumber(91, 96));
					return char(getRandomNumber(123, 126));
				}
				case enCharacterTypes::eDigit:
					return char(getRandomNumber(48, 57));
				case enCharacterTypes::eNonSpecial: {
					short n = getRandomNumber(1, 3);

					if (n == 1)
						return getRandomCharacter(enCharacterTypes::eCapitalLetters);
					if (n == 2)
						return getRandomCharacter(enCharacterTypes::eSmallLetter);
					return getRandomCharacter(enCharacterTypes::eDigit);
				}
				case enCharacterTypes::eAll: {
					short n = getRandomNumber(1, 4);

					if (n == 1)
						return getRandomCharacter(enCharacterTypes::eCapitalLetters);
					if (n == 2)
						return getRandomCharacter(enCharacterTypes::eSmallLetter);
					if (n == 3)
						return getRandomCharacter(enCharacterTypes::eSpecialCharacter);
					return getRandomCharacter(enCharacterTypes::eDigit);
				}
				default:
					return char(254);
			}
		}

		char getRandomCharacter(const enCharacterTypes & type, const string & invalidChars) {
			char character = 0;

			do character = getRandomCharacter(type);
			while (invalidChars.find(character) != string::npos);

			return character;
		}

		char getRandomCharacter(const string & acceptedChars) {
			return acceptedChars[getRandomNumber<short>(0, acceptedChars.length() - 1)];
		}

		string getRandomWord(const enCharacterTypes & type, unsigned short length) {
			string word = "";

			for (length; length > 0; length--)
				word += getRandomCharacter(type);

			return word;
		}

		string getRandomWord(const enCharacterTypes & type, const string & invalidChars, unsigned short length) {
			string word = "";
			char charToAppend = 0;

			for (length; length > 0; length--) 
				word += getRandomCharacter(type, invalidChars);

			return word;
		}
		
		string getRandomWord(const string & acceptedChars, unsigned short length) {
			string word = "";

			for (length; length > 0; length--)
				word += getRandomCharacter(acceptedChars);
			
			return word;
		}

		string getRandomKey(const enCharacterTypes & type, const unsigned short & slotLength, unsigned short keySize, const char & separator = '-') {
			string key = "";

			for (keySize; keySize > 0; keySize--)
				key += getRandomWord(type, slotLength) + separator;

			key.pop_back(); //To remove the separator at the end

			return key;
		}
	
		string getRandomKey(const enCharacterTypes & type, const string & invalidChars, const unsigned short & slotLength, unsigned short keySize, const char & separator = '-') {
			string key = "";

			for (keySize; keySize > 0; keySize--)
				key += getRandomWord(type, invalidChars, slotLength) + separator;

			key.pop_back(); //To remove the separator at the end

			return key;
		}

		string getRandomKey(const string & acceptedChars, unsigned short slotLength, unsigned short keySize, const char & separator = '-') {
			string key = "";
		
			for (keySize; keySize > 0; keySize--)
				key += getRandomWord(acceptedChars, slotLength) + separator;
		
			key.pop_back(); //To remove the separator at the end

			return key;
		}

	}

	namespace Arrays {
		enum enRotationDirections { eToLeft, eToRight };

		template <typename Integer>
		void fillArrayWithRandomNumbers(Integer array[], const unsigned short & arrayLength) {
			for (unsigned short i = 0; i < arrayLength; i++)
				array[i] = Generators::getRandomNumber<Integer>();
		}

		template <typename Integer>
		void fillArrayWithRandomNumbers(Integer array[], const unsigned short & arrayLength, const int from, const int to) {
			for (unsigned short i = 0; i < arrayLength; i++)
				array[i] = Generators::getRandomNumber(from, to);
		}

		void fillArrayWithRandomCharacters(char array[], const unsigned short & arrayLength, const Generators::enCharacterTypes type) {
			for (unsigned short i = 0; i < arrayLength; i++)
				array[i] = Generators::getRandomCharacter(type);
		}

		void fillArrayWithRandomWords(string array[], const unsigned short & arrayLength, const Generators::enCharacterTypes type, const unsigned short & wordLength) {
			for (unsigned short i = 0; i < arrayLength; i++)
				array[i] = Generators::getRandomWord(type, wordLength);
		}

		void fillArrayWithRandomKeys(string array[], const unsigned short & arrayLength, const Generators::enCharacterTypes type, const unsigned short & slotLength, const unsigned short & keySize) {
			for (unsigned short i = 0; i < arrayLength; i++)
				array[i] = Generators::getRandomKey(type, slotLength, keySize);
		}

		template <typename DataType>
		void shuffleArray(DataType array[], const unsigned short & arrayLength) {
			for (unsigned short i = 0; i < arrayLength; i++)
				swap(array[i], array[Generators::getRandomNumber<unsigned short>(i, arrayLength - 1)]);
		}

		template <typename Number>
		Number getMin(Number array[], const unsigned short & arrayLength) {
			Number min = array[0];

			for (unsigned short i = 1; i < arrayLength; i++)
				if (array[i] < min)
					min = array[i];

			return min;
		}

		template <typename Number>
		Number getMax(Number array[], const unsigned short & arrayLength) {
			Number max = array[0];

			for (unsigned short i = 1; i < arrayLength; i++)
				if (array[i] > max)
					max = array[i];

			return max;
		}

		template <typename Number>
		Number getSum(Number array[], const unsigned short & arrayLength) {
			Number sum = 0;

			for (unsigned short i = 0; i < arrayLength; i++)
				sum += array[i];

			return sum;
		}

		template <typename Number>
		float getAverage(Number array[], const unsigned short & arrayLength) {
			Number sum = getSum<Number>(array, arrayLength);

			return static_cast<float>(sum) / arrayLength;
		}

		template <typename Number>
		float getMedian(Number array[], const unsigned short & arrayLength) {
			if (arrayLength % 2) //If odd
				return array[arrayLength / 2];
			else //If even
				return static_cast<float>(array[arrayLength / 2] + array[arrayLength / 2 - 1]) / 2;
		}

		template <typename DataType>
		bool doesElementExistIn(DataType array[], const unsigned short & arrayLength, const DataType & element) {
			for (unsigned short i = 0; i < arrayLength; i++)
				if (array[i] == element)
					return true;

			return false;
		}

		template <typename DataType>
		unsigned short getElementCount(DataType array[], const unsigned short & arrayLength, const DataType & element) {
			unsigned short count = 0;

			for (unsigned short i = 0; i < arrayLength; i++)
				if (array[i] == element)
					count++;

			return count;
		}

		template <typename Number>
		bool isSorted(Number array[], const unsigned short & arrayLength) {
			for (unsigned short i = 1; i < arrayLength; i++)
				if (array[i] < array[i - 1])
					return false;

			return true;
		}

		//This is a Bubble Sort Algorithm...
		template <typename Number>
		void sort(Number array[], const unsigned short & arrayLength) {
			bool didSwapOccur = true;

			while (didSwapOccur) {
				didSwapOccur = false;

				for (unsigned short i = 1; i < arrayLength; i++)
					if (array[i] < array[i - 1]) {
						swap(array[i], array[i - 1]);
						didSwapOccur = true;
					}
			}
		}

		template <typename DataType>
		void reverse(DataType array[], const unsigned short & arrayLength) {
			for (unsigned short i = 0; i < arrayLength / 2; i++)
				swap(array[i], array[arrayLength - i - 1]);
		}

		template <typename DataType>
		bool isPalindrome(DataType array[], const unsigned short & arrayLength) {
			for (unsigned short i = 0; i < arrayLength / 2; i++)
				if (array[i] != array[arrayLength - i - 1])
					return false;

			return true;
		}

		template <typename DataType>
		void rotateElements(DataType array[], const unsigned short & arrayLength, unsigned short steps, const enRotationDirections & direction) {
			vector<DataType> vResult;
			unsigned short factor = 0;

			while (steps >= arrayLength)
				steps -= arrayLength;

			if (direction == enRotationDirections::eToLeft) {
				for (unsigned short i = 0; i < arrayLength; i++) {
					if (i + steps - factor * arrayLength >= arrayLength)
						factor++;

					vResult.push_back(array[i + steps - factor * arrayLength]);
				}

				for (unsigned short i = 0; i < arrayLength; i++)
					array[i] = vResult[i];
			}
			else if (direction == enRotationDirections::eToRight)
				rotateElements(array, arrayLength, arrayLength - steps, enRotationDirections::eToLeft);
		}

		template <typename DataType>
		string toString(DataType array[], const unsigned short & arrayLength, const string & separator = ", ") {
			if (arrayLength == 0)
				return "[]";
			
			ostringstream result;

			result << '[' << array[0];

			for (unsigned short i = 1; i < arrayLength; i++)
				result << separator << array[i];

			result << ']';
			return result.str();
		}
	
	}

	namespace Vectors {
		enum enRotationDirections { eToLeft, eToRight };

		template <typename Integer>
		vector<Integer> getVectorOfRandomNumbers(unsigned short vectorSize) {
			vector<Integer> vNumbers;

			for (vectorSize; vectorSize > 0; vectorSize--)
				vNumbers.push_back(Generators::getRandomNumber<Integer>());

			return vNumbers;
		}

		template <typename Integer>
		vector<Integer> getVectorOfRandomNumbers(unsigned short vectorSize, const Integer from, const Integer to) {
			vector<Integer> vNumbers;

			for (vectorSize; vectorSize > 0; vectorSize--)
				vNumbers.push_back(getRandomNumber(from, to));

			return vNumbers;
		}

		vector<char> getVectorOfRandomCharacters(const Generators::enCharacterTypes type, unsigned short vectorSize) {
			vector<char> vCharacters;

			for (vectorSize; vectorSize > 0; vectorSize--)
				vCharacters.push_back(Generators::getRandomCharacter(type));

			return vCharacters;
		}

		vector<string> getVectorOfRandomWords(const Generators::enCharacterTypes type, const unsigned short & wordLength, unsigned short vectorSize) {
			vector<string> vWords;

			for (vectorSize; vectorSize > 0; vectorSize--)
				vWords.push_back(Generators::getRandomWord(type, wordLength));

			return vWords;
		}

		vector<string> getVectorOfRandomKeys(const Generators::enCharacterTypes type, const unsigned short & slotLength, const unsigned short & keySize, unsigned short vectorSize) {
			vector<string> vKeys;

			for (vectorSize; vectorSize > 0; vectorSize--)
				vKeys.push_back(Generators::getRandomKey(type, slotLength, keySize));

			return vKeys;
		}

		template <typename DataType>
		void shuffleVector(vector<DataType> & vct) {
			for (unsigned short i = 0; i < vct.size(); i++)
				swap(vct[i], vct[getRandomNumber(i, vct.size() - 1)]);
		}

		template <typename Number>
		Number getMin(vector<Number> & vct) {
			Number min = vct[0];

			for (Number & N : vct)
				if (N < min)
					min = N;

			return min;
		}

		template <typename Number>
		Number getMax(vector<Number> & vct) {
			Number max = vct[0];

			for (Number & N : vct)
				if (N > max)
					max = N;

			return max;
		}

		template <typename Number>
		Number getSum(vector<Number> & vct) {
			Number sum = 0;

			for (Number & N : vct)
				sum += N;

			return sum;
		}

		template <typename Number>
		float getAverage(vector<Number> & vct) {
			Number sum = getSum<Number>(vct);

			return static_cast<float>(sum) / vct.size();
		}

		template <typename Number>
		float getMedian(vector<Number> & vct) {
			if (vct.size() % 2) //If odd
				return vct[vct.size() / 2];
			else //If even
				return static_cast<float>(vct[vct.size() / 2] + vct[vct.size() / 2 - 1]) / 2;
		}

		template <typename DataType>
		bool doesElementExistIn(vector<DataType> & vct, const DataType & element) {
			for (DataType & E : vct)
				if (E == element)
					return true;

			return false;
		}

		template <typename DataType>
		unsigned short getElementCount(vector<DataType> & vct, const DataType & element) {
			unsigned short count = 0;

			for (DataType & E : vct)
				if (E == element)
					count++;

			return count;
		}

		template <typename Number>
		bool isSorted(vector<Number> & vct) {
			for (unsigned short i = 1; i < vct.size(); i++)
				if (vct[i] < vct[i - 1])
					return false;

			return true;
		}

		//This is a Bubble Sort Algorithm...
		template <typename Number>
		void sort(vector<Number> & vct) {
			bool didSwapOccur = true;

			while (didSwapOccur) {
				didSwapOccur = false;

				for (unsigned short i = 1; i < vct.size(); i++)
					if (vct[i] < vct[i - 1]) {
						swap(vct[i], vct[i - 1]);
						didSwapOccur = true;
					}
			}
		}

		template <typename DataType>
		void reverse(vector<DataType> & vct) {
			for (unsigned short i = 0; i < vct.size() / 2; i++)
				swap(vct[i], vct[vct.size() - i - 1]);
		}

		template <typename DataType>
		bool isPalindrome(vector<DataType> & vct) {
			for (unsigned short i = 0; i < vct.size() / 2; i++)
				if (vct[i] != vct[vct.size() - i - 1])
					return false;

			return true;
		}

		template <typename DataType>
		void removeDuplicates(vector<DataType> & vct) {
			for (short i = vct.size() - 1; i >= 0; i--)
				if (getElementCount<DataType>(vct, vct[i]) > 1)
					vct.erase(vct.begin() + i);
		}

		template <typename DataType>
		void rotateElements(vector<DataType> & vct, unsigned short steps, const enRotationDirections direction) {
			vector<DataType> vResult;
			unsigned short factor = 0;

			while (steps >= vct.size())
				steps -= vct.size();

			if (direction == enRotationDirections::eToLeft) {
				for (unsigned short i = 0; i < vct.size(); i++) {
					if (i + steps - factor * vct.size() >= vct.size())
						factor++;

					vResult.push_back(vct[i + steps - factor * vct.size()]);
				}

				vct = vResult;
			}
			else if (direction == enRotationDirections::eToRight)
				rotateElements(vct, vct.size() - steps, enRotationDirections::eToLeft);
		}

		template <typename DataType>
		vector<DataType> mergeTwoVectors(const vector<DataType> & vct1, const vector<DataType> & vct2) {
			vector<DataType> vResult;

			for (DataType & E1 : vct1)
				vResult.push_back(E1);

			for (DataType & E2 : vct2)
				vResult.push_back(E2);

			return vResult;
		}

		template <typename DataType>
		vector<vector<DataType>> splitVector(const vector<DataType> & vct, const DataType & delimiter) {
			vector<DataType> subVector;
			vector<vector<DataType>> subVectors;

			for (const DataType & E : vct) {
				if (E == delimiter) {
					subVectors.push_back(subVector);
					subVector.clear();
				}
				else
					subVector.push_back(E);
			}

			subVectors.push_back(subVector);

			return subVectors;
		}

		template <typename DataType>
		vector<DataType> getSubVector(const vector<DataType> & vct, unsigned short startPos, unsigned short endPos) {
			if (startPos > endPos)
				swap(startPos, endPos);

			vector<DataType> subVector;
			subVector.assign(vct.begin() + startPos, vct.begin() + endPos + 1);

			return subVector;
		}
	
		template <typename DataType>
		string toString(const vector<DataType> & vct, const string & separator = ", ") {
			if (vct.empty())
				return "[]";

			ostringstream result;

			result << '[' << vct[0];

			for (unsigned short i = 1; i < vct.size(); i++)
				result << separator << vct[i];

			result << ']';
			return result.str();
		}
	
	}

	namespace Encrypt_Decrypt {
		string encrypt(string text, const string & key) {
			unsigned short keyPos = 0;

			for (char & C : text) {
				if (keyPos == key.length())
					keyPos = 0;

				//Here goes the encyrption algorithm...
				C = (C + key[keyPos] - 64) % 95 + 32;

				keyPos++;
			}

			return text;
		}

		string decrypt(string text, const string & key) {
			unsigned short keyPos = 0;
			
			for (char & C : text) {
				if (keyPos == key.length())
					keyPos = 0;

				//Here goes the decyrption algorithm...
				C = (C - key[keyPos] + 95) % 95 + 32;

				keyPos++;
			}

			return text;
		}
	}

	namespace Mesc {
		void printHeader(const short & outerIndent, const char & symbol, const string & title, const string & subtitle = "") {
			const short maxLength = Maths::getMax<short>(title.length(), subtitle.length());
			const short shortIndent = round(sqrt(3 * maxLength));
			const short longIndent = shortIndent + abs(static_cast<short>(title.length() - subtitle.length())) / 2;

			//To print the upper line:
			cout << setw(outerIndent) << "" << string(maxLength + shortIndent * 2, symbol) << "\n\n";

			//To print the title:
			cout << setw(outerIndent) << "" << string(title.length() > subtitle.length() ? shortIndent : longIndent, ' ') << title << "\n";

			//To print the subtitle:
			if (subtitle != "")
				cout << setw(outerIndent) << "" << string(subtitle.length() > title.length() ? shortIndent : longIndent, ' ') << subtitle << "\n";

			//To print the bottom line:
			cout << '\n' << setw(outerIndent) << "" << string(maxLength + shortIndent * 2, symbol) << "\n";
		}

		string getNumberAsText(const double & number, const short & roundPrecision = 0) {
			return strings::convertNumToTxt(number, roundPrecision);
		}
	}

}

/*
	namespace helpers {
		short getEncryptionCounter(const string & key) {
			vector<short> vDigits;

			for (const char & C : key)
				if (isdigit(C))
					vDigits.push_back(C - 48);

			if (vDigits.empty())
				return round(static_cast<float>(key[0] + key[key.length() - 1]) / 24);
			else {
				short s = key[0] + key[key.length() - 1];

				if ((s % 9 < 4 && s % 5 != 1) || (s % 11 == 3 && s % 4 != 0))
					s = 0;
				else
					s = 1;

				for (s; s < vDigits.size(); s += 3)
					vDigits[s] *= -1;

				s = 0;
				for (short & D : vDigits)
					s += D;

				if (s == 0)
					return round(static_cast<float>(key[0] + key[key.length() - 1]) / 24);

				return abs(s);
			}
		}
	}
*/
