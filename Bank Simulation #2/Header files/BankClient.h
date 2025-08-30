// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include "Person.h"
#include "Global.h"
#include "Files.h"
#include "StringHandler.h"
#include <fstream>
#include <string>

class BankClient : public Person {
private:
	//UpdateMode is the default mode for an active client object
	enum enModes { eEmptyMode, eUpdateMode, eAddNewMode};

public:
	struct stTransferEntry {
		string _dateTimeText, _sourceAccountNumber, _destinationAccountNumber, _username;
		float _amount, _sourceBalance, _destinationBalance;
	};

private:
	///Properties:
	enModes _Mode;
	string _AccountNumber; //Read-Only
	float _Balance;
	string _PinCode;

	///Private Methods:
	static BankClient _convertLineToClient(const string & _line, const string & _separator = "#//#") {
		const vector<string> _lineParts = StringHandler::getVectorOfTokens(_line, _separator);

		return BankClient(enModes::eUpdateMode, _lineParts[0], _lineParts[1],
			_lineParts[2], _lineParts[3], _lineParts[4], Utils::Encrypt_Decrypt::decrypt(_lineParts[5], ENCRYPTION_KEY), stof(_lineParts[6]));
	}

	string _convertClientToLine(const BankClient & _client, const string & _separator = "#//#") {
		string line = "";

		line += _client.getFirstName() + _separator;
		line += _client.getLastName() + _separator;
		line += _client.getEmail() + _separator;
		line += _client.getPhone() + _separator;
		line += _client.getAccountNumber() + _separator;
		line += Utils::Encrypt_Decrypt::encrypt(_client.getPinCode(), ENCRYPTION_KEY) + _separator;
		line += to_string(_client.getBalance());

		return line;
	}

	string _getTransferLogLine(const float & amount, const BankClient & _destination, const string & username, const string & _separator = "#//#") {
		string _result = "";
		dates::sDateTime _dateTime = dates::getSystemDateTime();
		
		_result += dates::formatDate(_dateTime.date) + ' ';
		_result += dates::formatTime(_dateTime.time, true, true) + _separator;
		_result += _AccountNumber + _separator;
		_result += _destination.getAccountNumber() + _separator;
		_result += to_string(amount) + _separator;
		_result += to_string(_Balance) + _separator;
		_result += to_string(_destination.getBalance()) + _separator;
		_result += username;

		return _result;
	}

	bool _registerTransferLog(const float & amount, const BankClient & destination, const string & username) {
		return files::appendLnInFile("TransferLog.txt", _getTransferLogLine(amount, destination, username));
	}

	static BankClient _getEmptyClient() {
		return BankClient(enModes::eEmptyMode, "", "", "", "", "", "", 0);
	}

	static stTransferEntry _convertTransferLineToStruct(const string & _line, const string & _separator = "#//#") {
		vector<string> _vLineParts = strings::getVctOfTkns(_line, _separator);
		stTransferEntry _transferEntry;

		_transferEntry._dateTimeText = _vLineParts[0];
		_transferEntry._sourceAccountNumber = _vLineParts[1];
		_transferEntry._destinationAccountNumber = _vLineParts[2];
		_transferEntry._amount = stoi(_vLineParts[3]);
		_transferEntry._sourceBalance = stoi(_vLineParts[4]);
		_transferEntry._destinationBalance = stoi(_vLineParts[5]);
		_transferEntry._username = _vLineParts[6];

		return _transferEntry;
	}

	static vector<BankClient> _loadClientsFromFile(){
		fstream _clientsFile("Clients.txt", ios::in | ios::app);
		vector<BankClient> _vClients;

		if (_clientsFile.is_open()) {
			string _line;

			while (getline(_clientsFile, _line))
				_vClients.push_back(_convertLineToClient(_line));

			_clientsFile.close();
		}
		else
			cerr << "\nError: Cannot open file: Clients.txt [_loadClientsFromFile()]";

		return _vClients;
		}

	void _saveClientsToFile(const vector<BankClient> & _vClients) {
		fstream _clientsFile("Clients.txt", ios::out); //write mode

		if (_clientsFile.is_open()) {

			for (const BankClient & C : _vClients)
				_clientsFile << _convertClientToLine(C) << '\n';

			_clientsFile.close();
		}
		else
			cerr << "\nError: Cannot open file: Clients.txt [_saveClientsToFile(const vector<BankClient>&)]";
	}

	void _appendLineToUsersFile(const string & line) {
		fstream _clientsFile("Clients.txt", ios::app);

		if (_clientsFile.is_open()) {
			_clientsFile << line << '\n';

			_clientsFile.close();
		}
		else
			cerr << "\nError: Cannot open file: Clients.txt [_appendLineToUsersFile(const string&)]";
	}

	void _updateClientInFile() {
		vector<BankClient> _vClients = _loadClientsFromFile();

		for (BankClient & C : _vClients)
			if (C.getAccountNumber() == _AccountNumber) {
				C = *this;
				break;
			}

		_saveClientsToFile(_vClients);
	}

	void _addClientToFile() {
		_appendLineToUsersFile(_convertClientToLine(*this));
	}

public:
	///Constructor:
	BankClient(const enModes mode, const string & firstName, const string & lastName,
		const string & email, const string & phone, const string & accountNumber,
		const string & pinCode, const float & accountBalance)

		: Person(firstName, lastName, email, phone) {
		_Mode = mode;
		_AccountNumber = accountNumber;
		_Balance = accountBalance;
		_PinCode = pinCode;
	}

	BankClient(){}

