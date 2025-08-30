// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class DepositScreen : private Screen {
public:
	//----Main Method:
	static void displayDepositScreen() {
		_printScreenHeader(40, "Deposit Screen");

		BankClient client = _readExistingClientByAccountNumber();

		_printClientInfo(client);

		float amount = readers::readPositiveNumber<float>("\nEnter the amount to deposit:\n", "Invalid input, try again:\n");

		cout << '\n';

		if (readers::readBoolean("Are you sure you want to perform this transactions? [Y] - [N] -> ")) {
			client.deposit(amount, false);

			cout << '\n' << setw(44) << "" << "Done successfully! New balance is: " << client.getBalance();
		}
	}

};

