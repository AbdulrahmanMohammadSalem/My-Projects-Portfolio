// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include <iostream>
#include <string>

using namespace std;

namespace readers {
	template <typename Number>
	Number readNumber(string msg, const string errMsg = "") {
		Number num = 0;

		while (true) {
			cout << msg;
			cin >> num;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore(((numeric_limits<streamsize>::max))(), '\n');
			} else
				return num;
		}
	}
	
	template <typename Number>
	Number readNumber(string msg, const Number from, const Number to, const string errMsg = "") {
		Number num = 0;

		while (true) {
			cout << msg;
			cin >> num;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore(((numeric_limits<streamsize>::max))(), '\n');
			
			} else if (num >= from && num <= to)
				return num;
		}
	}

	template <typename Number>
	Number readPositiveNumber(string msg, const string errMsg = "") {
		Number num = 0;

		while (true) {
			cout << msg;
			cin >> num;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			} else if (num > 0)
				return num;
		}
	}

	template <typename Number>
	Number readNegativeNumber(string msg, const string errMsg = "") {
		Number num = 0;

		while (true) {
			cout << msg;
			cin >> num;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			} else if (num < 0)
				return num;
		}
	}

	template <typename Number>
	Number readNonPositveNumber(string msg, const string errMsg = "") {
		Number num = 0;

		while (true) {
			cout << msg;
			cin >> num;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			} else if (num <= 0)
				return num;
		}
	}

	template <typename Number>
	Number readNonNegativeNumber(string msg, const string errMsg = "") {
		Number num = 0;

		while (true) {
			cout << msg;
			cin >> num;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			} else if (num >= 0)
				return num;
		}
	}

	template <typename Number>
	Number readNonZeroNumber(string msg, const string errMsg = "") {
		Number num = 0;

		while (true) {
			cout << msg;
			cin >> num;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			} else if (num != 0)
				return num;
		}
	}

	string readWord(string msg, const string errMsg = "") {
		string wrd;

		while (true) {
			cout << msg;
			cin >> wrd;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.peek() != '\n')
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			else
				return wrd;
		}
	}

	string readString(const string msg, const bool isAfterCin = false) {
		string str = "";

		cout << msg;
		getline(isAfterCin ? cin >> ws : cin, str);

		return str;
	}

	char readCharacter(string msg, const string allowedChars = "", const string & errMsg = "") {
		char chr = ' ';

		while (true) {
			cout << msg;
			cin >> chr;

			if (errMsg != "" && msg != errMsg)
				msg = errMsg;

			if (cin.fail() || cin.peek() != '\n') {
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			} else if (allowedChars == "" || allowedChars.find(chr) != std::string::npos)
				return chr;
		}
	}

	bool readBoolean(string msg, const string errMsg = "") {
		return tolower(readCharacter(msg, "ynYN", errMsg)) == 'y';
	}

}
