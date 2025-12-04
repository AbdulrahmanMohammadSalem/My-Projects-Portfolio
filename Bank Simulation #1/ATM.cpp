// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

#pragma warning(disable: 4996)

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;

const string clientsFileName = "Clients.txt";
const string clientsHistoryFileName = "History.txt";
const string messageFileName = "Messages.txt";

void displayLoginScreen();
void displayMainMenuScreen();

enum enMainMenuOptions {
	eQuickWithdraw = 1, eNormalWithdraw, eDeposit, eTransferMoney, eCheckMessages,
	eCheckBalance, eCheckAccountHistory, eChangePhoneNumber, eChangePINCode, eLogout
};

enum enClientUpdateType {
	eClientCreated, eFullUpdate, eOnlyDeposit, eOnlyWithdraw, eTransferedMoney,
	eReceivedMoney, eChangedPhoneNumber, eChangedPinCode, eAccountLocked, eAccountUnlocked
};

struct sClient {
	string accountNumber = "";
	string pinCode = "";
	string name = "";
	string phoneNumber = "";
	string dateCreated;
	string dateModified = "-";
	double accountBalance = 0;
	bool markAsLocked = false;
};

struct sClientForHistory {
	string accountNumber = "";
	string pinCode = "";
	string name = "";
	string phoneNumber = "";
	string date = "";
	double accountBalance = 0;
	enClientUpdateType actionDone = enClientUpdateType::eClientCreated;
};

vector<sClient> vClients;
short activeClientPosition = 0;

void printLoginScreenHeader() {
	cout << "------------------------------\n";
	cout << "        Login Screen\n";
	cout << "------------------------------\n";
}

float readPositiveNumber(const string message) {
	float number;

	while (true) {
		cout << message;
		cin >> number;

		if (cin.fail() || cin.peek() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} else if (number > 0 || number == -99)
			return number;
	}
}

string readWord(const string message, const short length) {
	string word;

	while (true) {
		cout << message;
		cin >> word;

		if (cin.peek() != '\n')
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		else if (word.length() <= length)
			return word;
	}
}

short getDigitCount(const string & str) {
	short count = 0;

	for (const char & C : str)
		if (isdigit(C))
			count++;

	return count;
}

string readPhoneNumber(string message) {
	string phoneNumber;

	while (true) {
		cout << message;
		cin >> phoneNumber;

		if (cin.fail() || cin.peek() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} else if (getDigitCount(phoneNumber) == phoneNumber.length() && phoneNumber.length() <= 14)
			return phoneNumber;
	}
}

void printAccountLockedScreen() {
	cout << "This account is locked.\n";
	cout << "Please contact the administrator.";

	cout << "\n\nPress any key to continue...";
	system("pause>nul");
}

vector<string> splitStringToVector(string & str, const string delimiter) {
	unsigned short delimiterPosition = 0;
	vector<string> vector;

	// "kjfdksjksfj#//# osdjfj"
	while ((delimiterPosition = str.find(delimiter)) != (unsigned short) string::npos) {
		vector.push_back(str.substr(0, delimiterPosition));

		str.erase(0, delimiterPosition + delimiter.length());
	}

	vector.push_back(str);

	return vector;
}

sClient convertClientLineToRecord(string & line) {
	vector<string> vctClientData = splitStringToVector(line, "#//#");
	sClient record;

	record.accountNumber = vctClientData[0];
	record.pinCode = vctClientData[1];
	record.name = vctClientData[2];
	record.phoneNumber = vctClientData[3];
	record.accountBalance = stod(vctClientData[4]);
	record.dateCreated = vctClientData[5];
	record.dateModified = vctClientData[6];
	record.markAsLocked = vctClientData[7] == "1";

	return record;
}

short getAccountPosition(const string & accountNumber) {
	for (short i = 0; i < vClients.size(); i++)
		if (vClients[i].accountNumber == accountNumber)
			return i;

	return -1;
}

vector<sClient> loadAllClientsFromFile() {
	fstream clientsFile(clientsFileName, ios::in);
	vector<sClient> vClients;

	if (clientsFile.is_open()) {
		string currentLine;

		while (getline(clientsFile, currentLine))
			vClients.push_back(convertClientLineToRecord(currentLine));

		clientsFile.close();
	} else
		cerr << "Error: Cannot open clients file.";

	return vClients;
}

