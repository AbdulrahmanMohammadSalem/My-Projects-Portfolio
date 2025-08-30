// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Files.h"

using namespace files;

class FileHandler {
private:
	string _Path;
public:
	
	FileHandler(const string & path) {
		_Path = path;
	}

	string getPath() const {
		return _Path;
	}

	void setPath(const string & path) {
		_Path = path;
	}

	__declspec(property(get = getPath, put = setPath)) string Path;

	bool print() {
		return prtFile(_Path);
	}

	bool clear() {
		return clrFile(_Path);
	}

	//This keeps the file intact
	bool copyToFile(const string & path, const bool override = false) {
		return copyFileToFile(_Path, path, override);
	}

	//This keeps the source file intact
	bool copyFromFile(const string & path, const bool override = false) {
		return copyFileToFile(path, _Path, override);
	}

	//This clears the file
	bool moveToFile(const string & path, const bool override = false) {
		return moveFileToFile(_Path, path, override);
	}

	//This clears the source file
	bool moveFromFile(const string & path, const bool override = false) {
		return moveFileToFile(path, _Path, override);
	}

	//This keeps the file intact
	bool copyToVector(vector<string> & vct, const bool override) {
		return copyFileToVct(_Path, vct, override);
	}

	//This clears the file
	bool moveToVector(vector<string> & vct, const bool override) {
		return moveFileToVct(_Path, vct, override);
	}

	vector<string> toVector(const bool clearFile = false) {
		return getVectorFromFile(_Path, clearFile);
	}

	//This keeps the source vector intact
	bool copyFromVector(vector<string> & vct, const bool override = false) {
		return copyVctToFile(vct, _Path, override);
	}

	//This clears the source vector
	bool moveFromVector(vector<string> & vct, const bool override = false) {
		return moveVctToFile(vct, _Path, override);
	}

	bool deleteLine(const size_t & position) {
		return delLnFromFile(_Path, position);
	}

	bool deleteLine(const string & line) {
		return delLnFromFile(_Path, line);
	}

	bool deleteString(const string & str) {
		return delStrFromFile(_Path, str);
	}

	bool deleteString(const string & str, const size_t & count) {
		return delStrFromFile(_Path, str, count);
	}

	bool appendLine(const string & line) {
		return appendLnInFile(_Path, line);
	}

	bool insertLine(const size_t & position, const string & line) {
		return insertLnInFile(_Path, position, line);
	}

	bool insertThisInFile(const string & path, const size_t & position) {
		return insertFileInFile(_Path, path, position);
	}

	bool insertFileInThis(const string & path, const size_t & position) {
		return insertFileInFile(path, _Path, position);
	}

	bool updateLine(const size_t & position, const string & newLine) {
		return updateLnInFile(_Path, position, newLine);
	}

	bool updateLine(const string & line, const string & newLine) {
		return updateLnInFile(_Path, line, newLine);
	}

	bool updateString(const string & str, const string & newStr) {
		return updateStrInFile(_Path, str, newStr);
	}

	bool updateString(const string & str, const string & newStr, const size_t & count) {
		return updateStrInFile(_Path, str, newStr, count);
	}

	bool doesContainLine(const string & line) {
		return isLnInFile(_Path, line);
	}

	bool doesContainInLine(const size_t & lineposition, const string & str) {
		return isStrInLn(_Path, lineposition, str);
	}

	bool doesContainString(const string & str) {
		return isStrInFile(_Path, str);
	}

	string getLine(const size_t & position) {
		return getLnFromFile(_Path, position);
	}

	short getFirstLinePosition(const string & line) {
		return getFirstLnPosInFile(_Path, line);
	}

	bool isEmpty() {
		return isFileEmpty(_Path);
	}

	bool encrypt(const string & key) {
		return encryptFile(_Path, key);
	}

	bool decrypt(const string & key) {
		return decryptFile(_Path, key);
	}

	bool encryptLine(const string & line, const string & key) {
		return encryptLn(_Path, line, key);
	}

	bool decryptLine(const string & line, const string & key) {
		return decryptLn(_Path, line, key);
	}

	bool encryptLine(const size_t & position, const string & key) {
		return encryptLn(_Path, position, key);
	}

	bool decryptLine(const size_t & position, const string & key) {
		return decryptLn(_Path, position, key);
	}

};
