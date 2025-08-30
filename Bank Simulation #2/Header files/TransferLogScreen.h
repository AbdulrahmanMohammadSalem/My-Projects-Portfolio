// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class TransferLogScreen : private Screen {
private:
	//Helper Methods:
	static void _printTransferLogTableHeader() {
		cout << setw(7) << "" << Utils::Mesc::getCharacterString('-', 104);
		cout << '\n' << setw(7) << ""; //Outer indent
		
		cout << setw(21) << left << "| Date/Time";
		cout << setw(11) << left << "| s. Acc.";
		cout << setw(11) << left << "| d. Acc.";
		cout << setw(15) << left << "| Amount";
		cout << setw(15) << left << "| s. Balance";
		cout << setw(15) << left << "| d. Balance";
		cout << setw(15) << left << "| Username";

		cout << "|\n";
		cout << setw(7) << "" << Utils::Mesc::getCharacterString('-', 104) << '\n';
	}

	static void _printTransferLogTableRow(const BankClient::stTransferEntry & E) {
		cout << setw(7) << ""; //Outer indent

		cout << setw(21) << ("| " + E._dateTimeText);
		cout << setw(11) << ("| " + E._sourceAccountNumber);
		cout << setw(11) << ("| " + E._destinationAccountNumber);
		cout << "| " << setw(13) << E._amount;
		cout << "| " << setw(13) << E._sourceBalance;
		cout << "| " << setw(13) << E._destinationBalance;
		cout << setw(15) << ("| " + E._username) << "|\n";
	}

	static void _printTransferLogTableRows(const vector<BankClient::stTransferEntry> & _vTransferEntries) {
		for (const BankClient::stTransferEntry & E : _vTransferEntries)
			_printTransferLogTableRow(E);
	}

	static void _printTransferLogTable(const vector<BankClient::stTransferEntry> & _vTransferEntries) {
		_printTransferLogTableHeader();

		_printTransferLogTableRows(_vTransferEntries);

		cout << setw(7) << "" << Utils::Mesc::getCharacterString('-', 104) << endl;
	}

public:
	//Main Method:
	static void displayTransferLogScreen() {
		const vector<BankClient::stTransferEntry> _vTransferEntries = BankClient::getTransferEntries();

		_printScreenHeader(42, "Transfer Log Screen", to_string(_vTransferEntries.size()) + " Entries available.");

		_printTransferLogTable(_vTransferEntries);
	}

};
