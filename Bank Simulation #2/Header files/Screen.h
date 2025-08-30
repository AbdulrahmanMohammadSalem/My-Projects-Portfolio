// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "B:\PROGRAMMING\Abu-Hadhoud\My Libraries\Readers.h"
#include "B:\PROGRAMMING\Abu-Hadhoud\My Libraries\dates.h"
#include "B:\PROGRAMMING\Abu-Hadhoud\My Libraries\Utils.h"
#include "BankClient.h"
#include "SystemUser.h"
#include "Currency.h"
#include "Global.h"

class Screen {
private:
	//Helper Methods:
	static void _printUserPermissionsAsText(SystemUser & _user) {
		switch (_user.getPermissions()) {
			case -1:
				cout << "\nPermissions : Full Access";
				return;

			case 0:
				cout << "\nPermissions : No Access";
				return;

			default:
				cout << "\nPermissions :";

				if (_user.doesHavePermission(SystemUser::enPermissions::pListClients))
					cout << "\n   - Show Clients List";

				if (_user.doesHavePermission(SystemUser::enPermissions::pAddNewClient))
					cout << "\n   - Add New Client";
				
				if (_user.doesHavePermission(SystemUser::enPermissions::pDeleteClient))
					cout << "\n   - Delete Client";
				
				if (_user.doesHavePermission(SystemUser::enPermissions::pUpdateClient))
					cout << "\n   - Update Client";
				
				if (_user.doesHavePermission(SystemUser::enPermissions::pFindClient))
					cout << "\n   - Find Client";
				
				if (_user.doesHavePermission(SystemUser::enPermissions::pShowTransactionsMenu))
					cout << "\n   - Transactions";
				
				if (_user.doesHavePermission(SystemUser::enPermissions::pManageUsers))
					cout << "\n   - Manage Users";
				
				if (_user.doesHavePermission(SystemUser::enPermissions::pLoginRegister))
					cout << "\n   - Login Register";
		}
	}

protected:
	//Screen-Related...
	static void _printScreenHeader(const short & _outerIndent, const string & _title, const string & _subtitle = "") {
		Utils::Mesc::printHeader(_outerIndent, '-', _title, _subtitle); //This function is shared down below...
		
		cout << setw(42) << "" << "Active User: " << ActiveUser.getUsername() << "  Date: " << dates::formatDate(dates::getSystemDate(), "dd/mm/yyyy") << "\n\n\n";
	}

	static bool _checkScreenAccessibility(const SystemUser::enPermissions & _screen) {
		if (!ActiveUser.doesHavePermission(_screen)) {
			Utils::Mesc::printHeader(38, '#', "Access Denied!", "Please contact your Admin.");

			return false;
		}
		
		return true;
	}

	//Client-Related...
	static void _printClientInfo(const BankClient & C) {
		cout << "\nClient Card:";
		cout << "\n-----------------------------";
		cout << "\nFirst Name  : " << C.getFirstName();
		cout << "\nLast Name   : " << C.getLastName();
		cout << "\nFull Name   : " << C.getFullName();
		cout << "\nEmail       : " << C.getEmail();
		cout << "\nPhone       : " << C.getPhone();
		cout << "\nAcc. Number : " << C.getAccountNumber();
		cout << "\nPIN Code    : " << C.getPinCode();
		cout << "\nBalance     : " << C.getBalance();
		cout << "\n-----------------------------" << endl;
	}

	static string _readNonExistingAccountNumber() {
		string _accountNumber = readers::readWord("Please enter the new account number:\n");

		while (true) {
			if (BankClient::doesClientExist(_accountNumber))
				_accountNumber = readers::readWord("Account number already exists, try again:\n");
			else
				return _accountNumber;
		}
	}

	static BankClient _readExistingClientByAccountNumber(const string _message = "Please enter the client's account number:\n") {
		string _accountNumber = readers::readWord(_message);
		BankClient _client; //I made an empty constructor for this.

		while (true) {
			if ((_client = BankClient::findClient(_accountNumber)).isEmpty())
				_accountNumber = readers::readWord("This account number hasn't been found, try again:\n");
			else
				return _client;
		}
	}