	///Properties setting up:
	bool isEmpty() {
		return _Mode == enModes::eEmptyMode;
	}

	string getAccountNumber() const {
		return _AccountNumber;
	}
	__declspec(property(get = getAccountNumber)) string AccountNumber;
	
	float getBalance() const {
		return _Balance;
	}
	void setBalance(const float & accountBalance) {
		_Balance = accountBalance;
	}
	__declspec(property(get = getBalance, put = setBalance)) float Balance;
	
	string getPinCode() const {
		return _PinCode;
	}
	void setPinCode(const string & pinCode) {
		_PinCode = pinCode;
	}
	__declspec(property(get = getPinCode, put = setPinCode)) string PinCode;

	///Public Methods:
	static BankClient findClient(const string & accountNumber) {
		fstream _clientsFile("Clients.txt", ios::in | ios::app); //The append mode creates the file if it doesn't exist.

		if (_clientsFile.is_open()) {
			string _line;
			BankClient _client; //There's a default constructor.

			while (getline(_clientsFile, _line)) {
				_client = _convertLineToClient(_line);

				if (_client.getAccountNumber() == accountNumber) {
					_clientsFile.close(); //This is important, to insure the accessibility of the file in the future.

					return _client;
				}
			}

			//If the return statement hasn't been reached -> the client hasn't been found.

			_clientsFile.close();
			return _getEmptyClient();
		}
		else
			cerr << "\nError: Cannot open file: Clients.txt [findClient(const string &)]";
	}

	static BankClient findClient(const string & accountNumber, const string & pinCode) {
		fstream _clientsFile("Clients.txt", ios::in | ios::app); //The append mode creates the file if it doesn't exist.

		if (_clientsFile.is_open()) {
			string _line;
			BankClient _client; //There's a default constructor.

			while (getline(_clientsFile, _line)) {
				_client = _convertLineToClient(_line);

				if (_client.getAccountNumber() == accountNumber && _client.getPinCode() == pinCode) {
					_clientsFile.close(); //This is important, to insure the accessibility of the file in the future.

					return _client;
				}
			}

			//If the return statement hasn't been reached -> the client hasn't been found.

			_clientsFile.close();
			return _getEmptyClient();
		}
		else
			cerr << "\nError: Cannot open file: Clients.txt [findClient(const string&, const string&)]";
	}

	static bool doesClientExist(const string & accountNumber) {
		return !findClient(accountNumber).isEmpty();
	}

	static BankClient intializeNewClient(const string & accountNumber) {
		return BankClient(enModes::eAddNewMode, "", "", "", "", accountNumber, "", 0);
	}

	static vector<BankClient> getClientsList() {
		return _loadClientsFromFile();
	}

	static vector<stTransferEntry> getTransferEntries() {
		fstream _transferLogFile("TransferLog.txt", ios::app | ios::in);
		vector<stTransferEntry> _vEntries;

		if (_transferLogFile.is_open()) {
			string _logLine;

			while (getline(_transferLogFile, _logLine))
				_vEntries.push_back(_convertTransferLineToStruct(_logLine));

			_transferLogFile.close();
		}
		else
			cerr << "\nError: Cannot open file: TransferLog.txt [getTransferEntries]";

		return _vEntries;
	}

	static float getTotalBalances() {
		const vector<BankClient> _vClients = _loadClientsFromFile();
		float totalBalances = 0;

		for (const BankClient & C : _vClients)
			totalBalances += C.getBalance();

		return totalBalances;
	}

	enum enSaveResults { eErrorEmptyClient, eErrorClientAlreadyExists, eErrorClientDoesNotExists, eSuccess };

	enSaveResults saveInFile(const bool & validateObject) {
		switch (_Mode) {
			case enModes::eEmptyMode: //The client is empty.
				return enSaveResults::eErrorEmptyClient;

			case enModes::eUpdateMode: //The client already exists in the file, but contains outdated data.
				if (validateObject && !doesClientExist(_AccountNumber))
					return enSaveResults::eErrorClientDoesNotExists;
				
				_updateClientInFile();
				
				return enSaveResults::eSuccess;

			case enModes::eAddNewMode: //The client does not exist in the file at all.
				if (validateObject && doesClientExist(_AccountNumber))
					return enSaveResults::eErrorClientAlreadyExists;
				
				_addClientToFile();

				_Mode = enModes::eUpdateMode; //The update mode is the default mode for active clients

				return enSaveResults::eSuccess;
		}
	}

	void remove() {
		vector<BankClient> _vClients = _loadClientsFromFile();

		for (vector<BankClient>::iterator it = _vClients.begin(); it != _vClients.end(); it++)
			if (it->getAccountNumber() == _AccountNumber) {
				_vClients.erase(it);
				break;
			}

		*this = _getEmptyClient();

		_saveClientsToFile(_vClients);
	}

	bool deposit(const float & amount, const bool & validateObject) {
		_Balance += amount;
		return saveInFile(validateObject) == enSaveResults::eSuccess;
	}

	bool withdraw(const float & amount, const bool & validateObject, const bool & validateAmount) {
		if (validateAmount && amount > _Balance)
			return false;

		_Balance -= amount;
		return saveInFile(validateObject) == enSaveResults::eSuccess;
	}

	bool transferTo(BankClient & destinationClient, const float & amount, const string & username, const bool & validateSource, const bool & validateDestination, const bool & validateAmount) {
		return withdraw(amount, validateSource, validateAmount) && destinationClient.deposit(amount, validateDestination)
			&& _registerTransferLog(amount, destinationClient, username);
	}

};

