// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

/* Abbreviations used in this file are:
* vow = vowel
* cap = capitalize, capital
* ltr = letter
* str = string
* fst = first
* low = lower
* wrd = word
* chr = character
* dig = digit
* rev = reverse
* inv = invert
* num = number
* cnt = count, counter
* sml = small
* spc = space
* crr = current
* vwl = vowel
* vct = vector
* tkn = token
* del = delimiter
* pos = position
* trm = trim
* rgt = right
* lft = left
* rem = remove, erase
* punc = punctuations, punctuation marks
* oper = operations, operation marks
* mrg = merge
* arr = array
* lng = length
* seq = sequence
* txt = text

*/

#pragma once

#include <string>
#include <cctype>
#include <cmath>
#include <vector>

using namespace std;

namespace strings_utilities {

	//if (num >= 1000000000000 && num <= 999999999999999) //From trillion to quadrillion
	//	return numToTxtCore(num / 1000000000000) + " Trillion " + numToTxtCore(num % 1000000000000);
	//
	//if (num >= 1000000000000000 && num <= 999999999999999999) //From quadrillion to quintillion
	//	return numToTxtCore(num / 1000000000000000) + " Quadrillion " + numToTxtCore(num % 1000000000000000);
	//
	//if (num >= 1000000000000000000 && num <= 18446744073709551615) //From quintillion to unsinged long long limit
	//	return numToTxtCore(num / 1000000000000000000) + " Quintillion " + numToTxtCore(num % 1000000000000000000);


	//This function only deals with integer values up to unsigned int
	string numToTxtCore(const unsigned int & num) {
		if (num >= 0 && num <= 19) {
			string arr[20] = { "", "One", "Two", "Three", "Four", "Five", "Six",
				"Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen",
				"Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"
			};

			return arr[num];
		}

		if (num >= 20 && num <= 99) {
			string arr[8] = { "Twenty", "Thirty", "Forty", "Fifty",
				"Sixty", "Seventy", "Eighty", "Ninety"
			};

			return arr[num / 10 - 2] + ' ' + numToTxtCore(num % 10);
		}

		if (num >= 100 && num <= 999) //From hundred to thousand
			return numToTxtCore(num / 100) + " Hundred " + numToTxtCore(num % 100);

		if (num >= 1000 && num <= 999999) //From thousand to million
			return numToTxtCore(num / 1000) + " Thousand " + numToTxtCore(num % 1000);

		if (num >= 1000000 && num <= 999999999) //From million to billion
			return numToTxtCore(num / 1000000) + " Million " + numToTxtCore(num % 1000000);

		if (num >= 1000000000 && num <= 999999999999) //From billion to trillion
			return numToTxtCore(num / 1000000000) + " Billion " + numToTxtCore(num % 1000000000);
	}

	double specialRound(const double & number, const short & roundPrecision) {
		return round(number * pow(10, roundPrecision)) / pow(10, roundPrecision);
	}
}

namespace strings {
	bool isVow(char ltr) {
		ltr = tolower(ltr);

		return ltr == 'a' ||
			ltr == 'e' ||
			ltr == 'o' ||
			ltr == 'i' ||
			ltr == 'u';
	}

	bool isLtr(char chr) {
		return isupper(chr) || islower(chr);
	}

	//Specials are: @ # $ % ^ & ~ ` _ \ | 
	bool isSpecial(char chr) {
		return chr == '@' ||
			chr == '#' ||
			chr == '$' ||
			chr == '%' ||
			chr == '^' ||
			chr == '&' ||
			chr == '~' ||
			chr == '`' ||
			chr == '_' ||
			chr == '\\' ||
			chr == '|';
	}
	
	//Punctuations are: . , ; : ? ! ' "
	bool isPunc(const char chr) {
		return chr == '.' ||
			chr == ',' ||
			chr == ';' ||
			chr == ':' ||
			chr == '?' ||
			chr == '!' ||
			chr == '\'' ||
			chr == '"';
	}

	//Operations are: + - * / × ÷ = < > ^
	bool isOper(const char chr) {
		return chr == '+' ||
			chr == '-' ||
			chr == '*' ||
			chr == '/' ||
			chr == '×' ||
			chr == '÷' ||
			chr == '=' ||
			chr == '<' ||
			chr == '>' ||
			chr == '^';
	}

