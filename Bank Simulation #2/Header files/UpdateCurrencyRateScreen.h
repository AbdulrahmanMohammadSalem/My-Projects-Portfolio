// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class UpdateCurrencyRateScreen : private Screen {
public:
	//Main Method:
	static void displayUpdateCurrencyRateScreen() {
		_printScreenHeader(40, "Update Currency Rate Screen");

		Currency _currency; //I made an empty constructor for this.
		
		while (true) {
			_currency = _readingExistingCurrencyByCode();

			if (_currency.getCurrencyCode() == "USD")
				cout << "\nYou cannot change the rate of this specific currency, choose another one.\n\n";
			else {
				cout << "\nCurrency Found! :-)\n";
				break;
			}
		}

		_printCurrencyInfo(_currency);

		cout << '\n';

		if (readers::readBoolean("Are you sure you want to change the Currency Rate? [Y] - [N] -> ")) {
			cout << '\n';

			_currency.setRate(readers::readPositiveNumber<float>("Enter the new Rate: "), true);

			cout << "\nDone Successfully!\n";

			_printCurrencyInfo(_currency);
		}
	}

};
