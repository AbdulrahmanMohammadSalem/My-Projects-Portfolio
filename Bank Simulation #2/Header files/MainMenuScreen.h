// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "ShowClientListScreen.h"
#include "AddNewClientScreen.h"
#include "DeleteClientScreen.h"
#include "UpdateClientScreen.h"
#include "FindClientScreen.h"
#include "TransactionsMenuScreen.h"
#include "ManageUsersScreen.h"
#include "LoginRegisterScreen.h"
#include "CurrencyExchangeScreen.h"
#include "Global.h"

class MainMenuScreen : private Screen {
private:
    enum enMainMenuOptions {
        eListClients = 1, eAddNewClient, eDeleteClient,
        eUpdateClient, eFindClient, eShowTransactionsMenu,
        eManageUsers, eLoginRegister, eCurrencyExchange, eLogout
    };

    static  void _backToMainMenuScreen() {
        cout << '\n' << setw(40) << left << " " << "Press any key to go back to Main Menu...\n";
        
        system("pause>nul");
        displayMainMenu();
    }

    static enMainMenuOptions _readMainMenuOption() {
        cout << setw(35) << "" << "Choose an Option [1] to [10] -> ";
        
        return static_cast<enMainMenuOptions>(readers::readNumber<short>("", 1, 10, "Invalid input, try again -> "));
    }

    static void _showListClientScreen() {
        ShowClientListScreen::displayClientList();
    }

    static void _showAddNewClientScreen() {
        AddNewClientScreen::displayNewClientScreen();
    }

    static void _showDeleteClientScreen() {
        DeleteClientScreen::displayDeleteClientScreen();
    }

    static void _showUpdateClientScreen() {
        UpdateClientScreen::displayUpdateClientScreen();
    }

    static void _showFindClientScreen() {
        FindClientScreen::displayFindClientScreen();
    }

    static void _showTransactionsMenuScreen() {
        TransactionsMenuScreen::displayTransactionsMenu();
    }

    static void _showManageUsersScreen() {
        ManageUsersScreen::displayManageUsersMenu();
    }

    static void _showLoginRegisterScreen() {
        LoginRegisterScreen::displayLoginRegisterScreen();
    }

    static void _showCurrencyExchangeScreen() {
        CurrencyExchangeScreen::displayCurrencyExchangeScreen();
    }

    static void _logout() {
        ActiveUser = SystemUser(); //This fills the ActiveUser with garbage values from the memory
    }

    static void _performMainMenuOption(const enMainMenuOptions & option) {
        system("cls");

        switch (option) {
            case enMainMenuOptions::eListClients:
                _showListClientScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eAddNewClient:
                _showAddNewClientScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eDeleteClient:
                _showDeleteClientScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eUpdateClient:
                _showUpdateClientScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eFindClient:
                _showFindClientScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eShowTransactionsMenu:
                _showTransactionsMenuScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eManageUsers:
                _showManageUsersScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eLoginRegister:
                _showLoginRegisterScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eCurrencyExchange:
                _showCurrencyExchangeScreen();
                _backToMainMenuScreen();
                break;

            case enMainMenuOptions::eLogout:
                _logout();
        }
    }

public:
	static void displayMainMenu() {
		system("cls");
		_printScreenHeader(44, "Main Menu Screen");

        cout << setw(35) << "" << "================================================\n";
        cout << setw(35) << "" << "                   Main Menu\n";
        cout << setw(35) << "" << "================================================\n";
        cout << setw(35) << "" << "    [01] Show Clients List.\n";
        cout << setw(35) << "" << "    [02] Add New Client.\n";
        cout << setw(35) << "" << "    [03] Delete Client.\n";
        cout << setw(35) << "" << "    [04] Update Client Info.\n";
        cout << setw(35) << "" << "    [05] Find Client.\n";
        cout << setw(35) << "" << "    [06] Transactions.\n";
        cout << setw(35) << "" << "    [07] Manage Users.\n";
        cout << setw(35) << "" << "    [08] Login History.\n";
        cout << setw(35) << "" << "    [09] Currency Exchange.\n";
        cout << setw(35) << "" << "    [10] Logout.\n";
        cout << setw(35) << "" << "================================================\n";

        _performMainMenuOption(_readMainMenuOption());
	}
};