	//Brackets are: ( ) [ ] { } < >
	bool isBracket(const char chr) {
		return chr == '(' ||
			chr == ')' ||
			chr == '[' ||
			chr == ']' ||
			chr == '{' ||
			chr == '}' ||
			chr == '<' ||
			chr == '>';
	}

	bool isStrPalindrome(const string & str) {
		for (unsigned short i = 0; i < str.length() / 2; i++)
			if (str[i] != str[str.length() - i - 1])
				return false;

		return true;
	}

	string capAllLtrs(string str) {
		for (char & c : str)
			c = toupper(c);
		return str;
	}

	string lowAllLtrs(string str) {
		for (char & c : str)
			c = tolower(c);
		return str;
	}

	string capAllTkns(string str, const char del = ' ') {
		for (char & c : str)
			if (c != del)
				c = toupper(c);

		return str;
	}

	string lowAllTkns(string str, const char del = ' ') {
		for (char & c : str)
			if (c != del)
				c = tolower(c);

		return str;
	}

	string capTknFstLtr(string str, const char del = ' ') {
		bool isFirstChr = true;

		for (char & c : str) {
			if (c != del && isFirstChr)
				c = toupper(c);

			isFirstChr = c == del;
		}

		return str;
	}

	string lowTknFstLtr(string str, const char del = ' ') {
		bool isFirstChar = true;

		for (char & c : str) {
			if (c != del && isFirstChar)
				c = tolower(c);

			isFirstChar = c == del;
		}

		return str;
	}

	string revStr(string str) {
		string output = "";

		for (short i = str.length() - 1; i > -1; i--)
			output += str[i];

		return output;
	}

	char invChrCase(char c) {
		c = isupper(c) ? tolower(c) : toupper(c);
		return c;
	}

	string invStrCase(string str) {
		for (char & c : str)
			c = invChrCase(c);
		return str;
	}

	unsigned short getVowCnt(string & str) {
		unsigned short cnt = 0;

		for (char & chr : str)
			if (isVow(chr))
				cnt++;

		return cnt;
	}
	
	unsigned short getCapLtrCnt(const string & str) {
		unsigned short cnt = 0;

		for (char c : str)
			if (isupper(c))
				cnt++;

		return cnt;
	}

	unsigned short getSmlLtrCnt(const string & str) {
		unsigned short cnt = 0;

		for (char c : str)
			if (islower(c))
				cnt++;

		return cnt;
	}

	unsigned short getLtrCnt(const string & str) {
		unsigned short cnt = 0;

		for (char c : str)
			if (isLtr(c))
				cnt++;

		return cnt;
	}

	unsigned short getNonLtrCnt(const string & str) {
		unsigned short cnt = 0;

		for (char c : str)
			if (!isLtr(c))
				cnt++;

		return cnt;
	}

	unsigned short getSpcCnt(const string & str) {
		unsigned short cnt = 0;

		for (char c : str)
			if (c == ' ')
				cnt++;

		return cnt;
	}

	unsigned short getChrCnt(const string & str, const char & chr, bool matchCase = true) {
		unsigned short cnt = 0;
		
		if (matchCase) {
			for (char crrCharacter : str)
				if (crrCharacter == chr)
					cnt++;
		} else {
			for (char crrCharacter : str)
				if (crrCharacter == chr || tolower(crrCharacter) == tolower(chr))
					cnt++;
		}

		return cnt;
	}

	unsigned short getDigCnt(string & str) {
		unsigned short cnt = 0;

		for (char & c : str)
			if (isdigit(c))
				cnt++;
		
		return cnt;
	}

	unsigned short getNumOfTkns(string str, const char del = ' ') {
		str += del;
		unsigned short numOfWrds = 0;

		for (unsigned short i = 0; i < str.length(); i++) {
			if (str[i] != del && str[i + 1] == del)
				numOfWrds++;
		}

		return numOfWrds;
	}

	unsigned short getNumOfTkns(string str, const string del) {
		unsigned short numOfTkns = 0, delPos = 0;

		while ((delPos = str.find(del)) != (unsigned short) string::npos) {
			if (str.substr(0, delPos) != "")
				numOfTkns++;

			str.erase(0, delPos + del.length());
		}

		if (str != "")
			numOfTkns++;

		return numOfTkns;
	}

