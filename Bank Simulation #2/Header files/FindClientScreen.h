// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class FindClientScreen : private Screen {
public:
	//----Main Method:
	static void displayFindClientScreen() {
		if (!_checkScreenAccessibility(SystemUser::enPermissions::pFindClient))
			return;

		_printScreenHeader(40, "Find Client Screen");

		BankClient _client = _readExistingClientByAccountNumber();

		_printClientInfo(_client);
	}

};

