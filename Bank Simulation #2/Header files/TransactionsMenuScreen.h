// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Screen.h"
#include "DepositScreen.h"
#include "WithdrawScreen.h"
#include "TotalBalancesScreen.h"
#include "TransferScreen.h"
#include "TransferLogScreen.h"

class TransactionsMenuScreen : private Screen {
private:
	enum enTransactionsMenuOptions {
		eDeposit = 1, eWithdraw, eTotalBalances, eTransfer, eTransferLog, eBackToMainMenu
	};

	//----Helper Methods:
	static enTransactionsMenuOptions _readTransactionsMenuOption() {
		cout << setw(37) << "" << "Choose an Option [1] to [6] -> ";

		return static_cast<enTransactionsMenuOptions>(readers::readNumber<short>("", 1, 6, "Invalid input, try again -> "));
	}

	static void _backToTransactionsScreen() {
		cout << '\n' << setw(40) << left << " " << "Press any key to go back to Transactions Menu...\n";

		system("pause>nul");
		system("cls");
		displayTransactionsMenu();
	}

	static void _showDepositScreen() {
		DepositScreen::displayDepositScreen();
	}

	static void _showWithdrawScreen() {
		WithdrawScreen::displayWithdrawScreen();
	}

	static void _showTotalBalancesScreen() {
		TotalBalancesScreen::displayTotalBalancesScreen();
	}

	static void _showTransferScreen() {
		TransferScreen::displayTransferScreen();
	}

	static void _showTransferLogScreen() {
		TransferLogScreen::displayTransferLogScreen();
	}

	static void _performTransactionsMenuOption(const enTransactionsMenuOptions & option) {
		system("cls");
		
		switch (option) {
			case enTransactionsMenuOptions::eDeposit:
				_showDepositScreen();
				_backToTransactionsScreen();
				break;

			case enTransactionsMenuOptions::eWithdraw:
				_showWithdrawScreen();
				_backToTransactionsScreen();
				break;

			case enTransactionsMenuOptions::eTotalBalances:
				_showTotalBalancesScreen();
				_backToTransactionsScreen();
				break;

			case enTransactionsMenuOptions::eTransfer:
				_showTransferScreen();
				_backToTransactionsScreen();
				break;

			case enTransactionsMenuOptions::eTransferLog:
				_showTransferLogScreen();
				_backToTransactionsScreen();
		}
	}

public:
	//----Main Method:
	static void displayTransactionsMenu() {
		if (!_checkScreenAccessibility(SystemUser::enPermissions::pShowTransactionsMenu))
			return;

		_printScreenHeader(41, "Transactions Screen");

		cout << setw(37) << "" << "============================================\n";
		cout << setw(37) << "" << "              Transactions Menu\n";
		cout << setw(37) << "" << "============================================\n";
		cout << setw(37) << "" << "    [1] Deposit.\n";
		cout << setw(37) << "" << "    [2] Withdraw.\n";
		cout << setw(37) << "" << "    [3] Show Total Balances.\n";
		cout << setw(37) << "" << "    [4] Transfer.\n";
		cout << setw(37) << "" << "    [5] Transfer History.\n";
		cout << setw(37) << "" << "    [6] Back to Main Menu.\n";
		cout << setw(37) << "" << "============================================\n";
		
		_performTransactionsMenuOption(_readTransactionsMenuOption());
	}
};
