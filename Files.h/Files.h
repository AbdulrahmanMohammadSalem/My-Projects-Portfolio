// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

/* Abbreviations used in this file are:
* prt = print
* crr = current
* clr = clear
* src = source
* vct = vector
* ln = line
* del = delete, deletion
* pos = position, index
* num = number

*/

#pragma once

#include "Utils.h"
#include <string>
#include <fstream>

using namespace std;

namespace files {
	// If any of these functions succeeds, it return true. Otherwise, returns false.

	bool prtFile(const string & path) {
		fstream file(path, ios::in); //read mode

		if (file.is_open()) {
			string crrLn = "";

			while (getline(file, crrLn))
				cout << crrLn << '\n';

			file.close();
			return true;
		}

		return false;
	}

	bool clrFile(const string & path) {
		fstream file(path, ios::out); //write mode

		if (file.is_open()) {
			file.close();
			return true;
		}

		return false;
	}

	bool copyFileToFile(const string & srcPath, const string & destinationPath, const bool override = false) {
		fstream src(srcPath, ios::in); //read mode
		fstream destination(destinationPath, override ? ios::out : ios::app); //write / append mode

		if (src.is_open() && destination.is_open()) {
			string crrLn = "";

			while (getline(src, crrLn))
				destination << crrLn << '\n';

			src.close();
			destination.close();
			
			return true;
		}

		return false;
	}

	bool moveFileToFile(const string & srcPath, const string & destinationPath, const bool override = false) {
		return copyFileToFile(srcPath, destinationPath, override) && clrFile(srcPath);
	}

	bool copyFileToVct(const string & srcPath, vector<string> & vct, bool override = false) {
		fstream file(srcPath, ios::in); //read mode
		
		if (file.is_open()) {
			if (override)
				vct.clear();

			string crrLn = "";

			while (getline(file, crrLn))
				vct.push_back(crrLn);
		
			file.close();

			return true;
		}

		return false;
	}

	bool moveFileToVct(const string & srcPath, vector<string> & vct, bool override = false) {
		return copyFileToVct(srcPath, vct, override) && clrFile(srcPath);
	}

	vector<string> getVectorFromFile(const string & srcPath, const bool clearSrc = false) {
		vector<string> vct;
		
		if (clearSrc)
			moveFileToVct(srcPath, vct);
		else
			copyFileToVct(srcPath, vct);
			
		return vct;
	}

	bool copyVctToFile(const vector<string> & src, const string & destinationPath, const bool override = false, const bool copyEmptyLns = true) {
		fstream destination(destinationPath, override ? ios::out : ios::app); //write / append mode

		if (destination.is_open()) {
			for (const string & crrLn : src)
				if (copyEmptyLns || crrLn != "")
					destination << crrLn << '\n';

			destination.close();

			return true;
		}

		return false;
	}

	bool moveVctToFile(vector<string> & src, const string & destinationPath, bool override = false) {
		bool result = copyVctToFile(src, destinationPath, override);
		src.clear();
		return result;
	}

	bool delLnFromFile(const string & path, const size_t & lnPos) {
		vector<string> vct;
		bool result = copyFileToVct(path, vct);

		vct.at(lnPos) = "";

		return copyVctToFile(vct, path, true) && result;
	}

	bool delLnFromFile(const string & path, const string & ln) {
		vector<string> vct;
		bool result = copyFileToVct(path, vct);

		for (string & vctLn : vct)
			if (vctLn == ln)
				vctLn = "";


		return copyVctToFile(vct, path, true) && result;
	}

	bool delStrFromFile(const string & path, const string & str) {
		vector<string> vct;
		bool result = copyFileToVct(path, vct);
		size_t strPos = 0;

		for (string & ln : vct) {
			strPos = ln.find(str);
			while (strPos != std::string::npos) {
				ln.erase(strPos, str.length());
				strPos = ln.find(str);
			}
		}
		
		return copyVctToFile(vct, path, true) && result;
	}

	bool delStrFromFile(const string & path, const string & str, const size_t & count) {
		vector<string> vct;
		bool result = copyFileToVct(path, vct);
		size_t strPos = 0, numOfDels = 0;

		for (string & ln : vct) {
			strPos = ln.find(str);

			while (strPos != std::string::npos && numOfDels < count) {
				ln.erase(strPos, str.length());
				numOfDels++;
				strPos = ln.find(str);
			}

			if (numOfDels == count)
				break;
		}
		
		return copyVctToFile(vct, path, true) && result;
	}

	bool appendLnInFile(const string & path, const string & ln) {
		fstream file(path, ios::app);

		if (file.is_open()) {
			file << ln << '\n';

			file.close();
			return true;
		}
		else
			cerr << "Error: Cannot open file: " << path << endl;

		return false;
	}

	bool insertLnInFile(const string & path, const size_t & pos, const string & ln) {
		vector<string> vLines;

		bool result = copyFileToVct(path, vLines);

		vLines.insert(vLines.begin() + pos, ln);

		return copyVctToFile(vLines, path, true) && result;
	}

