// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

#include <iostream>
#include <cstdlib>

using namespace std;

enum enLvl { Easy = 1, Med = 2, Hard = 3, LvlMix = 4 };
enum enOpType { Add = 1, Sub = 2, Mult = 3, Div = 4, OpMix = 5 };

struct stQuestionInfo {
    short firstNum = 0;
    short secondNum = 0;
    enLvl questionLvl;
    enOpType questionOpType;
    int realSolution = 0;
    int userSolution = 0;
    bool hasBeenAnsweredCorrectly = false;
};

struct stQuizInfo {
    stQuestionInfo questionsList[100];
    short numOfQuestions = 0;
    enLvl questionsLvl;
    enOpType quizOpType;
    short numOfRightAnswers = 0;
    short numOfWrongAnswers = 0;
    bool isPass = false;
};

void resetScreen() {
    system("cls");
    system("color 0F");
}

short readHowManyQuestions() {
    short quizLng = 0;
    string msg = "How many quesions do you want to answer? ";
    do {
        cout << msg;
        cin >> quizLng;
        msg = "  Enter Only From 1 to 100: ";
    } while (quizLng < 1 || quizLng > 100);
    return quizLng;
}

enLvl readQuizLvl() {
    short lvl = 0;
    do {
        cout << "Choose Questions Level -> [1]: Easy, [2]: Med, [3]: Hard, [4]: Mix ? ";
        cin >> lvl;
    } while (lvl < 1 || lvl > 4);
    return (enLvl) lvl;
}

enOpType readQuizOpType() {
    short opType = 0;
    do {
        cout << "Choose Questions Operation Type -> [1]: Add, [2]: Sub, [3]: Mult, [4]: Div, [5]: Mix ? ";
        cin >> opType;
    } while (opType < 1 || opType > 5);
    return (enOpType) opType;
}

short generateRndNum(const short from, const short to) {
    return rand() % (to - from + 1) + from;
}

int simpleCalculator(const short firstNum, const short secondNum, const enOpType opType) {
    switch (opType) {
        case enOpType::Add:
            return firstNum + secondNum;
        case enOpType::Sub:
            return firstNum - secondNum;
        case enOpType::Mult:
            return firstNum * secondNum;
        case enOpType::Div:
            return firstNum / secondNum;
    }
}

stQuestionInfo generateQuestion(enLvl questionLvl, enOpType questionOpType) {
    stQuestionInfo question;

    if (questionLvl == enLvl::LvlMix)
        questionLvl = (enLvl) generateRndNum(1, 3);
    if (questionOpType == enOpType::OpMix)
        questionOpType = (enOpType) generateRndNum(1, 4);

    question.questionLvl = questionLvl;
    question.questionOpType = questionOpType;

    switch (question.questionLvl) {
        case enLvl::Easy:
            question.firstNum = generateRndNum(1, 20);
            question.secondNum = generateRndNum(1, 20);
            break;
        case enLvl::Med:
            question.firstNum = generateRndNum(20, 100);
            question.secondNum = generateRndNum(20, 100);
            break;
        case enLvl::Hard:
            question.firstNum = generateRndNum(100, 600);
            question.secondNum = generateRndNum(100, 600);
            break;
    }

    question.realSolution = simpleCalculator(question.firstNum, question.secondNum, questionOpType);
    
    return question;
}

void generateQuizQuestions(stQuizInfo & quiz) {
    for (short currQuestion = 0; currQuestion < quiz.numOfQuestions; currQuestion++)
        quiz.questionsList[currQuestion] = generateQuestion(quiz.questionsLvl, quiz.quizOpType);
}

string getOperationTypeSymbol(const enOpType opType) {
    switch (opType) {
        case enOpType::Add:
            return "+";
        case enOpType::Sub:
            return "-";
        case enOpType::Mult:
            return "x";
        case enOpType::Div:
            return "/";
        default:
            return "Mix";
    }
}

void printQuestion(const stQuizInfo quiz, const short questionOrder) {
    cout << endl << "Question [" << questionOrder + 1 << '/' << quiz.numOfQuestions << "]\n";
    cout << quiz.questionsList[questionOrder].firstNum << '\n';
    cout << quiz.questionsList[questionOrder].secondNum << ' ';
    cout << getOperationTypeSymbol(quiz.questionsList[questionOrder].questionOpType) << '\n';
    cout << "------------" << endl;
}

int readUserSolution() {
    int userSolution = 0;
    cin >> userSolution;
    return userSolution;
}

void correctQuestionAnswer(stQuizInfo & quiz, const short questionOrder) {
    if (quiz.questionsList[questionOrder].userSolution != quiz.questionsList[questionOrder].realSolution) {
        quiz.questionsList[questionOrder].hasBeenAnsweredCorrectly = false;
        quiz.numOfWrongAnswers++;

        system("color 4F");
        cout << '\a';
        cout << "Wrong Answer :-(\n";
        cout << "The Right Answer = " << quiz.questionsList[questionOrder].realSolution;
    } else {
        quiz.questionsList[questionOrder].hasBeenAnsweredCorrectly = true;
        quiz.numOfRightAnswers++;

        system("color 2F");
        cout << "Right Answer :-)";
    }

    cout << endl;
}

void askAndCorrectQuestionsListAnswers(stQuizInfo& quiz) {
    for (short currQuestion = 0; currQuestion < quiz.numOfQuestions; currQuestion++) {
        printQuestion(quiz, currQuestion);

        quiz.questionsList[currQuestion].userSolution = readUserSolution();

        correctQuestionAnswer(quiz, currQuestion);
    }

    quiz.isPass = quiz.numOfRightAnswers >= quiz.numOfWrongAnswers;
}

string getResultText(const bool isPass) {
    return isPass ? "PASS :-)" : "FAIL :-(";
}

string getLvlText(const enLvl lvl) {
    switch (lvl) {
        case enLvl::Easy:
            return "Easy";
        case enLvl::Med:
            return "Med";
        case enLvl::Hard:
            return "Hard";
        default:
            return "Mix";
    }
}

void printQuizResults(const stQuizInfo quiz) {
    cout << "\n----------------------------------";
    cout << "\n     Final Result is " << getResultText(quiz.isPass);
    cout << "\n----------------------------------";
    cout << "\nNumber of Questions : " << quiz.numOfQuestions;
    cout << "\nLevel of Questions  : " << getLvlText(quiz.questionsLvl);
    cout << "\nOperation Type      : " << getOperationTypeSymbol(quiz.quizOpType);
    cout << "\nRight Answers : " << quiz.numOfRightAnswers;
    cout << "\nWrong Answers : " << quiz.numOfWrongAnswers;
    cout << "\n----------------------------------";
}

void playMathGame() {
    stQuizInfo quiz;

    quiz.numOfQuestions = readHowManyQuestions();
    quiz.questionsLvl = readQuizLvl();
    quiz.quizOpType = readQuizOpType();

    generateQuizQuestions(quiz);
    askAndCorrectQuestionsListAnswers(quiz);
    printQuizResults(quiz);
}

void startGame() {
    char shouldPlayAgain = 'Y';

    do {
        resetScreen();
        playMathGame();

        cout << endl << "Do you want to play again (Y/N) ? ";
        cin >> shouldPlayAgain;
    } while (shouldPlayAgain == 'Y' || shouldPlayAgain == 'y');
}

int main() {
    srand((unsigned) time(NULL));

    startGame();
    
    return 0;
}

