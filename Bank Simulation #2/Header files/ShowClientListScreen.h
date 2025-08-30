// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class ShowClientListScreen : private Screen {
private:
	//----Helper Methods:
	static void _printClientsTableHeader() {
		cout << setw(8) << " " << "------------------------------------------------------------------------------------------------------\n";
		cout << setw(8) << " " << "| " << left << setw(15) << "Account Number";
		cout << "| " << left << setw(20) << "Client Name";
		cout << "| " << left << setw(12) << "Phone";
		cout << "| " << left << setw(20) << "Email";
		cout << "| " << left << setw(10) << "PIN Code";
		cout << "| " << left << setw(12) << "Balance" << "|\n";
		cout << setw(8) << " " << "------------------------------------------------------------------------------------------------------\n";
	}

	static void _printClientLine(const BankClient & _client) {
		cout << setw(8) << " " << "| " << setw(15) << left << _client.getAccountNumber();
		cout << "| " << setw(20) << left << _client.getFullName();
		cout << "| " << setw(12) << left << _client.getPhone();
		cout << "| " << setw(20) << left << _client.getEmail();
		cout << "| " << setw(10) << left << _client.getPinCode();
		cout << "| " << setw(12) << left << _client.getBalance() << "|\n";
	}

	static void _printClientsTable(const vector<BankClient> & _vClients) {
		_printClientsTableHeader();

		if (_vClients.empty())
			cout << setw(49) << " " << "No clients available\n";
		else for (const BankClient & C : _vClients)
			_printClientLine(C);

		cout << setw(8) << " " << "------------------------------------------------------------------------------------------------------\n" << endl;
	}

public:
	//----Main Method:
	static void displayClientList() {
		if (!_checkScreenAccessibility(SystemUser::enPermissions::pListClients))
			return;

		const vector<BankClient> _vClients = BankClient::getClientsList();

		_printScreenHeader(31, "Clients List Screen", "(" + to_string(_vClients.size()) + ") Client(s) available in the system");

		_printClientsTable(_vClients);
	}

};