string convertClientRecordToLine(const sClient & record, const string delimiter) {
	string line = "";

	line += record.accountNumber + delimiter;
	line += record.pinCode + delimiter;
	line += record.name + delimiter;
	line += record.phoneNumber +delimiter;
	line += to_string(record.accountBalance) + delimiter;
	line += record.dateCreated + delimiter;
	line += record.dateModified + delimiter;
	line += record.markAsLocked ? "1" : "0";

	return line;
}

void saveAllClientsToFile() {
	fstream clientsFile(clientsFileName, ios::out);

	if (clientsFile.is_open()) {
		for (const sClient & C : vClients)
			clientsFile << convertClientRecordToLine(C, "#//#") << '\n';
		
		clientsFile.close();
	} else
		cerr << "Error: Cannot open clients file.";
}

string getSystemDateTime() {
	time_t timeNow = time(0);
	tm * timeNowStruct = localtime(&timeNow);
	ostringstream result;

	result << timeNowStruct->tm_mday << '/';
	result << timeNowStruct->tm_mon + 1 << '/';
	result << timeNowStruct->tm_year + 1900 << ' ';
	result << setw(2) << setfill('0') << timeNowStruct->tm_hour << ':';
	result << setw(2) << setfill('0') << timeNowStruct->tm_min << ':';
	result << setw(2) << setfill('0') << timeNowStruct->tm_sec;

	return result.str();
}

short readNumberInRange(const string message, const short from, const short to) {
	short number = 0;

	while (true) {
		cout << message;
		cin >> number;

		if (cin.fail() || cin.peek() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} else if (number >= from && number <= to)
			return number;
	}
}

vector<string> copyFileToVector(const string & fileName) {
	fstream file(fileName, ios::in);
	vector<string> vector;

	if (file.is_open()) {
		string currentLine;

		while (getline(file, currentLine))
			if (currentLine != "")
				vector.push_back(currentLine);

		file.close();
	} else
		cerr << "Error: Cannot open this file.";

	return vector;
}

void insertStringInVector(vector<string> & vct, const short & pos, const string & element) {
	vector<string> vTemp;

	for (short i = 0; i < pos; i++)
		vTemp.push_back(vct.at(i));

	vTemp.push_back(element);

	for (short i = pos; i < vct.size(); i++)
		vTemp.push_back(vct.at(i));

	vct = vTemp;
}

short getNextAccountNumberPosition(vector<string> & vLines, const string & accountNumber) {
	bool hasFoundAccountNumber = false;
	vLines.push_back(" "); //This is crucial in case the accountNumber is the last in file

	for (short i = 0; i < vLines.size(); i++) {
		if (vLines[i] == accountNumber) {
			hasFoundAccountNumber = true;
			continue;
		}

		if (hasFoundAccountNumber && vLines[i].find("#//#") == string::npos) {
			vLines.pop_back(); //To get rid of that white space
			return i;
		}

	}

	return -1; //This will never be reached anyways
}

string convertClientHistoryRecordToLine(const sClient & client, const string delimiter, const enClientUpdateType actionDone) {
	string result = "";

	if (client.dateModified == "-")
		result += client.dateCreated + delimiter;
	else
		result += client.dateModified + delimiter;

	result += client.pinCode + delimiter;
	result += client.name + delimiter;
	result += client.phoneNumber + delimiter;
	result += to_string(client.accountBalance) + delimiter;
	result += to_string(actionDone);

	return result;
}

void copyVectorToFile(const string & fileName, vector<string> & vector) {
	fstream clientsFile(fileName, ios::out);

	if (clientsFile.is_open()) {
		for (string & currentLine : vector)
			clientsFile << currentLine << '\n';

		clientsFile.close();
	} else
		cerr << "Error: Cannot open this file.";
}

void insertClientRecordInHistoryFile(const sClient & client, const enClientUpdateType actionDone) {
	vector<string> vAllLines = copyFileToVector(clientsHistoryFileName);

	insertStringInVector(vAllLines, getNextAccountNumberPosition(vAllLines, client.accountNumber), convertClientHistoryRecordToLine(client, "#//#", actionDone));

	copyVectorToFile(clientsHistoryFileName, vAllLines);
}

void printMainMenuOptions() {
	cout << "=======================================================================\n";
	cout << "                           Main Menu Screen\n";
	cout << "=======================================================================\n";
	cout << "\t[1]: Quick Withdraw.                [2]: Normal Withdraw.\n";
	cout << "\t[3]: Deposit.                       [4]: Transfer Money.\n";
	cout << "\t[5]: Check Messages.                [6]: Check Balance.\n";
	cout << "\t[7]: Check Account History.         [8]: Change Phone Number.\n";
	cout << "\t[9]: Change PIN Code.               [10]: Logout.\n";
	cout << "=======================================================================\n";
}

