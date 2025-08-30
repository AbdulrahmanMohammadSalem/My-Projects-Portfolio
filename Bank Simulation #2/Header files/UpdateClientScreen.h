// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class UpdateClientScreen : private Screen {
private:
	//----Helper Method:
	static void _readNewClientInfo(BankClient & C) {
		cout << "\nUpdating Client Info : ";
		cout << "\n-------------------------------\n";
		string userInput;

		//Reading first name:
		userInput = readers::readString("Enter first name : ", true);
		if (userInput != "-99")
			C.setFirstName(userInput);

		//Reading last name:
		userInput = readers::readString("Enter last name  : ");
		if (userInput != "-99")
			C.setLastName(userInput);

		//Reading email:
		userInput = readers::readWord("Enter email      : ");
		if (userInput != "-99")
			C.setEmail(userInput);

		//Reading phone:
		userInput = readers::readWord("Enter phone      : ");
		if (userInput != "-99")
			C.setPhone(userInput);

		//Reading pin code:
		userInput = readers::readWord("Enter PIN code   : ");
		if (userInput != "-99")
			C.setPinCode(userInput);

		//Reading balance:
		float balance = readers::readNumber<float>("Enter balance    : ");
		if (balance > 0)
			C.setBalance(balance);
		cout << "-------------------------------" << endl;
	}

public:
	//----Main Method:
	static void displayUpdateClientScreen() {
		if (!_checkScreenAccessibility(SystemUser::enPermissions::pUpdateClient))
			return;

		_printScreenHeader(40, "Update Client Screen", "You can enter [-99] to skip a field");

		BankClient client = _readExistingClientByAccountNumber();

		_printClientInfo(client);

		if (readers::readBoolean("\nAre you sure you want to update this client? [Y] - [N] -> ")) {
			_readNewClientInfo(client);

			BankClient::enSaveResults saveResult = client.saveInFile(false);

			switch (saveResult) {
				case BankClient::enSaveResults::eErrorEmptyClient:
					cerr << "Error: The client is empty.";
					break;

				case BankClient::enSaveResults::eSuccess:
					cout << '\n' << setw(46) << left << " " << "Client updated successfully!";
			}
		}
	}

};

