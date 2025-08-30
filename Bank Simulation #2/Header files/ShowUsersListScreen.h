// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class ShowUsersListScreen : private Screen {
private:
	//----Helper Methods:
	static void _printUsersTableHeader() {
		cout << setw(8) << left << "" << "\n\t--------------------------------------------------------------------------------------------------------\n";
		cout << setw(8) << left << "" << "| " << left << setw(12) << "UserName";
		cout << "| " << left << setw(25) << "Full Name";
		cout << "| " << left << setw(12) << "Phone";
		cout << "| " << left << setw(20) << "Email";
		cout << "| " << left << setw(10) << "Password";
		cout << "| " << left << setw(12) << "Permissions |";
		cout << setw(8) << left << "" << "\n\t--------------------------------------------------------------------------------------------------------\n";
	}

	static void _printUserLine(const SystemUser & _user) {
		cout << setw(8) << left << "" << "| " << setw(12) << left << _user.getUsername();
		cout << "| " << setw(25) << left << _user.getFullName();
		cout << "| " << setw(12) << left << _user.getPhone();
		cout << "| " << setw(20) << left << _user.getEmail();
		cout << "| " << setw(10) << left << _user.getPassword();
		cout << "| " << setw(12) << left << _user.getPermissions() << "|\n";
	}

	static void _printUsersTable(const vector<SystemUser> & _vUsers) {
		_printUsersTableHeader();

		if (_vUsers.empty())
			cout << setw(49) << " " << "No Users available\n";
		else for (const SystemUser & U : _vUsers)
			_printUserLine(U);

		cout << setw(8) << "" << "--------------------------------------------------------------------------------------------------------\n" << endl;
	}

public:
	//----Main Method:
	static void displayUsersList() {
		const vector<SystemUser> _vUsers = SystemUser::getUsersList();

		_printScreenHeader(33, "Users List Screen", "(" + to_string(_vUsers.size()) + ") User(s) available in the system");

		_printUsersTable(_vUsers);
	}
	
};