void printQuickWithdrawOptions() {
	cout << "-----------------------------------------\n";
	cout << "             Quick Withdraw\n";
	cout << "-----------------------------------------\n";
	cout << "        [1]: 20         [2]: 50\n";
	cout << "        [3]: 100        [4]: 200\n";
	cout << "        [5]: 400        [6]: 600\n";
	cout << "        [7]: 800        [8]: 1000\n";
	cout << "        [9]: Exit\n";
	cout << "-----------------------------------------\n";
}

void backToMainMenuScreen() {
	cout << "\nPress any key to return to Main Menu Screen...";
	system("pause>nul");
	displayMainMenuScreen();
}

bool readBoolean(const string message) {
	char choice = ' ';

	while (true) {
		cout << message << " [Y] - [N]: ";
		cin >> choice;

		if (cin.fail() || cin.peek() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} else
			return tolower(choice) == 'y';
	}
}

void depositAmount(const float amount) {
	vClients[activeClientPosition].accountBalance += amount;
	vClients[activeClientPosition].dateModified = getSystemDateTime();

	saveAllClientsToFile();
	insertClientRecordInHistoryFile(vClients[activeClientPosition], amount > 0 ? enClientUpdateType::eOnlyDeposit : enClientUpdateType::eOnlyWithdraw);
}

void displayQuickWithdrawScreen() {
	system("cls");
	printQuickWithdrawOptions();

	cout << "Your Balance is " << vClients[activeClientPosition].accountBalance << "\n\n";

	short amounts[8] = {20, 50, 100, 200, 400, 600, 800, 1000}, userChoice;
	
	while (true) {
		userChoice = readNumberInRange("Choose an option [1] to [9]: ", 1, 9);

		if (userChoice != 9) {
			if (vClients[activeClientPosition].accountBalance >= amounts[userChoice - 1]) {
				if (readBoolean("\nAre you sure you want to perform this transaction?")) {
					depositAmount(-amounts[userChoice - 1]);

					cout << "\nDone Successfully.";
					cout << "\nYour balance is now " << vClients[activeClientPosition].accountBalance << endl;

					backToMainMenuScreen();
				}

				break;
			} else
				cout << "\nYou exceeded your balance, try again.\n";
		} else {
			displayMainMenuScreen();
			break;
		}
	}
}

void printNormalWithdrawScreenHeader() {
	cout << "* You can enter [-99] to cancel.\n";
	cout << "-----------------------------------------\n";
	cout << "             Normal Withdraw\n";
	cout << "-----------------------------------------\n";
}

int readWithdrawAmount(const string message) {
	int amount;

	while (true) {
		cout << message;
		cin >> amount;

		if (cin.fail() != cin.peek() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} else if (amount % 5 == 0)
			return amount;
	}
}

void displayNormalWithdrawScreen() {
	system("cls");
	printNormalWithdrawScreenHeader();

	cout << "Your Balance is " << vClients[activeClientPosition].accountBalance << "\n\n";
	
	int amount;

	while (true) {
		amount = readWithdrawAmount("Enter amount (multiple of 5): ");

		if (amount == -99)
			break;

		if (vClients[activeClientPosition].accountBalance >= amount) {
			if (readBoolean("\nAre you sure you want to perform this transaction?")) {
				depositAmount(-amount);

				cout << "\nDone Successfully.";
				cout << "\nyour balance is now " << vClients[activeClientPosition].accountBalance << endl;
			}

			break;
		} else 
			cout << "\nYou exceeded your balance, try again.\n";
	}

	backToMainMenuScreen();
}

void printDepositScreenHeader() {
	cout << "* You can enter [-99] to cancel.\n";
	cout << "-----------------------------------------\n";
	cout << "                 Deposit\n";
	cout << "-----------------------------------------\n";
}

void displayDepositScreen() {
	system("cls");
	printDepositScreenHeader();

	cout << "Your Balance is " << vClients[activeClientPosition].accountBalance << "\n\n";

	short amount = readPositiveNumber("Enter amount: ");

	if (amount != -99)
		if (readBoolean("\nAre you sure you want to perform this transaction?")) {
			depositAmount(amount);

			cout << "\nDone successfully.";
			cout << "\nYour balance is now " << vClients[activeClientPosition].accountBalance << endl;
		}

	backToMainMenuScreen();
}

