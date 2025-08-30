// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "ShowUsersListScreen.h"
#include "AddNewUserScreen.h"
#include "DeleteUserScreen.h"
#include "UpdateUserScreen.h"
#include "FindUserScreen.h"

class ManageUsersScreen : public Screen {
private:
    enum enManageUsersMenuOptions {
        eListUsers = 1, eAddNewUser, eDeleteUser,
        eUpdateUser, eFindUser, eBackToMainMenu
    };

    //----Helper Methods:
    static enManageUsersMenuOptions _readManageUsersMenuOption() {
        cout << setw(37) << "" << "Choose an Option [1] to [6] -> ";

        return static_cast<enManageUsersMenuOptions>(readers::readNumber<short>("", 1, 6, "Invalid input, try again -> "));
    }

    static void _backToManageUsersScreen() {
        cout << '\n' << setw(31) << left << " " << "Press any key to go back to Manage Users Menu...\n";

        system("pause>nul");
        system("cls");
        displayManageUsersMenu();
    }

    static void _showListUsersScreen() {
        ShowUsersListScreen::displayUsersList();
    }

    static void _showAddNewUserScreen() {
        AddNewUserScreen::displayNewUserScreen();
    }

    static void _showDeleteUserScreen() {
        DeleteUserScreen::displayDeleteUserScreen();
    }

    static void _showUpdateUserScreen() {
        UpdateUserScreen::displayUpdateUserScreen();
    }

    static void _showFindUserScreen() {
        FindUserScreen::displayFindUserScreen();
    }

    static void _performManageUsersMenuOption(const enManageUsersMenuOptions option) {
        system("cls");

        switch (option) {
            case ManageUsersScreen::eListUsers:
                _showListUsersScreen();
                _backToManageUsersScreen();
                break;

            case ManageUsersScreen::eAddNewUser:
                _showAddNewUserScreen();
                _backToManageUsersScreen();
                break;

            case ManageUsersScreen::eDeleteUser:
                _showDeleteUserScreen();
                _backToManageUsersScreen();
                break;

            case ManageUsersScreen::eUpdateUser:
                _showUpdateUserScreen();
                _backToManageUsersScreen();
                break;

            case ManageUsersScreen::eFindUser:
                _showFindUserScreen();
                _backToManageUsersScreen();
        }
    }

public:
	//----Main Method:
	static void displayManageUsersMenu() {
        if (!_checkScreenAccessibility(SystemUser::enPermissions::pManageUsers))
            return;

		_printScreenHeader(42, "Manage Users Screen");
        
        cout << setw(37) << "" << "============================================\n";
        cout << setw(37) << "" << "             Manage Users Menu\n";
        cout << setw(37) << "" << "============================================\n";
        cout << setw(37) << "" << "    [1] Show Users List.\n";
        cout << setw(37) << "" << "    [2] Add New User.\n";
        cout << setw(37) << "" << "    [3] Delete User.\n";
        cout << setw(37) << "" << "    [4] Update User Info.\n";
        cout << setw(37) << "" << "    [5] Find User.\n";
        cout << setw(37) << "" << "    [6] Back to Main Menu.\n";
        cout << setw(37) << "" << "============================================\n";

        _performManageUsersMenuOption(_readManageUsersMenuOption());
	}

};
