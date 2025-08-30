// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class DeleteUserScreen : private Screen {
public:
	//Main Method:
	static void displayDeleteUserScreen() {
		_printScreenHeader(40, "Delete User Screen");

		SystemUser UserToDelete = _readExistingUserByUsername();

		_printUserInfo(UserToDelete);

		if (readers::readBoolean("\nAre you sure you want to delete this User? [Y] - [N] -> ")) {
			UserToDelete.remove();

			cout << '\n' << setw(46) << "" << "User deleted successfully!";
		}
	}

};
