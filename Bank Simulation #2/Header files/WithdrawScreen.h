// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class WithdrawScreen : private Screen {
public:
	//----Main Method:
	static void displayWithdrawScreen() {
		_printScreenHeader(40, "Withdraw Screen");

		BankClient client = _readExistingClientByAccountNumber();

		_printClientInfo(client);

		if (client.getBalance() == 0)
			cout << "\nYou cannot withdraw.";
		else {
			float amount = readers::readPositiveNumber<float>("\nEnter the amount to withdraw:\n", "Invalid input, try again:\n");

			while (amount > client.getBalance())
				amount = readers::readPositiveNumber<float>("The amount exceeds the balance, try again:\n", "Invalid input, try again:\n");

			cout << '\n';

			if (readers::readBoolean("Are you sure you want to perform this transactions? [Y] - [N] -> ")) {
				client.withdraw(amount, false, false);

				cout << '\n' << setw(44) << "" << "Done successfully! New balance is: " << client.getBalance();
			}
		}
	}
};
