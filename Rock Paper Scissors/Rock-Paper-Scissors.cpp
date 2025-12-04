// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

#include <iostream>
#include <cstdlib>

using namespace std;

enum enWinner { User = 1, Computer = 2, Draw = 3 };

enum enGameChoice { Stone = 1, Paper = 2, Scissors = 3 };

struct stGameResults {
	short numOfRounds = 0;
	short playerWonTimes = 0;
	short computerWonTimes = 0;
	short drawTimes = 0;
	enWinner gameWinner;
	string gameWinnerName = "";
};

struct stRoundInfo {
	short roundOrder = 0;
	enGameChoice playerChoice;
	enGameChoice computerChoice;
	enWinner roundWinner;
	string roundWinnerName = "";
};

short askHowManyRounds() {
	short howManyRounds = 0;

	do {
		cout << "How many rounds 1 to 10 ?\n";
		cin >> howManyRounds;
	} while (howManyRounds > 10 || howManyRounds < 1);

	return howManyRounds;
}

void resetScreen() {
	system("cls");
	system("color 0F");
}

short genRndNum(const short from, const short to) {
	return rand() % (to - from + 1) + from;
}

enGameChoice readPlayerChoice() {
	short playerChoice = 0;

	do {
		cout << "Your Choice -> [1]: Stone, [2]: Paper, [3]: Scissors ? ";
		cin >> playerChoice;
	} while (playerChoice > 3 || playerChoice < 1);

	return (enGameChoice) playerChoice;
}

enGameChoice getComputerChoice() {
	return (enGameChoice) genRndNum(1, 3);
}

enWinner getRoundWinner(const stRoundInfo roundInfo) {
	if (roundInfo.playerChoice == roundInfo.computerChoice)
		return enWinner::Draw;

	switch (roundInfo.playerChoice) {
		case enGameChoice::Stone:
			if (roundInfo.computerChoice == enGameChoice::Paper)
				return enWinner::Computer;
			break;
		case enGameChoice::Paper:
			if (roundInfo.computerChoice == enGameChoice::Scissors)
				return enWinner::Computer;
			break;
		case enGameChoice::Scissors:
			if (roundInfo.computerChoice == enGameChoice::Stone)
				return enWinner::Computer;
			break;
	}

	return enWinner::User;
}

string getWinnerName(const enWinner winner) {
	string arrWinners[3] = {"Player", "Computer", "Draw"};
	return arrWinners[winner - 1];
}

string getChoiceName(const enGameChoice choice) {
	const string arrChoices[3] = {"Stone", "Paper", "Scissors"};
	return arrChoices[choice - 1];
}

void printRoundInfo(const stRoundInfo roundInfo) {
	cout << "\n-----------Round [" << roundInfo.roundOrder << "]-----------";
	cout << "\nPlayer Choice   : " << getChoiceName(roundInfo.playerChoice);
	cout << "\nComputer Choice : " << getChoiceName(roundInfo.computerChoice);
	cout << "\nRound Winner    : " << getWinnerName(roundInfo.roundWinner);
	cout << "\n-------------------------------" << endl;
}

void setScreenColorByWinner(const enWinner winner) {
	switch (winner) {
		case enWinner::User:
			system("color 2F");
			break;
		case enWinner::Computer:
			system("color 4F");
			cout << '\a';
			break;
		default:
			system("color 6F");
	}
}

enWinner getGameWinner(const short playerWonTimes, const short computerWonTimes) {
	if (playerWonTimes > computerWonTimes)
		return enWinner::User;
	if (computerWonTimes > playerWonTimes)
		return enWinner::Computer;
	return enWinner::Draw;
}

stGameResults getGameResults(const short numOfRounds, const short playerWonTimes, const short computerWonTimes, const short drawTimes) {
	stGameResults gameResults;

	gameResults.numOfRounds = numOfRounds;
	gameResults.playerWonTimes = playerWonTimes;
	gameResults.computerWonTimes = computerWonTimes;
	gameResults.drawTimes = drawTimes;
	gameResults.gameWinner = getGameWinner(playerWonTimes, computerWonTimes);
	gameResults.gameWinnerName = getWinnerName(gameResults.gameWinner);

	return gameResults;
}

stGameResults playGame(const short numOfRounds) {
	stRoundInfo roundInfo;
	short playerWonTimes = 0, computerWonTimes = 0, drawTimes = 0;

	for (short currRound = 1; currRound <= numOfRounds; currRound++) {
		cout << "\nRound [" << currRound << "] Begins:\n";

		roundInfo.roundOrder = currRound;
		roundInfo.playerChoice = readPlayerChoice();
		roundInfo.computerChoice = getComputerChoice();
		roundInfo.roundWinner = getRoundWinner(roundInfo);
		roundInfo.roundWinnerName = getWinnerName(roundInfo.roundWinner);

		if (roundInfo.roundWinner == enWinner::User)
			playerWonTimes++;
		else if (roundInfo.roundWinner == enWinner::Computer)
			computerWonTimes++;
		else
			drawTimes++;

		printRoundInfo(roundInfo);
		setScreenColorByWinner(roundInfo.roundWinner);
	}

	return getGameResults(numOfRounds, playerWonTimes, computerWonTimes, drawTimes);
}

string tabs(short numOfTabs) {
	string tabs = "";
	for (numOfTabs; numOfTabs > 0; numOfTabs--)
		tabs += "\t";
	return tabs;
}

void printGameOverScreen() {
	cout << "\n" << tabs(4) << "-------------------------------------";
	cout << "\n" << tabs(4) << "          +++ Game Over +++";
	cout << "\n" << tabs(4) << "-------------------------------------";
}

void printFinalGameResults(const stGameResults gameResults) {
	setScreenColorByWinner(gameResults.gameWinner);

	cout << "\n" << tabs(4) << "Number of Rounds   : " << gameResults.numOfRounds;
	cout << "\n" << tabs(4) << "Player Won Times   : " << gameResults.playerWonTimes;
	cout << "\n" << tabs(4) << "Computer Won Times : " << gameResults.computerWonTimes;
	cout << "\n" << tabs(4) << "Draw Times         : " << gameResults.drawTimes;
	cout << "\n" << tabs(4) << "Final Winner : " << gameResults.gameWinnerName;
	cout << "\n" << tabs(4) << "-------------------------------------" << endl;
}

void askTheUserToPlayAgain(char& playAgain) {
	do {
		cout << tabs(4) << "Do you want to play again (Y/N) ? ";
		cin >> playAgain;
	} while (playAgain != 'Y' && playAgain != 'N' && playAgain != 'y' && playAgain != 'n');
}

void startGame() {
	char playAgain = 'Y';

	do {
		resetScreen();
		stGameResults gameResults = playGame(askHowManyRounds());
		printGameOverScreen();
		printFinalGameResults(gameResults);

		askTheUserToPlayAgain(playAgain);
	} while (playAgain == 'Y' || playAgain == 'y');
}

int main() {
	srand((unsigned) time(NULL));

	startGame();

	return 0;
}

