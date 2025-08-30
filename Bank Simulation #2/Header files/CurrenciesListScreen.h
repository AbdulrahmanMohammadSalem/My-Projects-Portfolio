// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "Currency.h"

class CurrenciesListScreen : public Screen {
private:
	//Helper Methods:
	static void _printCurrenciesTableHeader() {
		cout << setw(7) << "" << Utils::Mesc::getCharacterString('-', 121) << '\n';
		cout << setw(7) << ""; //Outer indent

		cout << setw(40) << left << "| Country";
		cout << setw(10) << left << "| Code";
		cout << setw(50) << left << "| Name";
		cout << setw(20) << left << "| Rate/(1$)";

		cout << "|\n";
		cout << setw(7) << "" << Utils::Mesc::getCharacterString('-', 121) << '\n';
	}
	
	static void _printCurrenciesTableRows(const vector<Currency> & _vCurrencies) {
		if (_vCurrencies.empty())
			cout << setw(55) << "" << "Nothing to show here\n";
		else for (const Currency & C : _vCurrencies) {
			cout << setw(7) << ""; //Outer indent

			cout << setw(40) << left << ("| " + C.getCountry());
			cout << setw(10) << left << ("| " + C.getCurrencyCode());
			cout << setw(50) << left << ("| " + C.getCurrencyName());
			cout << "| " << setw(18) << left << C.getRate() << "|\n";
		}
	}

	static void _printCurrenciesTable(const vector<Currency> & _vCurrencies) {
		_printCurrenciesTableHeader();

		_printCurrenciesTableRows(_vCurrencies);

		cout << setw(7) << "" << Utils::Mesc::getCharacterString('-', 121) << '\n';
	}

public:
	//Main Method:
	static void displayCurrencyListScreen() {
		const vector<Currency> _vCurrencies = Currency::getAllCurrencies();

		_printScreenHeader(47, "Currencies List", to_string(_vCurrencies.size()) + " Currencies available.");

		_printCurrenciesTable(_vCurrencies);
	}
};
