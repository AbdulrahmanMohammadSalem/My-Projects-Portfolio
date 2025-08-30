// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Person.h"
#include "Files.h"
#include "StringHandler.h"
#include "Dates.h"
#include <string>

class SystemUser : public Person {
private:
	//UpdateMode is the default mode for an active user object
	enum enModes { eEmptyMode, eUpdateMode, eAddNewMode };

public:
	struct stLoginEntry {
		string _dateTimeText, _username, _password;
		short _permissions;
	};

	enum enPermissions {
		pNon = 0, pAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
		pUpdateClient = 8, pFindClient = 16, pShowTransactionsMenu = 32,
		pManageUsers = 64, pLoginRegister = 128
	};

private:
	///Properties:
	enModes _Mode;
	string _Username;
	string _Password;
	short _Permissions;

	///Private Methods:
	static SystemUser _getEmptyUser() {
		return SystemUser(enModes::eEmptyMode, "", "", "", "", "", "", 0);
	}

	static SystemUser _convertLineToUser(const string & _line, const string & _separator = "#//#") {
		const vector<string> _lineParts = StringHandler::getVectorOfTokens(_line, _separator);

		return SystemUser(enModes::eUpdateMode, _lineParts[0], _lineParts[1], _lineParts[2],
			_lineParts[3], _lineParts[4], Utils::Encrypt_Decrypt::decrypt(_lineParts[5], ENCRYPTION_KEY), stoi(_lineParts[6]));
	}

	string _convertUserToLine(const SystemUser & _user, const string & _separator = "#//#") {
		string line = "";

		line += _user.getFirstName() + _separator;
		line += _user.getLastName() + _separator;
		line += _user.getEmail() + _separator;
		line += _user.getPhone() + _separator;
		line += _user.getUsername() + _separator;
		line += Utils::Encrypt_Decrypt::encrypt(_user.getPassword(), ENCRYPTION_KEY) + _separator;
		line += to_string(_user.getPermissions());

		return line;
	}

	string _convertUserToLogEntry(const string & _separator = "#//#") {
		string result = "";
		const dates::sDateTime _dateTime = dates::getSystemDateTime();

		result += dates::formatDate(_dateTime.date) + ' ';
		result += dates::formatTime(_dateTime.time, true, true) + _separator;
		result += _Username + _separator;
		result += Utils::Encrypt_Decrypt::encrypt(_Password, ENCRYPTION_KEY) + _separator;
		result += to_string(_Permissions);

		return result;
	}

	static stLoginEntry _convertLoginEntryLineToStruct(const string & _line, const string & _separator = "#//#") {
		stLoginEntry _sLoginEntry;
		vector<string> _vEntryParts = strings::getVctOfTkns(_line, _separator);

		_sLoginEntry._dateTimeText = _vEntryParts[0];
		_sLoginEntry._username = _vEntryParts[1];
		_sLoginEntry._password = Utils::Encrypt_Decrypt::decrypt(_vEntryParts[2], ENCRYPTION_KEY);
		_sLoginEntry._permissions = stoi(_vEntryParts[3]);

		return _sLoginEntry;
	}

	static vector<SystemUser> _loadUsersFromFile() {
		fstream _usersFile("Users.txt", ios::in | ios::app);
		vector<SystemUser> _vUsers;

		if (_usersFile.is_open()) {
			string _line;

			while (getline(_usersFile, _line))
				_vUsers.push_back(_convertLineToUser(_line));

			_usersFile.close();
		}
		else
			cerr << "\nError: Cannot open file: Users.txt [_loadUsersFromFile()]";

		return _vUsers;
	}

	void _saveUsersToFile(const vector<SystemUser> & _vUsers) {
		fstream _usersFile("Users.txt", ios::out); //write mode

		if (_usersFile.is_open()) {

			for (const SystemUser & U : _vUsers)
				_usersFile << _convertUserToLine(U) << '\n';

			_usersFile.close();
		}
		else
			cerr << "\nError: Cannot open file: Users.txt [_saveUsersToFile(const vector<BankUser>&)]";
	}

	void _updateUserInFile() {
		vector<SystemUser> _vUsers = _loadUsersFromFile();

		for (SystemUser & U : _vUsers)
			if (U.getUsername() == _Username) {
				U = *this;
				break;
			}

		_saveUsersToFile(_vUsers);
	}

	void _addUserToFile() {
		files::appendLnInFile("Users.txt", _convertUserToLine(*this));
	}

public:
	///Constructor:
	SystemUser(const enModes & mode, const string & firstName, const string & lastName,
		const string & email, const string & phone, const string & username,
		const string & password, const short & permissions)
		
		: Person(firstName, lastName, email, phone) {
		_Mode = mode;
		_Username = username;
		_Password = password;
		_Permissions = permissions;
	}

	SystemUser(){}

	///Properties Setting Up:
	bool isEmpty() {
		return _Mode == enModes::eEmptyMode;
	}

