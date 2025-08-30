// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Strings.h"
using namespace strings;

class StringHandler {
private:
	string _value;

public:
	//Setup as an Object:
	StringHandler() {
		_value = "";
	}
	StringHandler(const string value) {
		_value = value;
	}

	string getValue() {
		return _value;
	}
	void setValue(const string newValue) {
		_value = newValue;
	}

	__declspec(property(get = getValue, put = setValue)) string Value;

	//Main Methods:
	bool equals(const string str) {
		return _value == str;
	}

	static string upperizeAll(string str) {
		return capAllLtrs(str);
	}
	void upperizeAll() {
		_value = capAllLtrs(_value);
	}

	static string lowerizeAll(string str) {
		return lowAllLtrs(str);
	}
	void lowerizeAll() {
		_value = lowAllLtrs(_value);
	}

	static string upperizeTokens(string str, const char separator = ' ') {
		return capAllTkns(str, separator);
	}
	void upperizeTokens(const char separator = ' ') {
		_value = capAllTkns(_value, separator);
	}

	static string lowerizeTokens(string str, const char separator = ' ') {
		return lowAllTkns(str, separator);
	}
	void lowerizeTokens(const char separator = ' ') {
		_value = lowAllTkns(_value, separator);
	}

	static string upperizeFirstLetters(string str, const char separator = ' ') {
		return capTknFstLtr(str, separator);
	}
	void upperizeFirstLetters(const char separator = ' ') {
		_value = capTknFstLtr(_value, separator);
	}

	static string lowerizeFirstLetters(string str, const char separator = ' ') {
		return lowTknFstLtr(str, separator);
	}
	void lowerizeFirstLetters(const char separator = ' ') {
		_value = lowTknFstLtr(_value, separator);
	}

	static string reverse(string str) {
		return revStr(str);
	}
	void reverse() {
		_value = revStr(_value);
	}

	static string reverseSequence(string sequence, const char separator = ' ') {
		return revTknsSeq(sequence, separator);
	}
	void reverseSequence(const char separator = ' ') {
		_value = revTknsSeq(_value, separator);
	}

	static string invertCase(string str) {
		return invStrCase(str);
	}
	void invertCase() {
		_value = invStrCase(_value);
	}

	static bool isVowel(const char letter) {
		return isVow(letter);
	}

	static bool isLetter(const char character) {
		return isLtr(character);
	}

	//Specials are: @ # $ % ^ & ~ ` _ \ | 
	static bool isSpecial(const char character) {
		return isSpecial(character);
	}

	//Punctuations are: . , ; : ? ! ' "
	static bool isPunctuation(const char character) {
		return isPunc(character);
	}

	//Operations are: + - * /     = < > ^
	static bool isOperation(const char character) {
		return isOper(character);
	}

	//Brackets are: ( ) [ ] { } < >
	static bool isBracket(const char character) {
		return strings::isBracket(character);
	}

	static bool isStringPalindrome(const string & str) {
		return isStrPalindrome(str);
	}
	bool isPalindrome() {
		return isStrPalindrome(_value);
	}

	static unsigned short getVowelCount(string & str) {
		return getVowCnt(str);
	}
	unsigned short getVowelCount() {
		return getVowCnt(_value);
	}

	static unsigned short getUpperCount(const string & str) {
		return getCapLtrCnt(str);
	}
	unsigned short getUpperCount() {
		return getCapLtrCnt(_value);
	}
	
	static unsigned short getLowerCount(const string & str) {
		return getSmlLtrCnt(str);
	}
	unsigned short getLowerCount() {
		return getSmlLtrCnt(_value);
	}

	static unsigned short getLetterCount(const string & str) {
		return getLtrCnt(str);
	}
	unsigned short getLetterCount() {
		return getLtrCnt(_value);
	}

	static unsigned short getNonLetterCount(const string & str) {
		return getNonLtrCnt(str);
	}
	unsigned short getNonLetterCount() {
		return getNonLtrCnt(_value);
	}

	static unsigned short getSpacesCount(const string & str) {
		return getSpcCnt(str);
	}
	unsigned short getSpacesCount() {
		return getSpcCnt(_value);
	}

	static unsigned short getCharacterCount(const string & str, const char & character, bool matchCase = true) {
		return getChrCnt(str, character, matchCase);
	}
	unsigned short getCharacterCount(const char & character, bool matchCase = true) {
		return getChrCnt(_value, character, matchCase);
	}

	static unsigned short getDigitCount(string & str) {
		return getDigCnt(str);
	}
	unsigned short getDigitCount() {
		return getDigCnt(_value);
	}

	static unsigned short getTokensCount(string str, const char separator = ' ') {
		return getNumOfTkns(str, separator);
	}
	unsigned short getTokensCount(const char separator = ' ') {
		return getNumOfTkns(_value, separator);
	}

	static unsigned short getTokensCount(string str, const string separator) {
		return getNumOfTkns(str, separator);
	}
	unsigned short getTokensCount(const string separator) {
		return getNumOfTkns(_value, separator);
	}

	static vector<string> getVectorOfTokens(string str, const char separator = ' ') {
		return getVctOfTkns(str, separator);
	}
	vector<string> getVectorOfTokens(const char separator = ' ') {
		return getVctOfTkns(_value, separator);
	}

