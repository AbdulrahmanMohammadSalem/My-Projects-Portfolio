// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "SystemUser.h"

class AddNewUserScreen : private Screen {
private:
	//----Helper Method:
	static void _readUserInfo(SystemUser & U) {
		cout << "\nAdding new User:";
		cout << "\n-------------------------------\n";
		U.setFirstName(readers::readString("Enter first name : ", true));
		U.setLastName(readers::readString ("Enter last name  : "));
		U.setPassword(readers::readWord   ("Enter password   : "));
		U.setEmail(readers::readWord      ("Enter email      : "));
		U.setPhone(readers::readWord      ("Enter phone      : "));

		cout << '\n';

		U.setPermissions(_readUserPermissions());
		
		cout << "-------------------------------" << endl;
	}

public:
	//----Main Method:
	static void displayNewUserScreen() {
		_printScreenHeader(40, "Add New User Screen");

		SystemUser newUser = SystemUser::intializeNewUser(_readNonExistingUsername());

		_readUserInfo(newUser);

		SystemUser::enSaveResults saveResult = newUser.saveInFile(false);

		switch (saveResult) {
			case SystemUser::enSaveResults::eSuccess:
				cout << '\n' << setw(50) << "" << "User added successfully!";
				break;

			case SystemUser::enSaveResults::eErrorEmptyUser:
				cerr << "\nError: Cannot save an empty User in file [displayNewUserScreen()]";
				break;

			case SystemUser::enSaveResults::eErrorUserAlreadyExists:
				cerr << "\nError: This User already exists in file [displayNewUserScreen()]";

			default:
				cerr << "\nUnexpected error happened.";
		}
	}

};

