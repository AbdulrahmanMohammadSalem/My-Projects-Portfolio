// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "MainMenuScreen.h"
#include "Global.h"

class LoginScreen : private Screen {
private:
	//Helper Method:
	static bool _login() {
		string _username, _password;
		short failedTimes = 0;

		while (true) {
			_username = readers::readWord("Enter username:\n");
			_password = readers::readWord("Enter password:\n");

			if ((ActiveUser = SystemUser::findUser(_username, _password)).isEmpty()) {
				failedTimes++;

				if (failedTimes == 3) {
					cout << "\nYou are locked after 3 failed trials.\n\n";
					return false;
				}
				else {
					cout << "\nInvalid username/password.";
					cout << "\nYou have " << 3 - failedTimes << " trial(s) left.\n\n";
				}
			}
			else {
				//To update the login LoginRegister file:
				ActiveUser.registerLogin();

				MainMenuScreen::displayMainMenu();
				return true;
			}
		}
	}

public:
	//Main Method:
	static bool displayLoginScreen() {
		_printScreenHeader(45, "Login Screen");

		return _login();
	}
};

