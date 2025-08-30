// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class FindCurrencyScreen : private Screen {
	enum enFindBy { eCode = 1, eCountry = 2};

private:
	//Helper Methods:
	static void _findCurrencyByCountry() {
		//Reading:
		string _country = readers::readString("Please enter the Country: ", true);
		Currency _currency; //I made an empty constructor for this.

		while (true) {
			if ((_currency = Currency::findByCountry(_country)).isEmpty())
				_country = readers::readString("This country hasn't been found, try again: ");
			else
				break;
		}

		//printing:
		cout << "\nCurrency Found! :-)\n";
		_printCurrencyInfo(_currency);
	}

	static void _findCurrencyByCode() {
		//Reading:
		Currency _currency = _readingExistingCurrencyByCode(); //I made an empty constructor for this.

		//Printing:
		cout << "\nCurrency Found! :-)\n";
		_printCurrencyInfo(_currency);
	}

	static void _performFindCurrencyAction(const enFindBy & findBy) {
		cout << '\n';

		if (findBy == enFindBy::eCode)
			_findCurrencyByCode();
		else
			_findCurrencyByCountry();
	}

public:
	//Main Method:
	static void displayFindCurrencyScreen() {
		_printScreenHeader(40, "Find Currency Screen");

		_performFindCurrencyAction(static_cast<enFindBy>(readers::readNumber<short>("Find by: [1] Code - [2] Country -> ")));
	}

};

