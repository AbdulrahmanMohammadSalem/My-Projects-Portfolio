// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"

class CurrencyCalculatorScreen : private Screen {
private:
	//Helper Method:
	static void _printCalculationResults(const Currency & _currencyFrom, const Currency & _currencyTo, const float & _amount) {
		_printCurrencyInfo(_currencyFrom, "Converting from:");
		_printCurrencyInfo(_currencyTo, "To:");

		cout << "\nYields:\n";

		cout << _amount << ' ' << _currencyFrom.getCurrencyCode()
			<< " = " 
			<< _currencyFrom.convertTo(_currencyTo, _amount) << ' ' << _currencyTo.getCurrencyCode();
	}

public:
	//Main Method:
	static void displayCurrencyCalculatorScreen() {
		bool shouldRestart = true;
		
		while (shouldRestart) {
			system("cls");
			_printScreenHeader(40, "Currency Calculator Screen");

			const Currency _currencyFrom = _readingExistingCurrencyByCode("Please enter the Currency code to calculate from:\n");
			const Currency _currencyTo = _readingExistingCurrencyByCode("Please enter the Currency code to calculate to:\n");
			const float _amount = readers::readPositiveNumber<float>("Enter the amount:\n", "Invalid input, try again:\n");

			_printCalculationResults(_currencyFrom, _currencyTo, _amount);

			cout << "\n\n\n";
			shouldRestart = readers::readBoolean("Perform another calculation? [Y] - [N] -> ");
		}
	}

};