void printTransferMoneyScreenHeader() {
	cout << "* You can enter [-99] to cancel.\n";
	cout << "-----------------------------------------\n";
	cout << "              Transfer Money\n";
	cout << "-----------------------------------------\n";
}

void transferAmount(const float & amount, const short & targetAccountPosition) {
	vClients[activeClientPosition].accountBalance -= amount;
	vClients[activeClientPosition].dateModified = getSystemDateTime();
	vClients[targetAccountPosition].accountBalance += amount;
	vClients[targetAccountPosition].dateModified = getSystemDateTime();

	saveAllClientsToFile();
	insertClientRecordInHistoryFile(vClients[activeClientPosition], enClientUpdateType::eTransferedMoney);
	insertClientRecordInHistoryFile(vClients[targetAccountPosition], enClientUpdateType::eReceivedMoney);
}

void displayTransferMoneyScreen() {
	system("cls");
	printTransferMoneyScreenHeader();

	cout << "Your Balance is " << vClients[activeClientPosition].accountBalance << "\n\n";

	bool shouldReadAmount = true;

	//Reading target accountNumber:
	string targetAccountNumber;
	short targetAccountPosition;

	while (true) {
		targetAccountNumber = readWord("Enter account number: ", 17);

		if (targetAccountNumber == "-99") {
			shouldReadAmount = false;
			break;
		}

		if (targetAccountNumber == vClients[activeClientPosition].accountNumber) {
			cout << "You cannot transfer to your own account, try again.\n\n";
			continue;
		}

		targetAccountPosition = getAccountPosition(targetAccountNumber);

		if (targetAccountPosition == -1)
			cout << "Account number [" << targetAccountNumber << "] does not exist.\n\n";
		else {
			cout << '\n';
			break;
		}
	}

	if (shouldReadAmount) {
		short amount;

		while (true) {
			amount = readPositiveNumber("Enter amount: ");

			if (amount <= vClients[activeClientPosition].accountBalance) {
				if (readBoolean("\nAre you sure you want to perform this transaction?")) {
					transferAmount(amount, targetAccountPosition);

					cout << "\nDone Successfully.";
					cout << "\nYour balance is now " << vClients[activeClientPosition].accountBalance << endl;
				}

				break;
			} else {
				cout << "You exceeded you balance, try again.\n\n";
			}
		}
	}

	backToMainMenuScreen();
}

string replaceAllInString(string str, string replaceFrom, const string replaceTo) {
	unsigned short pos = 0;

	while ((pos = str.find(replaceFrom, pos)) != (unsigned short) string::npos) {
		str.replace(pos, replaceFrom.length(), replaceTo);

		pos += replaceTo.length();
	}

	return str;
}

vector<string> getAllMessages(const string & accountNumber) {
	fstream messagesFile(messageFileName, ios::in);
	vector<string> vMessages;

	if (messagesFile.is_open()) {
		string currentLine;
		short delimiterPosition;

		while (getline(messagesFile, currentLine)) {
			if (currentLine != "") {
				delimiterPosition = currentLine.find("#//#");

				if (currentLine.substr(0, delimiterPosition) == accountNumber)
					vMessages.push_back(replaceAllInString(currentLine.substr(delimiterPosition + 4, currentLine.length() - delimiterPosition - 4), "\\n", "\n"));
			}
		}

		messagesFile.close();
	} else
		cerr << "Error: Cannot open messages file.";

	return vMessages;
}

void displayCheckMessagesScreen() {
	system("cls");

	vector<string> vMessages = getAllMessages(vClients[activeClientPosition].accountNumber);

	if (vMessages.empty())
		cout << "You don't have any messages.\n";
	else {
		cout << "You received " << vMessages.size() << " message(s):\n";

		for (const string & M : vMessages) {
			cout << "-------------------------------------\n";
			cout << M << "\n";
		}

		cout << "-------------------------------------\n";
	}
	
	backToMainMenuScreen();
}

void displayCheckBalanceScreen() {
	system("cls");

	cout << "Your balance is " << vClients[activeClientPosition].accountBalance << '\n';

	backToMainMenuScreen();
}