	string getUsername() const {
		return _Username;
	}
	void setUsername(const string & username) {
		_Username = username;
	}
	__declspec(property(get = getUsername, put = setUsername)) string Username;

	string getPassword() const {
		return _Password;
	}
	void setPassword(const string & password) {
		_Password = password;
	}
	__declspec(property(get = getPassword, put = setPassword)) string Password;

	short getPermissions() const {
		return _Permissions;
	}
	void setPermissions(const short & permissions) {
		_Permissions = permissions;
	}
	__declspec(property(get = getPermissions, put = setPermissions)) short Permissions;

	///Public Methods:
	static SystemUser findUser(const string & username) {
		fstream _usersFile("Users.txt", ios::in | ios::app); //The append mode creates the file if it doesn't exist.

		if (_usersFile.is_open()) {
			string _line;
			SystemUser _user; //There's a default constructor.

			while (getline(_usersFile, _line)) {
				_user = _convertLineToUser(_line);

				if (_user.getUsername() == username) {
					_usersFile.close(); //This is important, to insure the accessibility of the file in the future.

					return _user;
				}
			}

			//If the return statement hasn't been reached -> the User hasn't been found.

			_usersFile.close();
			return _getEmptyUser();
		}
		else
			cerr << "\nError: Cannot open file: Users.txt [findUser(const string &)]";
	}

	static SystemUser findUser(const string & username, const string & password) {
		fstream _usersFile("Users.txt", ios::in | ios::app); //The append mode creates the file if it doesn't exist.

		if (_usersFile.is_open()) {
			string _line;
			SystemUser _user; //There's a default constructor.

			while (getline(_usersFile, _line)) {
				_user = _convertLineToUser(_line);

				if (_user.getUsername() == username && _user.getPassword() == password) {
					_usersFile.close(); //This is important, to insure the accessibility of the file in the future.

					return _user;
				}
			}

			//If the return statement hasn't been reached -> the User hasn't been found.

			_usersFile.close();
			return _getEmptyUser();
		}
		else
			cerr << "\nError: Cannot open file: Users.txt [findUser(const string&, const string&)]";
	}

	static bool doesUserExist(const string & username) {
		return !findUser(username).isEmpty();
	}

	static SystemUser intializeNewUser(const string & username) {
		return SystemUser(enModes::eAddNewMode, "", "", "", "", username, "", 0);
	}

	static vector<SystemUser> getUsersList() {
		return _loadUsersFromFile();
	}

	static vector<stLoginEntry> getLoginEntries() {
		fstream _loginRegisterFile("LoginRegister.txt", ios::in);
		vector<stLoginEntry> _vLoginEntries;

		if (_loginRegisterFile.is_open()) {
			string _line;

			while (getline(_loginRegisterFile, _line))
				_vLoginEntries.push_back(_convertLoginEntryLineToStruct(_line));
			
			_loginRegisterFile.close();
		}
		else
			cerr << "\nError: Cannot open file: LoginRegister.txt [getLoginEntries()]";

		return _vLoginEntries;
	}

	static void prepareUsersFile() {
		if (files::isFileEmpty("Users.txt"))
			files::appendLnInFile("Users.txt", "Abdulrahman#//#Salem#//#sal@gmail.com#//#24525221#//#User1#//#RSTU#//#-1");
	}

	enum enSaveResults { eErrorEmptyUser, eErrorUserAlreadyExists, eErrorUserDoesNotExists, eSuccess };

	enSaveResults saveInFile(const bool validateObject) {
		switch (_Mode) {
			case enModes::eEmptyMode: //The User is empty.
				return enSaveResults::eErrorEmptyUser;

			case enModes::eUpdateMode: //The User already exists in the file, but contains outdated data.
				if (validateObject && !doesUserExist(_Username))
					return enSaveResults::eErrorUserDoesNotExists;

				_updateUserInFile();

				return enSaveResults::eSuccess;

			case enModes::eAddNewMode: //The User does not exist in the file at all.
				if (validateObject && doesUserExist(_Username))
					return enSaveResults::eErrorUserAlreadyExists;

				_addUserToFile();

				_Mode = enModes::eUpdateMode; //The update mode is the default mode for active Users

				return enSaveResults::eSuccess;
		}
	}

	void remove() {
		vector<SystemUser> _vUsers = _loadUsersFromFile();

		for (vector<SystemUser>::iterator it = _vUsers.begin(); it != _vUsers.end(); it++)
			if (it->getUsername() == _Username) {
				_vUsers.erase(it);
				break;
			}

		*this = _getEmptyUser();

		_saveUsersToFile(_vUsers);
	}

	bool doesHavePermission(const enPermissions & action) {
		switch (_Permissions) {
			case enPermissions::pAll:
				return true;

			case enPermissions::pNon:
				return false;

			default:
				return (_Permissions & static_cast<short>(action)) == static_cast<short>(action);
		}
	}

	void registerLogin() {
		files::appendLnInFile("LoginRegister.txt", _convertUserToLogEntry());
	}

};

