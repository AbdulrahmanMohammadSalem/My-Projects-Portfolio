#pragma execution_character_set("utf-8")

#include <windows.h>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

enum enResult { User, Computer, Tie };
enum enMove { Rock = 1, Paper = 2, Scissors = 3 };

struct stRoundVars {
    enResult roundResult;
    enMove userMove, computerMove;
};

struct stGameVars {
    short userWonTimes, computerWonTimes, tieTimes, numOfRounds;
};

short readNumInRng(const string msg, const short from, const short to) {
    short num = 0;

    do {
        cout << msg << endl;
        cin >> num;
    } while (num < from || num > to);

    return num;
}
short genRndNum(const short from, const short to) {
    return rand() % (to - from + 1) + from;
}

bool shouldStartNewGame(string msg) {
    string choice = "";

    do {
        cout << msg;
        cin >> choice;
    } while (choice != "Y" && choice != "y" && choice != "N" && choice != "n");
    
    return choice == "Y" || choice == "y";
}

enMove readMove(const string msg) {
    short move;

    do {
        cout << msg;
        cin >> move;
    } while (move < 1 || move > 3);

    return (enMove) move;
}
enMove chooseRndMove() {
    return (enMove) genRndNum(1, 3);
}

enResult getRoundResult(const enMove userMove, const enMove computerMove) {
    if (userMove == computerMove)
        return enResult::Tie;
   
    if (userMove == computerMove + 1 || (userMove == enMove::Rock && computerMove == enMove::Scissors))
        return enResult::User;
 
    return enResult::Computer;
}

void printRoundResult(const stRoundVars& roundVars, const short currRound, stGameVars& currGameVars) {
    switch (roundVars.roundResult) {
        case User:
            system("color 2F");
            currGameVars.userWonTimes++;
            break;
        case Computer:
            system("color 4F");
            cout << '\a';
            currGameVars.computerWonTimes++;
            break;
        case Tie:
            system("color 6F");
            currGameVars.tieTimes++;
            break;
    }

    string moves[3] = {"Rock", "Paper", "Scissors"};
    string winner[3] = {"Player", "Computer", "No Winner"};

    cout << "\n----------------Round[" << currRound << "]----------------";
    cout << "\nPlayer Choice   : " << moves[roundVars.userMove - 1];
    cout << "\nComputer Choice : " << moves[roundVars.computerMove - 1];
    cout << "\nRound Winner    : [" << winner[roundVars.roundResult] << "]";
    cout << "\n----------------------------------------\n" << endl;
}

void startRound(const short currRound, stGameVars& currGameVars) {
    system("color 0F");
    system("cls");

    cout << "Round [" << currRound << '/' << currGameVars.numOfRounds << "] Begins:\n\n";

    stRoundVars roundVars;

    roundVars.userMove = readMove("Your Choice -> [1]: Rock, [2]: Paper, [3]: Scissors ? ");
    roundVars.computerMove = chooseRndMove();
    roundVars.roundResult = getRoundResult(roundVars.userMove, roundVars.computerMove);
    
    printRoundResult(roundVars, currRound, currGameVars);

    cout << "Press any key to continue...";
    system("pause>nul");
}

void printGameResult(const stGameVars currGameVars) {
    system("cls");

    string finalWinner = "";

    if (currGameVars.userWonTimes > currGameVars.computerWonTimes) {
        system("color 2F");
        finalWinner += "Player";
    } else if (currGameVars.computerWonTimes > currGameVars.userWonTimes) {
        system("color 4f");
        finalWinner += "Computer";
    } else {
        system("color 6F");
        finalWinner += "Tie";
    }

    cout << "\t\t\t-------------------------------------------------\n";
    cout << "\t\t\t                +++ Game Over +++\n";
    cout << "\t\t\t-------------------------------------------------\n\n";
    cout << "\t\t\t┌-----------------[Game Results]----------------┐\n";
    cout << "\t\t\t| Game Rounds        : " << currGameVars.numOfRounds << "\t\t\t|\n";
    cout << "\t\t\t| Player Won Times   : " << currGameVars.userWonTimes << "\t\t\t|\n";
    cout << "\t\t\t| Computer Won Times : " << currGameVars.computerWonTimes << "\t\t\t|\n";
    cout << "\t\t\t| Draw Times         : " << currGameVars.tieTimes << "\t\t\t|\n";
    cout << "\t\t\t| Final Winner       : " << finalWinner << "\t\t\t|\n";
    cout << "\t\t\t└-----------------------------------------------┘\n" << endl;
}

void startGame() {
    stGameVars currGameVars;

    currGameVars.numOfRounds = readNumInRng("How many rounds 1 to 10 ?", 1, 10);
    currGameVars.userWonTimes = 0;
    currGameVars.computerWonTimes = 0;
    currGameVars.tieTimes = 0;

    for (short currRound = 1; currRound <= currGameVars.numOfRounds; currRound++) {
        startRound(currRound, currGameVars);
    }

    printGameResult(currGameVars);

    if (shouldStartNewGame("\t\t\tStart a new game? [Y/N]\n\t\t\t")) {
        system("cls");
        system("color 0F");
        startGame();
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand((unsigned) time(NULL));

    startGame();
    
    return 0;
}
