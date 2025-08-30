// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "BankClient.h"

class AddNewClientScreen : private Screen {
private:
	//----Helper Method:
	static void _readClientInfo(BankClient & C) {
		cout << "\nAdding new client:";
		cout << "\n-------------------------------\n";
		C.setFirstName(readers::readString("Enter first name : ", true));
		C.setLastName(readers::readString("Enter last name  : "));
		C.setEmail(readers::readString("Enter email      : "));
		C.setPhone(readers::readString("Enter phone      : "));
		C.setPinCode(readers::readString("Enter PIN code   : "));
		C.setBalance(readers::readPositiveNumber<float>("Enter balance    : "));
		cout << "-------------------------------" << endl;
	}

public:
	//----Main Method:
	static void displayNewClientScreen() {
		if (!_checkScreenAccessibility(SystemUser::enPermissions::pAddNewClient))
			return;

		_printScreenHeader(40, "Add New Client Screen");

		BankClient newClient = BankClient::intializeNewClient(_readNonExistingAccountNumber());

		_readClientInfo(newClient);

		BankClient::enSaveResults saveResult = newClient.saveInFile(false);

		switch (saveResult) {
			case BankClient::enSaveResults::eSuccess:
				cout << "\nClient added successfully!";
				break;

			case BankClient::enSaveResults::eErrorEmptyClient:
				cerr << "\nError: Cannot save an empty client in file [displayNewClientScreen()]";
				break;

			case BankClient::enSaveResults::eErrorClientAlreadyExists:
				cerr << "\nError: This client already exists in file [displayNewClientScreen()]";

			default:
				cerr << "\nUnexpected error happened.";
		}
	}

};

