// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class FindUserScreen : private Screen {
public:
	//----Main Method:
	static void displayFindUserScreen() {
		_printScreenHeader(40, "Find User Screen");

		SystemUser _user = _readExistingUserByUsername();

		_printUserInfo(_user);
	}
};

