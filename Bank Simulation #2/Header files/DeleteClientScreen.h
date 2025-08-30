// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "BankClient.h"

class DeleteClientScreen : private Screen {
public:
	//Main Method:
	static void displayDeleteClientScreen() {
		if (!_checkScreenAccessibility(SystemUser::enPermissions::pDeleteClient))
			return;

		_printScreenHeader(40, "Delete Client Screen");

		BankClient clientToDelete = _readExistingClientByAccountNumber();

		_printClientInfo(clientToDelete);

		if (readers::readBoolean("\nAre you sure you want to delete this client? [Y] - [N] -> ")) {
			clientToDelete.remove();

			cout << '\n' << setw(46) << "" << "Client deleted successfully!";
		}
	}
};