sClientForHistory convertClientHistoryLineToRecord(string & line) {
	vector<string> vHistoryRecordData = splitStringToVector(line, "#//#");
	sClientForHistory clientHistoryPoint;

	clientHistoryPoint.date = vHistoryRecordData[0];
	clientHistoryPoint.pinCode = vHistoryRecordData[1];
	clientHistoryPoint.name = vHistoryRecordData[2];
	clientHistoryPoint.phoneNumber = vHistoryRecordData[3];
	clientHistoryPoint.accountBalance = stod(vHistoryRecordData[4]);
	clientHistoryPoint.actionDone = (enClientUpdateType) stoi(vHistoryRecordData[5]);

	return clientHistoryPoint;
}

vector<sClientForHistory> loadClientRecordsFromHistoryFile() {
	fstream historyFile(clientsHistoryFileName, ios::in);
	vector<sClientForHistory> vClientHistory;

	if (historyFile.is_open()) {
		string currentLine = "";
		bool shouldPushBack = false;

		while (getline(historyFile, currentLine)) {
			if (shouldPushBack) {
				if (currentLine.find("#//#") == string::npos)
					break;
				else
					vClientHistory.push_back(convertClientHistoryLineToRecord(currentLine));

			} else if (currentLine == vClients[activeClientPosition].accountNumber)
				shouldPushBack = true;
		}

		historyFile.close();
	} else
		cerr << "Error: Cannot open clients file.";

	return vClientHistory;
}

string getClientUpdateTypeText(const enClientUpdateType & updateType) {
	switch (updateType) {
		case enClientUpdateType::eClientCreated: return "Account Created";
		case enClientUpdateType::eFullUpdate: return "Full Update";
		case enClientUpdateType::eOnlyDeposit: return "Deposit";
		case enClientUpdateType::eOnlyWithdraw: return "Withdraw";
		case enClientUpdateType::eTransferedMoney: return "Transfered Money";
		case enClientUpdateType::eReceivedMoney: return "Received Money";
		case enClientUpdateType::eChangedPhoneNumber: return "Changed Phone Number";
		case enClientUpdateType::eChangedPinCode: return "Changed PIN Code";
		case enClientUpdateType::eAccountLocked: return "Account Locked";
		case enClientUpdateType::eAccountUnlocked: return "Account Unlocked";
	}
}

void printAccountHistoryHeader() {
	cout << "\t\t\t\t\t\t\t" << vClients[activeClientPosition].accountNumber;

	cout << "\n------------------------------------------------------------------------------------------------------------------------";
	cout << "\n|        Date        |  PIN Code  |            Name            |     Phone     |    Balance    |      Action Done      |";
	cout << "\n------------------------------------------------------------------------------------------------------------------------" << endl;
}

void printAccountHistoryData() {
	vector<sClientForHistory> vClientRecords = loadClientRecordsFromHistoryFile();

	for (sClientForHistory & R : vClientRecords) {
		cout << "| " << setw(19) << left << R.date;
		cout << "| " << setw(11) << left << R.pinCode;
		cout << "| " << setw(27) << left << R.name;
		cout << "| " << setw(14) << left << R.phoneNumber;
		cout << "| " << setw(14) << left << R.accountBalance;
		cout << "| " << setw(22) << left << getClientUpdateTypeText(R.actionDone) << "|\n";
	}

	cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
}

void displayCheckAccountHistoryScreen() {
	system("cls");

	printAccountHistoryHeader();

	printAccountHistoryData();

	backToMainMenuScreen();
}

void printChangePhoneNumberScreenHeader() {
	cout << "------------------------------\n";
	cout << "     Change Phone Number\n";
	cout << "------------------------------\n";
}

void printChangePinCodeScreenHeader() {
	cout << "------------------------------\n";
	cout << "       Change PIN Code        \n";
	cout << "------------------------------\n";
}

void displayChangePhoneNumberScreen() {
	string phoneNumber;
	bool hasEnteredWrongly = false;

	while (true) {
		system("cls");
		printChangePhoneNumberScreenHeader();

		if (hasEnteredWrongly)
			cout << "Wrong phone number, try again.\n\n";

		phoneNumber = readPhoneNumber("Enter your current phone number:\n");

		if (phoneNumber == "-99")
			break;

		if (phoneNumber == vClients[activeClientPosition].phoneNumber) {
			phoneNumber = readPhoneNumber("Enter the new phone number:\n");

			vClients[activeClientPosition].phoneNumber = phoneNumber;
			saveAllClientsToFile();
			insertClientRecordInHistoryFile(vClients[activeClientPosition], enClientUpdateType::eChangedPhoneNumber);

			cout << "\nDone successfully.";
			backToMainMenuScreen();

			break;
		} else
			hasEnteredWrongly = true;
	}
}