	bool insertFileInFile(const string & srcPath, const string & destinationPath, const size_t & pos) {
		vector<string> src, destination;

		bool result = copyFileToVct(srcPath, src);
		result = result && copyFileToVct(destinationPath, destination);

		destination.insert(destination.begin() + pos, src.begin(), src.end());

		return copyVctToFile(destination, destinationPath, true) && result;
	}

	bool updateLnInFile(const string & path, const size_t & lnPos, const string & newLn) {
		vector<string> vct;
		bool result = copyFileToVct(path, vct);

		vct.at(lnPos) = newLn;

		return copyVctToFile(vct, path, true) && result;
	}

	bool updateLnInFile(const string & path, const string & ln, const string & newLn) {
		vector<string> vct;
		bool result = copyFileToVct(path, vct);

		for (string & vctLn : vct)
			if (vctLn == ln)
				vctLn = newLn;

		return copyVctToFile(vct, path, true) && result;
	}

	bool updateStrInFile(const string & path, const string & str, const string & newStr) {
		vector<string> vct;
		bool result = copyFileToVct(path, vct);
		size_t strPos = 0;

		for (string & ln : vct) {
			strPos = ln.find(str);
			while (strPos != std::string::npos) {
				ln.erase(strPos, str.length());
				ln.insert(strPos, newStr);
				strPos = ln.find(str);
			}
		}

		return copyVctToFile(vct, path, true) && result;
	}

	bool updateStrInFile(const string & path, const string & str, const string & newStr, const size_t & count) {
		vector<string> vct;
		bool result = copyFileToVct(path, vct);
		size_t strPos = 0, numOfDels = 0;

		for (string & ln : vct) {
			strPos = ln.find(str);

			while (strPos != std::string::npos && numOfDels < count) {
				ln.erase(strPos, str.length());
				ln.insert(strPos, newStr);
				numOfDels++;
				strPos = ln.find(str);
			}

			if (numOfDels == count)
				break;
		}
		
		return copyVctToFile(vct, path, true) && result;
	}

	bool isLnInFile(const string & path, const string & ln) {
		vector<string> vct;

		copyFileToVct(path, vct);

		for (string & vctLn : vct)
			if (vctLn == ln)
				return true;

		return false;
	}

	bool isStrInLn(const string & path, const size_t & lnPos, const string & str) {
		vector<string> vct;
		
		copyFileToVct(path, vct);

		return vct.at(lnPos).find(str) != str.npos;
	}

	bool isStrInFile(const string & path, const string & str) {
		vector<string> vct;

		copyFileToVct(path, vct);

		for (string & ln : vct)
			if (ln.find(str) != str.npos)
				return true;
		
		return false;
	}

	string getLnFromFile(const string & path, const size_t & pos) {
		vector<string> vct;

		copyFileToVct(path, vct);

		return vct.at(pos);
	}

	short getFirstLnPosInFile(const string & path, const string & ln) {
		vector<string> vct;

		copyFileToVct(path, vct);

		for (short i = 0; i < vct.size(); i++)
			if (vct.at(i) == ln)
				return i;

		return -1;
	}

	bool isFileEmpty(const string & path) {
		vector<string> vct;

		copyFileToVct(path, vct);

		return vct.empty();
	}

	bool encryptFile(const string & path, const string & key) {
		vector<string> vLines;

		bool result = copyFileToVct(path, vLines);

		for (string & L : vLines)
			Utils::Encrypt_Decrypt::encrypt(L, key);
		
		return copyVctToFile(vLines, path, true) && result;
	}

	bool decryptFile(const string & path, const string & key) {
		vector<string> vLines;

		bool result = copyFileToVct(path, vLines);

		for (string & L : vLines)
			Utils::Encrypt_Decrypt::decrypt(L, key);
		
		return copyVctToFile(vLines, path, true) && result;
	}

	bool encryptLn(const string & path, const string & ln, const string & key) {
		vector<string> vLines;

		bool result = copyFileToVct(path, vLines);

		for (string & L : vLines)
			if (L == ln) {
				L = Utils::Encrypt_Decrypt::encrypt(L, key);
				break;
			}

		return copyVctToFile(vLines, path, true) && result;
	}

	bool encryptLn(const string & path, const size_t & lnPos, const string & key) {
		vector<string> vLines;

		bool result = copyFileToVct(path, vLines);

		vLines.at(lnPos) = Utils::Encrypt_Decrypt::encrypt(vLines.at(lnPos), key);

		return copyVctToFile(vLines, path, true) && result;
	}
	
	bool decryptLn(const string & path, const string & ln, const string & key) {
		vector<string> vLines;

		bool result = copyFileToVct(path, vLines);

		for (string & L : vLines)
			if (L == ln) {
				L = Utils::Encrypt_Decrypt::decrypt(L, key);
				break;
			}

		return copyVctToFile(vLines, path, true) && result;
	}

	bool decryptLn(const string & path, const size_t & lnPos, const string & key) {
		vector<string> vLines;

		bool result = copyFileToVct(path, vLines);

		vLines.at(lnPos) = Utils::Encrypt_Decrypt::decrypt(vLines.at(lnPos), key);

		return copyVctToFile(vLines, path, true) && result;
	}
}