	vector<string> getVctOfTkns(string str, const char del = ' ') {
		str += del;
		vector<string> vctOfWrds;
		string wrd = "";

		for (unsigned short i = 0; i < str.length(); i++) {
			if (str[i] != del) {
				wrd += str[i];

				if (str[i + 1] == del) {
					vctOfWrds.push_back(wrd);
					wrd = "";
				}
			}
		}

		return vctOfWrds;
	}

	vector<string> getVctOfTkns(string str, const string del) {
		vector<string> vctOfTkns;
		unsigned short delPos = 0;
		string wrd = "";

		while ((delPos = str.find(del)) != (unsigned short) string::npos) {
			wrd = str.substr(0, delPos);

			if (wrd != "")
				vctOfTkns.push_back(wrd);

			str.erase(0, delPos + del.length());
		}

		if (str != "")
			vctOfTkns.push_back(str);

		return vctOfTkns;
	}

	string revTknsSeq(string seq, const char del = ' ') {
		vector<string> vctWrds = getVctOfTkns(seq, del);
		seq = "";

		for (short i = vctWrds.size() - 1; i >=0; i--)
			seq += (vctWrds.at(i) + del);

		seq.pop_back();
		return seq;
	}

	string trmRgt(string str, const char chrToRemove = ' ') {
		short pos = str.length() - 1;

		for (pos; pos >= 0; pos--)
			if (str[pos] != chrToRemove)
				break;

		str.erase(pos + 1, str.length() - pos - 1);
		return str;
	}

	string trmLft(string str, const char chrToRemove = ' ') {
		short pos = 0;

		for (pos; pos < str.length(); pos++)
			if (str[pos] != chrToRemove)
				break;
			
		str.erase(0, pos);

		return str;
	}

	string trm(string str, const char chrToRemove = ' ') {
		return trmRgt(trmLft(str, chrToRemove), chrToRemove);
	}

	string remDigs(string & str) {
		string res = "";

		for (char & chr : str)
			if (!isdigit(chr))
				res += chr;

		return res;
	}

	string keepDigs(string & str) {
		string res = "";

		for (char & chr : str)
			if (isdigit(chr))
				res += chr;

		return res;
	}
	
	string remLtrs(string & str) {
		string res = "";

		for (char & chr : str)
			if (!isLtr(chr))
				res += chr;

		return res;
	}

	string keepLtrs(string & str, const bool keepSpcs = false) {
		string res = "";

		for (char & chr : str)
			if (isLtr(chr) || (keepSpcs && chr == ' '))
				res += chr;
		
		return res;
	}

	//Specials are: @ # $ % ^ & ~ ` _ \ |
	string remSpecials(string & str) {
		string res = "";

		for (char & chr : str)
			if (!isSpecial(chr))
				res += chr;

		return res;
	}

	//Specials are: @ # $ % ^ & * ~ ` _ \ |
	string keepSpecials(string & str) {
		string res = "";

		for (char & chr : str)
			if (isSpecial(chr))
				res += chr;

		return res;
	}

	//Punctuations are: . , ; : ? ! " '
	string remPuncs(string & str) {
		string res = "";

		for (char & chr : str)
			if (!isPunc(chr))
				res += chr;

		return res;
	}

	//Punctuations are: . , ; : ? ! " '
	string keepPuncs(string & str) {
		string res = "";

		for (char & chr : str)
			if (isPunc(chr))
				res += chr;

		return res;
	}

	//Operations are: + - * / × ÷ = < > ^
	string remOpers(string & str) {
		string res = "";

		for (char & chr : str)
			if (!isOper(chr))
				res += chr;

		return res;
	}

	//Operations are: + - * / × ÷ = < > ^
	string keepOpers(string & str) {
		string res = "";

		for (char & chr : str)
			if (isOper(chr))
				res += chr;

		return res;
	}

	//Brackets are: ( ) [ ] { } < >
	string remBrackets(string & str) {
		string res = "";

		for (char & chr : str)
			if (!isBracket(chr))
				res += chr;

		return res;
	}

	//Brackets are: ( ) [ ] { } < >
	string keepBrackets(string & str) {
		string res = "";

		for (char & chr : str)
			if (isBracket(chr))
				res += chr;

		return res;
	}

