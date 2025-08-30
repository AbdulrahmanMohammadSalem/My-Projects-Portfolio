// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class UpdateUserScreen : private Screen {
private:
	//----Helper Method:
	static void _readNewUserInfo(SystemUser & U) {
		cout << "\nUpdating User Info : ";
		cout << "\n-------------------------------\n";
		string userInput;

		//Reading first name:
		userInput = readers::readString("Enter first name : ", true);
		if (userInput != "-99")
			U.setFirstName(userInput);

		//Reading last name:
		userInput = readers::readString("Enter last name  : ");
		if (userInput != "-99")
			U.setLastName(userInput);

		//Reading email:
		userInput = readers::readWord("Enter email      : ");
		if (userInput != "-99")
			U.setEmail(userInput);

		//Reading phone:
		userInput = readers::readWord("Enter phone      : ");
		if (userInput != "-99")
			U.setPhone(userInput);

		//Reading pin code:
		userInput = readers::readWord("Enter Password   : ");
		if (userInput != "-99")
			U.setPassword(userInput);

		cout << '\n';

		//Reading Permissions:
		if (readers::readBoolean("Do you want to update the user's permissions? [Y] - [N] -> ")) {
			cout << '\n';
			U.setPermissions(_readUserPermissions());
		}

		cout << "-------------------------------" << endl;
	}

public:
	//----Main Method:
	static void displayUpdateUserScreen() {
		_printScreenHeader(40, "Update User Screen", "You can enter [-99] to skip a field");

		SystemUser _user = _readExistingUserByUsername();

		_printUserInfo(_user);

		if (readers::readBoolean("\nAre you sure you want to update this User? [Y] - [N] -> ")) {
			_readNewUserInfo(_user);

			SystemUser::enSaveResults saveResult = _user.saveInFile(false);

			switch (saveResult) {
				case SystemUser::enSaveResults::eErrorEmptyUser:
					cerr << "Error: The User is empty.";
					break;

				case SystemUser::enSaveResults::eSuccess:
					if (_user.getUsername() == ActiveUser.getUsername())
						ActiveUser = _user;

					cout << '\n' << setw(46) << left << " " << "User updated successfully!";
			}
		}


	}

};