void displayChangePinCodeScreen() {
	string pinCode;
	bool hasEnteredWrongly = false;

	while (true) {
		system("cls");
		printChangePinCodeScreenHeader();

		if (hasEnteredWrongly)
			cout << "Wrong PIN code, try again.\n\n";

		pinCode = readWord("Enter your current PIN code:\n", 100);

		if (pinCode == "-99")
			break;

		if (pinCode == vClients[activeClientPosition].pinCode) {
			pinCode = readWord("Enter the new PIN code:\n", 17);

			vClients[activeClientPosition].pinCode = pinCode;
			saveAllClientsToFile();
			insertClientRecordInHistoryFile(vClients[activeClientPosition], enClientUpdateType::eChangedPinCode);

			cout << "\nDone successfully.";
			backToMainMenuScreen();

			break;
		} else
			hasEnteredWrongly = true;
	}
}

void performMainMenuOption(const enMainMenuOptions option) {
	switch (option) {
		case enMainMenuOptions::eQuickWithdraw:
			displayQuickWithdrawScreen();
			break;
		case enMainMenuOptions::eNormalWithdraw:
			displayNormalWithdrawScreen();
			break;
		case enMainMenuOptions::eDeposit:
			displayDepositScreen();
			break;
		case enMainMenuOptions::eTransferMoney:
			displayTransferMoneyScreen();
			break;
		case enMainMenuOptions::eCheckMessages:
			displayCheckMessagesScreen();
			break;
		case enMainMenuOptions::eCheckBalance:
			displayCheckBalanceScreen();
			break;
		case enMainMenuOptions::eCheckAccountHistory:
			displayCheckAccountHistoryScreen();
			break;
		case enMainMenuOptions::eChangePhoneNumber:
			displayChangePhoneNumberScreen();
			break;
		case enMainMenuOptions::eChangePINCode:
			displayChangePinCodeScreen();
			break;
		default:
			displayLoginScreen();
	}
}

void displayMainMenuScreen() {
	system("cls");
	printMainMenuOptions();

	performMainMenuOption((enMainMenuOptions) readNumberInRange("Choose [1] to [10]: ", 1, 10));
}

void displayLoginScreen() {
	bool loginFailed = true; //This flag controls everything
	string accountNumber = "";
	string pinCode = "";
	vClients = loadAllClientsFromFile();

	while (loginFailed) {
		loginFailed = false;

		//Reading account number:
		do {
			system("cls");
			printLoginScreenHeader();

			if (loginFailed)
				cout << "Account Number [" << accountNumber << "] does not exist.\n\n";

			accountNumber = readWord("Enter account number:\n", 17);

			activeClientPosition = getAccountPosition(accountNumber);

			loginFailed = activeClientPosition == -1;
		} while (loginFailed);

		if (vClients[activeClientPosition].markAsLocked) {
			system("cls");

			cout << "Welcome, " << vClients[activeClientPosition].name << "!\n\n";
			printAccountLockedScreen();

			loginFailed = true;
		} else {
			//Reading pin code:
			short loginFailedTimes = 0;
			loginFailed = true;

			while (loginFailedTimes < 3) {
				system("cls");
				cout << "Welcome, " << vClients[activeClientPosition].name << "!\n\n";

				if (loginFailedTimes > 0)
					cout << "Wrong PIN code! You have " << 3 - loginFailedTimes << " trial(s) left.\n\n";
				
				pinCode = readWord("Enter PIN code:\n", 17);

				if (pinCode == vClients[activeClientPosition].pinCode)
					break;
				else
					loginFailedTimes++;
			}

			if (loginFailedTimes == 3) {
				vClients[activeClientPosition].markAsLocked = true;
				vClients[activeClientPosition].dateModified = getSystemDateTime();

				saveAllClientsToFile();
				insertClientRecordInHistoryFile(vClients[activeClientPosition], enClientUpdateType::eAccountLocked);

				system("cls");

				cout << "Welcome, " << vClients[activeClientPosition].name << "!\n\n";
				printAccountLockedScreen();
			} else
				break;
		}
	}

	//This part will never be reached unless a successful login occurs
	displayMainMenuScreen();
}

int main() {
	displayLoginScreen();

    return 0;
}

