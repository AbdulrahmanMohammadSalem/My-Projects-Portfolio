// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class TransferScreen : private Screen {
	static void _printShortClientInfo(const BankClient & C) {
		cout << "\nClient card:";
		cout << "\n-------------------------";
		cout << "\nFull name  : " << C.getFullName();
		cout << "\nAcc. Num.  : " << C.getAccountNumber();
		cout << "\nBalance    : " << C.getBalance();
		cout << "\n-------------------------\n";
	}

public:
	//Main Method:
	static void displayTransferScreen() {
		_printScreenHeader(44, "Transfer Screen");

		//----Reading input with validation:
		BankClient clientFrom;

		while (true) {
			clientFrom = _readExistingClientByAccountNumber("Please enter the account number to transfer from:\n");

			if (clientFrom.getBalance() == 0)
				cout << "\nThe balance of the chosen client is zero, choose another one.\n\n";
			else
				break;
		}

		_printShortClientInfo(clientFrom);

		cout << '\n';

		BankClient clientTo;

		while (true) {
			clientTo = _readExistingClientByAccountNumber("Please enter the account number to transfer to:\n");

			if (clientTo.getAccountNumber() == clientFrom.getAccountNumber())
				cout << "\nInvalid input, you already chose that client.\n\n";
			else
				break;
		}

		_printShortClientInfo(clientTo);

		cout << '\n';

		float amount = readers::readPositiveNumber<float>("Enter the amount to transfer:\n", "Invalid input, try again:\n");

		while (amount > clientFrom.getBalance())
				amount = readers::readPositiveNumber<float>("The amount exceeds the balance, try again:\n", "Invalid input, try again:\n");

		cout << '\n';

		if (readers::readBoolean("Confirm transaction? [Y] - [N] -> ")) {
			//----Upplying the transfer operation:
			bool _transferResult = clientFrom.transferTo(clientTo, amount, ActiveUser.getUsername(), false, false, false);

			//----Printing results:
			_printShortClientInfo(clientFrom);
			_printShortClientInfo(clientTo);

			if (_transferResult)
				cout << '\n' << setw(50) << "" << "Done Successfully!";
			else
				cerr << "\nError: Something went wrong [displayTransferScreen]";
		}
	}
};
