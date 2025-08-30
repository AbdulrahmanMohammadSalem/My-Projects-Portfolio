// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class TotalBalancesScreen : private Screen {
private:
	//----Helper Methods:
	static void _printTotalBalancesTableHeader(const size_t & numberOfClients) {
		cout << setw(47) << "" << "Balances List - " << numberOfClients << " Client(s)\n";
		cout << setw(10) << "" << "---------------------------------------------------------------------------------------------------\n";
		cout << setw(10) << "" << "| " << left << setw(30) << "Account Number";
		cout << "| " << left << setw(35) << "Client Name";
		cout << "| " << left << setw(27) << "Balance" << "|\n";
		cout << setw(10) << "" << "---------------------------------------------------------------------------------------------------\n";
	}

	static void _printClientBalanceLine(const BankClient & client) {
		cout << setw(10) << "" << "| " << setw(30) << left << client.getAccountNumber();
		cout << "| " << setw(35) << left << client.getFullName();
		cout << "| " << setw(27) << left << client.getBalance() << "|\n";
	}

	static void _printTotalBalancesTableRows(const vector<BankClient> & _vClients) {
		if (_vClients.empty())
			cout << setw(50) << "" << "No clients available\n";
		else for (const BankClient & C : _vClients)
			_printClientBalanceLine(C);
	}

	static void _printTotalBalancesTable(const vector<BankClient> & _vClients) {
		_printTotalBalancesTableHeader(_vClients.size());

		_printTotalBalancesTableRows(_vClients);

		cout << setw(10) << "" << "---------------------------------------------------------------------------------------------------\n";
	}

	static void _printTotalBalancesFooter() {
		float totalBalances = BankClient::getTotalBalances();

		cout << setw(50) << "" << "Total Balances = " << totalBalances;
		cout << '\n' << setw(32) << "" << "(" << Utils::Mesc::getNumberAsText(totalBalances, 2) << ')' << endl;

	}

public:
	//----Main Method:
	static void displayTotalBalancesScreen() {
		const vector<BankClient> & _vClients = BankClient::getClientsList();

		_printScreenHeader(32, "Total Balances Screen", to_string(_vClients.size()) + " Client(s) available in the system.");

		_printTotalBalancesTable(_vClients);

		_printTotalBalancesFooter();
	}
};