	//User-Related...
	static void _printUserInfo(SystemUser & U) {
		cout << "\nUser Card:";
		cout << "\n---------------------------------------";
		cout << "\nFull Name   : " << U.getFullName();
		cout << "\nEmail       : " << U.getEmail();
		cout << "\nPhone       : " << U.getPhone();
		cout << "\nUsername    : " << U.getUsername();
		cout << "\nPassword    : " << U.getPassword();

		_printUserPermissionsAsText(U);

		cout << "\n---------------------------------------" << endl;
	}

	static string _readNonExistingUsername() {
		string _username = readers::readWord("Please enter the new username:\n");

		while (true) {
			if (SystemUser::doesUserExist(_username))
				_username = readers::readWord("Username already exists, try again:\n");
			else
				return _username;
		}
	}
	
	static SystemUser _readExistingUserByUsername() {
		string _username = readers::readWord("Please enter the username:\n");
		SystemUser _user; //I made an empty constructor for this.

		while (true) {
			_user = SystemUser::findUser(_username);

			if (_user.isEmpty())
				_username = readers::readWord("This username hasn't been found, try again:\n");
			else
				return _user;
		}
	}

	static short _readUserPermissions() {
		short permissions = 0;

		if (readers::readBoolean("Do you want to give Full Access? [Y] - [N] -> "))
			permissions = static_cast<short>(SystemUser::enPermissions::pAll);
		else {
			cout << "\nSetting Main Menu options permissions:\n";

			if (readers::readBoolean("   - Show Clients List? [Y] - [N] ->  "))
				permissions += static_cast<short>(SystemUser::enPermissions::pListClients);

			if (readers::readBoolean("   - Add New Client? [Y] - [N] ->  "))
				permissions += static_cast<short>(SystemUser::enPermissions::pAddNewClient);

			if (readers::readBoolean("   - Delete Client? [Y] - [N] ->  "))
				permissions += static_cast<short>(SystemUser::enPermissions::pDeleteClient);

			if (readers::readBoolean("   - Update Client Info? [Y] - [N] ->  "))
				permissions += static_cast<short>(SystemUser::enPermissions::pUpdateClient);

			if (readers::readBoolean("   - Find Client? [Y] - [N] ->  "))
				permissions += static_cast<short>(SystemUser::enPermissions::pFindClient);

			if (readers::readBoolean("   - Transactions? [Y] - [N] ->  "))
				permissions += static_cast<short>(SystemUser::enPermissions::pShowTransactionsMenu);

			if (readers::readBoolean("   - Manage Users? [Y] - [N] ->  "))
				permissions += static_cast<short>(SystemUser::enPermissions::pManageUsers);

			if (readers::readBoolean("   - Login Register? [Y] - [N] ->  "))
				permissions += static_cast<short>(SystemUser::enPermissions::pLoginRegister);

			//In case all the options were chosen:
			if (permissions == 255)
				permissions = static_cast<short>(SystemUser::enPermissions::pAll);
		}

		return permissions;
	}

	//Currency-Related...
	static Currency _readingExistingCurrencyByCode(const string & _message = "Please enter the Currency Code: ") {
		string _code = readers::readWord(_message, "Enter only one word: ");
		Currency _currency; //I made an empty constructor for this.

		while (true) {
			if ((_currency = Currency::findByCode(_code)).isEmpty())
				_code = readers::readWord("This currency hasn't been found, try again: ");
			else
				return _currency;
		}
	}

	static void _printCurrencyInfo(const Currency & C, const string & title = "Currency Card:") {
		cout << '\n' << title;
		cout << "\n--------------------------------";
		cout << "\nCountry     : " << C.getCountry();
		cout << "\nCode        : " << C.getCurrencyCode();
		cout << "\nName        : " << C.getCurrencyName();
		cout << "\nRate (/1$)  : " << C.getRate();
		cout << "\n--------------------------------\n";
	}

};
