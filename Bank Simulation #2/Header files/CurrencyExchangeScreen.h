// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "CurrenciesListScreen.h"
#include "FindCurrencyScreen.h"
#include "UpdateCurrencyRateScreen.h"
#include "CurrencyCalculatorScreen.h"

class CurrencyExchangeScreen : private Screen {
    enum enCurrencyExchangeMenuOptions {
        eListCurrencies = 1, eFindCurrency, eUpdateRate, eCurrencyCalculator, eMainMenu
    };

private:
    //Helper Methods:
    static  void _backToCurrencyExchangeMenu() {
        cout << '\n' << setw(36) << left << " " << "Press any key to go back to Currency Exchange Menu...\n";

        system("pause>nul");
        system("cls");
        displayCurrencyExchangeScreen();
    }

    static enCurrencyExchangeMenuOptions _readCurrencyExchangeMenuOption() {
        cout << setw(37) << "" << "Choose an Option [1] to [5] -> ";

        return static_cast<enCurrencyExchangeMenuOptions>(readers::readNumber<short>("", 1, 5, "Invalid input, try again -> "));
    }

    static void _showListCurrenciesScreen() {
        CurrenciesListScreen::displayCurrencyListScreen();
    }

    static void _showFindCurrencyScreen() {
        FindCurrencyScreen::displayFindCurrencyScreen();
    }

    static void _showUpdateRateScreen() {
        UpdateCurrencyRateScreen::displayUpdateCurrencyRateScreen();
    }

    static void _showCurrencyCalculatorScreen() {
        CurrencyCalculatorScreen::displayCurrencyCalculatorScreen();
    }

    static void _performCurrencyExchangeMenuOption(const enCurrencyExchangeMenuOptions & option) {
        switch (option) {
            case CurrencyExchangeScreen::eListCurrencies:
                system("cls");
                _showListCurrenciesScreen();
                _backToCurrencyExchangeMenu();
                break;

            case CurrencyExchangeScreen::eFindCurrency:
                system("cls");
                _showFindCurrencyScreen();
                _backToCurrencyExchangeMenu();
                break;

            case CurrencyExchangeScreen::eUpdateRate:
                system("cls");
                _showUpdateRateScreen();
                _backToCurrencyExchangeMenu();
                break;

            case CurrencyExchangeScreen::eCurrencyCalculator:
                _showCurrencyCalculatorScreen();
                _backToCurrencyExchangeMenu();
                break;
        }
    }

public:
	//Main Method:
	static void displayCurrencyExchangeScreen() {
		_printScreenHeader(35, "Currency Exchange Menu Screen");

        cout << setw(37) << "" << "============================================\n";
        cout << setw(37) << "" << "           Currency Exchange Menu\n";
        cout << setw(37) << "" << "============================================\n";
        cout << setw(37) << "" << "    [1] List Currencies.\n";
        cout << setw(37) << "" << "    [2] Find Currency.\n";
        cout << setw(37) << "" << "    [3] Update Curency Rate.\n";
        cout << setw(37) << "" << "    [4] Currency Calculator.\n";
        cout << setw(37) << "" << "    [5] Main Menu.\n";
        cout << setw(37) << "" << "============================================\n";

        _performCurrencyExchangeMenuOption(_readCurrencyExchangeMenuOption());
	}

};

