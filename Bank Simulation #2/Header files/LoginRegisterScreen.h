// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "SystemUser.h"

class LoginRegisterScreen : private Screen {
private:
	static void _printLoginRegisterTableHeader() {
		cout << setw(16) << "" << Utils::Mesc::getCharacterString('-', 86) << '\n';
		cout << setw(16) << ""; //Outer indent

		cout << setw(27) << left << "| Date/Time";
		cout << setw(20) << left << "| Username";
		cout << setw(18) << left << "| Password";
		cout << setw(20) << left << "| Permissions";

		cout << "|\n";
		cout << setw(16) << "" << Utils::Mesc::getCharacterString('-', 86) << '\n';
	}

	static void _printLoginRegisterRow(const SystemUser::stLoginEntry & E) {
		cout << setw(27) << left << ("| " + E._dateTimeText);
		cout << setw(20) << left << ("| " + E._username);
		cout << setw(18) << left << ("| " + E._password);
		cout << setw(20) << left << ("| " + to_string(E._permissions));
	}

	static void _printLoginRegisterTableRows(const vector<SystemUser::stLoginEntry> & _vLoginEntries) {
		for (const SystemUser::stLoginEntry & E : _vLoginEntries) {
			cout << setw(16) << ""; //Outer indent

			_printLoginRegisterRow(E);
			
			cout << "|\n";
		}
	}

	static void _printLoginRegisterTable(const vector<SystemUser::stLoginEntry> & _vLoginEntries) {
		_printLoginRegisterTableHeader();

		_printLoginRegisterTableRows(_vLoginEntries);

		cout << setw(16) << "" << Utils::Mesc::getCharacterString('-', 86) << '\n';
	}

public:
	static void displayLoginRegisterScreen() {
		if (!_checkScreenAccessibility(SystemUser::enPermissions::pLoginRegister))
			return;

		const vector<SystemUser::stLoginEntry> _vLoginEntries = SystemUser::getLoginEntries();

		_printScreenHeader(40, "Login Register Screen", to_string(_vLoginEntries.size()) + " Entries available.");

		_printLoginRegisterTable(_vLoginEntries);
	}
};