	string remChr(string & str, const char chr) {
		string res = "";

		for (char & crrChr : str)
			if (crrChr != chr)
				res += crrChr;

		return res;
	}

	string keepChr(string & str, const char chr) {
		string res = "";

		for (char & crrChr : str)
			if (crrChr == chr)
				res += crrChr;

		return res;
	}

	string mrgVct(vector<string> & vct, const char delimiter = ',') {
		string result = "";

		for (string & crrStr : vct)
			result += (crrStr + delimiter);
		
		result.pop_back();
		return result;
	}
	
	string mrgVct(vector<string> & vct, const string delimiter) {
		string result = "";

		for (string & crrStr : vct)
			result += (crrStr + delimiter);
		
		return result.substr(0, result.length() - delimiter.length());
	}

	string mrgArr(const string arr[], const short & arrLng, const char delimiter = ',') {
		string result = "";

		for (short i = 0; i < arrLng; i++)
			result += (arr[i] + delimiter);

		result.pop_back();
		return result;
	}

	string mrgArr(const string arr[], const short & arrLng, const string delimiter) {
		string result = "";

		for (short i = 0; i < arrLng; i++)
			result += (arr[i] + delimiter);

		return result.substr(0, result.length() - delimiter.length());
	}

	string replaceAll(string str, string replaceFrom, const string replaceTo, const bool matchCase = true) {
		unsigned short pos = 0;

		if (matchCase)
			while ((pos = str.find(replaceFrom, pos)) != (unsigned short) string::npos) {
				str.replace(pos, replaceFrom.length(), replaceTo);

				pos += replaceTo.length();
			}
		else {
			string str_low = lowAllLtrs(str);
			string replaceFrom_low = lowAllLtrs(replaceFrom);

			while ((pos = str_low.find(replaceFrom_low, pos)) != (unsigned short) string::npos) {
				str.replace(pos, replaceFrom.length(), replaceTo);
				str_low.replace(pos, replaceFrom.length(), replaceTo);

				pos += replaceTo.length();
			}
		}

		return str;
	}
			  
	string replaceCnt(string str, const string replaceFrom, const string replaceTo, const unsigned short count, const bool matchCase = true) {
		unsigned short pos = 0, counter = 0;

		if (matchCase)
			while ((pos = str.find(replaceFrom, pos)) != (unsigned short) string::npos && counter < count) {
				str.replace(pos, replaceFrom.length(), replaceTo);

				pos += replaceTo.length();
				counter++;
			} else {
				string str_low = lowAllLtrs(str);
				string replaceFrom_low = lowAllLtrs(replaceFrom);

				while ((pos = str_low.find(replaceFrom_low, pos)) != (unsigned short) string::npos && counter < count) {
					str.replace(pos, replaceFrom.length(), replaceTo);
					str_low.replace(pos, replaceFrom.length(), replaceTo);

					pos += replaceTo.length();
					counter++;
				}
			}

		return str;
	}
			  
	string replaceAllTkns(string str, string replaceFrom, const string replaceTo, const string del = " ", const bool matchCase = true) {
		vector<string> vTkns;
		
		if (del == " ")
			vTkns = getVctOfTkns(str);
		else
			vTkns = getVctOfTkns(str, del);

		if (matchCase) {
			for (string & crrTkn : vTkns)
				if (crrTkn == replaceFrom)
					crrTkn = replaceTo;
		} else {
			for (string & crrWrd : vTkns)
				if (lowAllLtrs(crrWrd) == lowAllLtrs(replaceFrom))
					crrWrd = replaceTo;
		}

		return mrgVct(vTkns, del);
	}

	//This function deals with decimal and negative values
	string convertNumToTxt(double num, const short & roundPrecision = 0) {
		string result = "";
		
		if (num < 0) {
			result += "Negative ";
			num *= -1;
		}

		num = strings_utilities::specialRound(num, roundPrecision);

		unsigned int integerPart = static_cast<unsigned int>(num);
		unsigned int decimalPart = static_cast<unsigned int>(round((num - integerPart) * pow(10, roundPrecision)));
		
		if (integerPart == 0)
			result += "Zero";
		else
			result += strings_utilities::numToTxtCore(integerPart);

		if (decimalPart != 0)
			result += " Point " + strings_utilities::numToTxtCore(decimalPart);
			
		return result;
	}

}