	static vector<string> getVectorOfTokens(string str, const string separator) {
		return getVctOfTkns(str, separator);
	}
	vector<string> getVectorOfTokens(const string separator) {
		return getVctOfTkns(_value, separator);
	}

	static string trimRight(string str, const char charactorToErase = ' ') {
		return trmRgt(str, charactorToErase);
	}
	void trimRight(const char characterToErase) {
		_value = trmRgt(_value, characterToErase);
	}

	static string trimLeft(string str, const char charactorToErase = ' ') {
		return trmLft(str, charactorToErase);
	}
	void trimLeft(const char characterToErase) {
		_value = trmLft(_value, characterToErase);
	}
	
	static string trim(string str, const char charactorToErase = ' ') {
		return trm(str, charactorToErase);
	}
	void trim(const char characterToErase) {
		_value = trm(_value, characterToErase);
	}

	static string removeDigits(string & str) {
		return remDigs(str);
	}
	void removeDigits() {
		_value = remDigs(_value);
	}

	static string keepDigits(string & str) {
		return keepDigs(str);
	}
	void keepDigits() {
		_value = keepDigs(_value);
	}
	
	static string removeLetters(string & str) {
		return remLtrs(str);
	}
	void removeLetters() {
		_value = remLtrs(_value);
	}

	static string keepLetters(string & str) {
		return keepLtrs(str);
	}
	void keepLetters() {
		_value = keepLtrs(_value);
	}

	//Specials are: @ # $ % ^ & ~ ` _ \ |
	static string removeSpecials(string & str) {
		return remSpecials(str);
	}
	void removeSpecials() {
		_value = remSpecials(_value);
	}

	//Specials are: @ # $ % ^ & ~ ` _ \ |
	static string keepSpecials(string & str) {
		return keepSpecials(str);
	}
	void keepSpecials() {
		_value = keepSpecials(_value);
	}

	//Punctuations are: . , ; : ? ! " '
	static string removePunctuations(string & str) {
		return remPuncs(str);
	}
	void removePunctuations() {
		_value = remPuncs(_value);
	}

	//Punctuations are: . , ; : ? ! " '
	static string keepPunctuations(string & str) {
		return keepPuncs(str);
	}
	void keepPunctuations() {
		_value = keepPuncs(_value);
	}

	//Operations are: + - * /     = < > ^
	static string removeOperations(string & str) {
		return remOpers(str);
	}
	void removeOperations() {
		_value = remOpers(_value);
	}

	//Operations are: + - * /     = < > ^
	static string keepOperations(string & str) {
		return keepOpers(str);
	}
	void keepOperations() {
		_value = keepOpers(_value);
	}

	//Brackets are: ( ) [ ] { } < >
	static string removeBrackets(string & str) {
		return remBrackets(str);
	}
	void removeBrackets() {
		_value = remBrackets(_value);
	}

	//Brackets are: ( ) [ ] { } < >
	static string keepBrackets(string & str) {
		return strings::keepBrackets(str);
	}
	void keepBrackets() {
		_value = strings::keepBrackets(_value);
	}

	static string removeCharacter(string & str, const char character) {
		return remChr(str, character);
	}
	void removeCharacter(const char character) {
		_value = remChr(_value, character);
	}

	static string keepCharacter(string & str, const char character) {
		return keepChr(str, character);
	}
	void keepCharacter(const char character) {
		_value = keepChr(_value, character);
	}

	static string replaceAll(string str, string replaceFrom, const string replaceTo, const bool matchCase = true) {
		return strings::replaceAll(str, replaceFrom, replaceTo, matchCase);
	}
	void replaceAll(string replaceFrom, const string replaceTo, const bool matchCase = true) {
		_value = strings::replaceAll(_value, replaceFrom, replaceTo, matchCase);
	}
	
	static string replaceCount(string str, const string replaceFrom, const string replaceTo, const unsigned short count, const bool matchCase = true) {
		return replaceCnt(str, replaceFrom, replaceTo, count, matchCase);
	}
	void replaceCount(const string replaceFrom, const string replaceTo, const unsigned short count, const bool matchCase = true) {
		_value = replaceCnt(_value, replaceFrom, replaceTo, count, matchCase);
	}

	static string replaceTkns(string str, string replaceFrom, const string replaceTo, const string separator = " ", const bool matchCase = true) {
		return replaceAllTkns(str, replaceFrom, replaceTo, separator, matchCase);
	}
	void replaceTkns(string replaceFrom, const string replaceTo, const string separator = " ", const bool matchCase = true) {
		_value = replaceAllTkns(_value, replaceFrom, replaceTo, separator, matchCase);
	}

	static string mergeToString(vector<string> & vct, const char separator = ',') {
		return mrgVct(vct, separator);
	}

	static string mergeToString(vector<string> & vct, const string separator) {
		return mrgVct(vct, separator);
	}

	static string mergeToString(const string array[], const short & arrayLength, const char separator = ',') {
		return mrgArr(array, arrayLength, separator);
	}

	static string mergeToString(const string array[], const short & arrayLength, const string separator) {
		return mrgArr(array, arrayLength, separator);
	}

	static string convertNumberToText(unsigned long long number) {
		return convertNumToTxt(number);
	}

};
