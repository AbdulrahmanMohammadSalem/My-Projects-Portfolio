#include "B:\PROGRAMMING\Abu-Hadhoud\My Libraries\Readers.h"
#include "B:\PROGRAMMING\Abu-Hadhoud\My Libraries\Dates.h"
#include "B:\PROGRAMMING\Abu-Hadhoud\My Libraries\files.h"

using namespace dates;

const string settingsFileName = "Settings.txt";

struct sSettings {
    string format = "dd/mm/yyyy";
    enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday;
    enWeekDays endOfWeek = enWeekDays::eFri;
};

sSettings globalSettings;

enum enMainMenuActions {
    eCheckLeapYear = 1, eCompareTwoDates, eShowYearData, eShowMonthData, eShowDayData,
    eWeekendChecker, eGetVacationDays, eGetVacationReturnDate, eGetDayNameFromDate,
    eDoesDateCoincideWith, eDisplayMonthCalendar, eDisplayYearCalendar, eGetDateBeforeSomeDays,
    eGetDateAfterSomeDays, eGetDateBeforeSomeWeeks, eGetDateAfterSomeWeeks, eGetDateBeforeSomeMonths,
    eGetDateAfterSomeMonths, eGetDateByDayOrderInYear, eGetdifferenceBetweenTwoDates,
    eCalculateAgeUpUntilNow, eCalculateAgeUpUntilSomeDate, eDoesDateExistInPeriod, ePeriodsOverlapChecker,
    eGetPeriodsOverlapDays, eGetTotalDaysUntilNow, eGetTotalDaysFromTheStartOfYear, eGetTotalDaysUntilTheEndOfYear,
    eGetTotalDaysUntilTheEndOfWeek, eSettings
};

void displayMainMenu();
void displaySettingsScreen();

sSettings loadSettingsFromFile() {
    fstream settingsFile;
    sSettings settings = { "dd/mm/yyyy", enWeekEndVariants::eFriday };

    if (files::isFileEmpty(settingsFileName)) {
        settingsFile.open(settingsFileName, ios::app);

        if (settingsFile.is_open()) {
            settings.format = readDateFormat("Enter Active Date Format:\n", "Program Setup: 1/3\n\n");
            system("cls");
            settings.weekEndDays = readWeekEndDays("Program Setup: 2/3\n\n");
            system("cls");
            settings.endOfWeek = readWeekDay("* You must choose the end of week:\n", "Program Setup: 3/3\n\n");

            settingsFile << settings.format << '\n';
            settingsFile << settings.weekEndDays << '\n';
            settingsFile << settings.endOfWeek;

            settingsFile.close();
        }
        else
            cerr << "Error: Cannot open settings file to fill it.";
    }
    else {
        vector<string> vctLines;
        files::copyFileToVct(settingsFileName, vctLines);

        settings.format = vctLines[0];
        settings.weekEndDays = (enWeekEndVariants)stoi(vctLines[1]);
    }

    return settings;
}

short readUserChoiceAfterAction() {
    cout << "\n[1]: Restart";
    cout << "\n[2]: Back To Main Menu";
    cout << "\n----------------------------\n";

    return readers::readInt("Choose [1] or [2]: ", 1, 2);
}

void displayLeapYearChecker() {
    short userChoice = 1, year;

    while (userChoice == 1) {
        system("cls");

        cout << "--------------Check a Leap Year-------------\n";
        cout << "|  You enter a year, and then the program  |\n";
        cout << "|  tells you whether it is leap or not.    |\n";
        cout << "--------------------------------------------\n" << endl;

        year = readers::readPosInt("Enter Year:\n");

        if (isYearLeap(year))
            cout << "\nYear " << year << " is a leap year.\n";
        else
            cout << "\nYear " << year << " is NOT a leap year.\n";

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayCompareTwoDates() {
    short userChoice = 1;
    sDate date1, date2;

    while (userChoice == 1) {
        system("cls");

        cout << "--------------Compare Between Two Dates--------------\n";
        cout << "|  You enter two dates, and then the program tells  |\n";
        cout << "|  you which one comes before the other and so on.  |\n";
        cout << "-----------------------------------------------------\n" << endl;

        date1 = readDateAsStr("Enter the first date:\n", globalSettings.format, true);
        date2 = readDateAsStr("Enter the second date:\n", globalSettings.format, true);

        switch (compareTwoDates(date1, date2)) {
            case enDateCompareResults::eAfter:
                cout << "\nThe first date is AFTER the second one.\n";
                break;
            case enDateCompareResults::eBefore:
                cout << "\nThe first date is BEFORE the second one.\n";
                break;
            default:
                cout << "\nThe two dates are the SAME.\n";
        }

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayShowYearData() {
    short userChoice = 1, year;

    while (userChoice == 1) {
        system("cls");

        cout << "-----------------Year Data Viewer-----------------\n";
        cout << "|  Enter a year, and then the program tells you  |\n";
        cout << "|  some basic information about that year.       |\n";
        cout << "--------------------------------------------------\n" << endl;

        year = readers::readPosInt("Enter the year:\n");

        cout << "\n----------------------------------";
        cout << "\nType              : " << (isYearLeap(year) ? "Leap" : "Regular");
        cout << "\nNumber of Days    : " << getDaysInYear(year);
        cout << "\nNumber of Hours   : " << getHoursInYear(year);
        cout << "\nNumber of Minutes : " << getMinsInYear(year);
        cout << "\nNumber of Seconds : " << getSecsInYear(year);
        cout << "\n----------------------------------";

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayShowMonthData() {
    short userChoice = 1, month, year;

    while (userChoice == 1) {
        system("cls");

        cout << "------------------Month Data Viewer------------------\n";
        cout << "|  Enter a month along with a year, and then the    |\n";
        cout << "|  program tells you some basic information about   |\n";
        cout << "|  that month.                                      |\n";
        cout << "-----------------------------------------------------\n" << endl;

        month = readers::readInt("Enter the month:\n", 1, 12);
        year = readers::readPosInt("Enter the year:\n");

        cout << "\n----------------------------------";
        cout << "\nNumber of Days    : " << getDaysInMonth(month, year);
        cout << "\nNumber of Hours   : " << getHoursInMonth(month, year);
        cout << "\nNumber of Minutes : " << getMinsInMonth(month, year);
        cout << "\nNumber of Seconds : " << getSecsInMonth(month, year);
        cout << "\n----------------------------------";

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayShowDayData() {
    system("cls");

    cout << "-----------Day Data Viewer------------\n";
    cout << "|                                    |\n";
    cout << "|  A day consists of 24 hours, 1440  |\n";
    cout << "|  minutes, and 86400 seconds.       |\n";
    cout << "--------------------------------------\n";

    cout << "\nPress any key to return to Main Menu...";
    system("pause>nul");
    displayMainMenu();
}

void displayWeekendChecker() {
    short userChoice = 1;
    sDate date;

    while (userChoice == 1) {
        system("cls");

        cout << "----------------Weekend Checker----------------\n";
        cout << "|  You enter a date, and then the program     |\n";
        cout << "|  tells you whether it is a weekend or not.  |\n";
        cout << "-----------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);

        if (isWeekEnd(date, globalSettings.weekEndDays))
            printf("\nYes, %s, %s, is a weekend.\n", getDayName(date).c_str(), formatDate(date, globalSettings.format).c_str());
        else
            printf("\nNo, %s, %s, is NOT a weekend.\n", getDayName(date).c_str(), formatDate(date, globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetVacationDays() {
    short userChoice = 1;
    sDate startDate, returnDate;

    while (userChoice == 1) {
        system("cls");

        cout << "-----------------Calculate Vacation Days-----------------\n";
        cout << "|  You enter the start date along with the return date  |\n";
        cout << "|  of the vacation, and then the program tells you the  |\n";
        cout << "|  actual vacation days excluding weekends.             |\n";
        cout << "---------------------------------------------------------\n" << endl;

        startDate = readDateAsStr("Enter the start date:\n", globalSettings.format);
        returnDate = readDateAsStr("Enter the return date:\n", globalSettings.format);

        cout << "\nActual vacation days: " << getVacationDays(startDate, returnDate, globalSettings.weekEndDays) << endl;

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetVacationReturnDate() {
    short userChoice = 1;
    sDate startDate;
    short duration;

    while (userChoice == 1) {
        system("cls");

        cout << "--------------------Calculate Vacation Return Date--------------------\n";
        cout << "|  You enter the start date of the vacation along with its duration  |\n";
        cout << "|  in days, and then the program tells you the return date of the    |\n";
        cout << "|  vacation excluding weekends from the duration.                    |\n";
        cout << "----------------------------------------------------------------------\n" << endl;

        startDate = readDateAsStr("Enter the start date:\n", globalSettings.format);
        duration = readers::readPosInt("Enter the duration:\n");

        printf("\nThe return date is --> %s\n", formatDate(getVacationReturnDate(startDate, duration, globalSettings.weekEndDays), globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDayNameFromDate() {
    short userChoice = 1;
    sDate date;

    while (userChoice == 1) {
        system("cls");

        cout << "--------------Get Day Name From Date--------------\n";
        cout << "|  You enter a date, and then the program tells  |\n";
        cout << "|  you the name of that day in the week.         |\n";
        cout << "--------------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);

        printf("\nThe date %s is a %s.\n", formatDate(date, globalSettings.format).c_str(), getDayName(date).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayDoesDateCoincideWith() {
    short userChoice = 1;
    sDate date;
    enWeekDays day;

    while (userChoice == 1) {
        system("cls");

        cout << "----------------Does Date Coincide With----------------\n";
        cout << "|  You enter a date along with a week day, and then   |\n";
        cout << "|  the program tells you if this date coincides with  |\n";
        cout << "|  that day or not.                                   |\n";
        cout << "-------------------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);
        day = readWeekDay("\nHere are all the week days:\n");

        if (doesDateCoincideWith(date, day))
            printf("\nYes, %s coincides with %s.\n", formatDate(date, globalSettings.format).c_str(), getDayName(day).c_str());
        else
            printf("\nNo, %s does NOT coincide with %s.\n", formatDate(date, globalSettings.format).c_str(), getDayName(day).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayMonthCalendar() {
    short userChoice = 1;
    short month, year;

    while (userChoice == 1) {
        system("cls");

        cout << "---------------------Display Month Calendar---------------------\n";
        cout << "|  You enter a month along with a year, and then the program   |\n";
        cout << "|  displays the calendar of that month in that year.           |\n";
        cout << "----------------------------------------------------------------\n" << endl;

        month = readers::readInt("Enter a month: ", 1, 12);
        year = readers::readPosInt("Enter a year: ");

        cout << endl;

        printMonthCalendar(month, year);

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayYearCalendar() {
    short userChoice = 1;
    short year;

    while (userChoice == 1) {
        system("cls");

        cout << "------------Display Year Calendar------------\n";
        cout << "|  You enter a year, and then the program   |\n";
        cout << "|  displays the calendar of that year.      |\n";
        cout << "---------------------------------------------\n" << endl;

        year = readers::readPosInt("Enter a year: ");

        cout << endl;

        printYearCalendar(year);

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDateBeforeSomeDays() {
    short userChoice = 1;
    sDate date;
    unsigned int days;

    while (userChoice == 1) {
        system("cls");

        cout << "-----------------Calculate Date Before Some Days-----------------\n";
        cout << "|  You enter a date along with a number of days, and then the   |\n";
        cout << "|  program tells you the date before that number of days.       |\n";
        cout << "-----------------------------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);
        days = readers::readPosInt("Enter the number of days:\n");

        printf("\nYour date before %d days is --> %s\n", days, formatDate(getDateBeforeSomeDays(date, days), globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDateAfterSomeDays() {
    short userChoice = 1;
    sDate date;
    unsigned int days;

    while (userChoice == 1) {
        system("cls");

        cout << "-----------------Calculate Date After Some Days-----------------\n";
        cout << "|  You enter a date along with a number of days, and then the  |\n";
        cout << "|  program tells you the date after that number of days.       |\n";
        cout << "----------------------------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);
        days = readers::readPosInt("Enter the number of days:\n");

        printf("\nYour date after %d days is --> %s\n", days, formatDate(getDateAfterSomeDays(date, days), globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDateBeforeSomeWeeks() {
    short userChoice = 1;
    sDate date;
    unsigned short weeks;

    while (userChoice == 1) {
        system("cls");

        cout << "-----------------Calculate Date Before Some Weeks-----------------\n";
        cout << "|  You enter a date along with a number of weeks, and then the   |\n";
        cout << "|  program tells you the date before that number of weeks.       |\n";
        cout << "------------------------------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);
        weeks = readers::readPosInt("Enter the number of weeks:\n");

        printf("\nYour date before %d weeks is --> %s\n", weeks, formatDate(getDateBeforeSomeWeeks(date, weeks), globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDateAfterSomeWeeks() {
    short userChoice = 1;
    sDate date;
    unsigned short weeks;

    while (userChoice == 1) {
        system("cls");

        cout << "-----------------Calculate Date After Some Weeks-----------------\n";
        cout << "|  You enter a date along with a number of weeks, and then the  |\n";
        cout << "|  program tells you the date after that number of weeks.       |\n";
        cout << "-----------------------------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);
        weeks = readers::readPosInt("Enter the number of weeks:\n");

        printf("\nYour date after %d weeks is --> %s\n", weeks, formatDate(getDateAfterSomeWeeks(date, weeks), globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDateAfterSomeMonths() {
    short userChoice = 1;
    sDate date;
    unsigned short months;

    while (userChoice == 1) {
        system("cls");

        cout << "-----------------Calculate Date After Some Months-----------------\n";
        cout << "|  You enter a date along with a number of months, and then the  |\n";
        cout << "|  program tells you the date after that number of months.       |\n";
        cout << "------------------------------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);
        months = readers::readPosInt("Enter the number of months:\n");

        printf("\nYour date after %d months is --> %s\n", months, formatDate(getDateAfterSomeMonths(date, months), globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDateBeforeSomeMonths() {
    short userChoice = 1;
    sDate date;
    unsigned short months;

    while (userChoice == 1) {
        system("cls");

        cout << "----------------Calculate Date Before Some Months----------------\n";
        cout << "|  You enter a date along with a number of months, and then the |\n";
        cout << "|  program tells you the date before that number of months.     |\n";
        cout << "-----------------------------------------------------------------\n" << endl;

        date = readDateAsStr("Enter the date:\n", globalSettings.format);
        months = readers::readPosInt("Enter the number of months:\n");

        printf("\nYour date before %d months is --> %s\n", months, formatDate(getDateBeforeSomeMonths(date, months), globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDateByDayOrderInYear() {
    short userChoice = 1;
    short year, dayOrder;

    while (userChoice == 1) {
        system("cls");

        cout << "-------------Calculate Date By Day Order In Year-------------\n";
        cout << "|  You enter a year along with the number of days passed    |\n";
        cout << "|  from the start of the year, and then the program tells   |\n";
        cout << "|  you the corresponding date.                              |\n";
        cout << "-------------------------------------------------------------\n" << endl;

        year = readers::readPosInt("Enter the year: ");

        if (isYearLeap(year))
            dayOrder = readers::readInt("Enter the day order [1] to [366]:\n", 1, 366);
        else
            dayOrder = readers::readInt("Enter the day order [1] to [365]:\n", 1, 365);

        printf("\nThe date after %d days is --> %s\n", dayOrder, formatDate(getDateByTotalDaysPassed(dayOrder, year), globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetDifferenceInDaysBetweenTwoDates() {
    short userChoice = 1;
    sDate date1, date2;
    int difference;

    while (userChoice == 1) {
        system("cls");

        cout << "------------Calculate Difference Between Two Dates------------\n";
        cout << "|  You enter two dates, and then the program tells you the   |\n";
        cout << "|  difference between them in days.                          |\n";
        cout << "--------------------------------------------------------------\n" << endl;

        date1 = readDateAsStr("Enter the first date:\n", globalSettings.format);
        date2 = readDateAsStr("Enter the second date:\n", globalSettings.format);

        difference = getDifferenceInDays(date1, date2);

        printf("\nThe difference is --> %d days.", difference);
        printf("\nThe difference (including the end day) is --> %d days.\n", difference + 1);

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetAgeUntilNow() {
    short userChoice = 1;
    sDate dateOfBirth, age;

    while (userChoice == 1) {
        system("cls");

        cout << "-------------------Calculate Age Up Until Now-------------------\n";
        cout << "|  You enter a date of birth, and then the program tells you   |\n";
        cout << "|  the age in years, months, and days including today's day.   |\n";
        cout << "----------------------------------------------------------------\n" << endl;


        dateOfBirth = readDateOfBirth("Enter the date of birth: ", globalSettings.format);

        age = getAge(dateOfBirth, true);

        printf("\nThe age (including today's day) is --> %d years, %d months, and %d days.\n", age.year, age.month, age.day);

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetAgeUntilDate() {
    short userChoice = 1;
    sDate dateOfBirth, date, age;

    while (userChoice == 1) {
        system("cls");

        cout << "-------------------Calculate Age Up Until Some Date-------------------\n";
        cout << "|  You enter a date of birth along with a date in past or in the     |\n";
        cout << "|  future, and then the program tells you the age in years, months,  |\n";
        cout << "|  and days including today's day.                                   |\n";
        cout << "----------------------------------------------------------------------\n" << endl;

        while (true) {
            dateOfBirth = readDateAsStr("Enter the date of birth: ", globalSettings.format);
            date = readDateAsStr("Enter the date to stop at: ", globalSettings.format);

            if (isDateAfterDate(dateOfBirth, date))
                cout << "Error: The date of birth must be before the other date.\n\n";
            else
                break;
        }

        age = getAge(dateOfBirth, date, true);

        printf("\nThe age (including the end day) is --> %d years, %d months, and %d days.\n", age.year, age.month, age.day);

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayDoesDateExistInPeriod() {
    short userChoice = 1;
    sPeriod period;
    sDate date;

    while (userChoice == 1) {
        system("cls");

        cout << "--------------------------Does Date Exist In Period--------------------------\n";
        cout << "|  You enter a period (two dates) along with a date, and then the program   |\n";
        cout << "|  tells you whether that date exists in that period or not.                |\n";
        cout << "-----------------------------------------------------------------------------\n" << endl;

        period.startDate = readDateAsStr("Enter the first date in period: ", globalSettings.format);
        period.endDate = readDateAsStr("Enter the second date in period: ", globalSettings.format);

        if (isDateAfterDate(period.startDate, period.endDate))
            swap(period.startDate, period.endDate);

        cout << endl;

        date = readDateAsStr("Enter the date to check: ", globalSettings.format);

        if (doesDateExistInPeriod(date, period))
            printf("\nYes, %s exists in that period.\n", formatDate(date, globalSettings.format).c_str());
        else
            printf("\nNo, %s does NOT exist in the period.\n", formatDate(date, globalSettings.format).c_str());

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayPeriodOverlapChecker() {
    short userChoice = 1;
    sPeriod period1, period2;

    while (userChoice == 1) {
        system("cls");

        cout << "-----------------------Periods Overlap Checker----------------------\n";
        cout << "|  You enter two periods (two dates each), and then the program    |\n";
        cout << "|  tells you whether those periods overlap or not.                 |\n";
        cout << "--------------------------------------------------------------------\n";

        cout << "\n* Reading the first period:\n";
        period1.startDate = readDateAsStr("   Enter the first date: ", globalSettings.format);
        period1.endDate = readDateAsStr("   Enter the second date: ", globalSettings.format);

        cout << "\n* Reading the second period:\n";
        period2.startDate = readDateAsStr("   Enter the first date: ", globalSettings.format);
        period2.endDate = readDateAsStr("   Enter the second date: ", globalSettings.format);

        if (isDateAfterDate(period1.startDate, period1.endDate))
            swap(period1.startDate, period1.endDate);

        if (isDateAfterDate(period2.startDate, period2.endDate))
            swap(period2.startDate, period2.endDate);

        if (doesTwoPeriodsOverlap(period1, period2))
            printf("\nYes, the two periods overlap.\n");
        else
            printf("\nNo, the two periods do NOT overlap.\n");

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetPeriodsOverlapDays() {
    short userChoice = 1;
    sPeriod period1, period2;

    while (userChoice == 1) {
        system("cls");

        cout << "------------------Calculate Periods Overlap Days------------------\n";
        cout << "|  You enter two periods (two dates each), and then the program  |\n";
        cout << "|  tells you the number of days common in each.                  |\n";
        cout << "------------------------------------------------------------------\n";

        cout << "\n* Reading the first period:\n";
        period1.startDate = readDateAsStr("   Enter the first date: ", globalSettings.format);
        period1.endDate = readDateAsStr("   Enter the second date: ", globalSettings.format);

        cout << "\n* Reading the second period:\n";
        period2.startDate = readDateAsStr("   Enter the first date: ", globalSettings.format);
        period2.endDate = readDateAsStr("   Enter the second date: ", globalSettings.format);

        if (isDateAfterDate(period1.startDate, period1.endDate))
            swap(period1.startDate, period1.endDate);

        if (isDateAfterDate(period2.startDate, period2.endDate))
            swap(period2.startDate, period2.endDate);

        printf("\nOverlap days: %d days.\n", getPeriodsOverlapDays(period1, period2));

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetTotalDaysUntilNow() {
    short userChoice = 1;

    while (userChoice == 1) {
        system("cls");

        cout << "----------------Calculate Total Days Up Until Now----------------\n";
        cout << "|  You enter a date, and then the program tells you the total   |\n";
        cout << "|  number of days from that date up until now.                  |\n";
        cout << "-----------------------------------------------------------------\n";

        printf("\nTotal number of days (including today's day) is --> %d\n", getDifferenceInDays(readDateOfBirth("Enter the date: ", globalSettings.format), getSystemDate(), true));

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetTotalDaysFromTheStartOfYear() {
    short userChoice = 1;

    while (userChoice == 1) {
        system("cls");

        cout << "--------------Calculate Total Days From The Start Of Year--------------\n";
        cout << "|  You enter a date, and then the program tells you the total number  |\n";
        cout << "|  of days from the start of the year (from your date) up until your  |\n";
        cout << "|  date including the end day.                                        |\n";
        cout << "-----------------------------------------------------------------------\n" << endl;

        printf("\nTotal number of days (including end day) is --> %d\n", getTotalDaysBefore(readDateAsStr("Enter the date: ", globalSettings.format)));

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetTotalDaysUntilTheEndOfYear() {
    short userChoice = 1;

    while (userChoice == 1) {
        system("cls");

        cout << "--------------Calculate Total Days Up Until The End Of Year--------------\n";
        cout << "|  You enter a date, and then the program tells you the total number of |\n";
        cout << "|  days from your date up until the end of the year (from your date)    |\n";
        cout << "|  including the end day.                                               |\n";
        cout << "-------------------------------------------------------------------------\n" << endl;

        printf("\nTotal number of days (including end day) is --> %d\n", getTotalDaysAfter(readDateAsStr("Enter the date: ", globalSettings.format)));

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void displayGetTotalDaysUntilTheEndOfWeek() {
    short userChoice = 1;

    while (userChoice == 1) {
        system("cls");

        cout << "--------------Calculate Total Days Up Until The End Of Week--------------\n";
        cout << "|  You enter a date, and then the program tells you the total number of |\n";
        cout << "|  days from your date up until the end of the current week including   |\n";
        cout << "|  the day of your date.                                                |\n";
        cout << "-------------------------------------------------------------------------\n" << endl;

        printf("\nTotal number of days (including the day of your date) is --> %d\n", getTotalDaysUntilEndOfWeek(readDateAsStr("Enter the date: ", globalSettings.format), globalSettings.endOfWeek, true));

        userChoice = readUserChoiceAfterAction();
    }

    displayMainMenu();
}

void saveSettingsToFile() {
    fstream settingsFile(settingsFileName, ios::out);

    if (settingsFile.is_open()) {
        settingsFile << globalSettings.format << '\n';
        settingsFile << globalSettings.weekEndDays << '\n';
        settingsFile << globalSettings.endOfWeek;

        settingsFile.close();
    }
    else
        cerr << "Error: Cannot open settings file for overriding.";
}

void backToSettingsScreen() {
    cout << "\nDone successfully.\n";
    cout << "Press any key to return to the Settings Screen...";
    system("pause>nul");
    displaySettingsScreen();
}

void displaySettingsScreen() {
    system("cls");

    cout << "* Here you can change:\n";
    cout << "    [1]: Accepted Date Format.\n";
    cout << "    [2]: Accepted Weekend Days.\n";
    cout << "    [3]: Accepted End of Week.\n\n";
    cout << "[4]: Exit.\n";
    cout << "-----------------------------------\n";

    short userChoice = readers::readInt("Choose [1] to [4]: ", 1, 4);

    switch (userChoice) {
        case 1:
            globalSettings.format = readDateFormat("Enter Active Date Format:\n");
            saveSettingsToFile();
            backToSettingsScreen();
            break;
        case 2:
            system("cls");
            globalSettings.weekEndDays = readWeekEndDays();
            saveSettingsToFile();
            backToSettingsScreen();
            break;
        case 3:
            system("cls");
            globalSettings.endOfWeek = readWeekDay("* Here are all the week days:\n");
            saveSettingsToFile();
            backToSettingsScreen();
            break;
        case 4:
            displayMainMenu();
    }
}

void performMainMenuAction(const enMainMenuActions userChoice) {
    switch (userChoice) {
        case eCheckLeapYear:
            displayLeapYearChecker();
            break;
        case eCompareTwoDates:
            displayCompareTwoDates();
            break;
        case eShowYearData:
            displayShowYearData();
            break;
        case eShowMonthData:
            displayShowMonthData();
            break;
        case eShowDayData:
            displayShowDayData();
            break;
        case eWeekendChecker:
            displayWeekendChecker();
            break;
        case eGetVacationDays:
            displayGetVacationDays();
            break;
        case eGetVacationReturnDate:
            displayGetVacationReturnDate();
            break;
        case eGetDayNameFromDate:
            displayGetDayNameFromDate();
            break;
        case eDoesDateCoincideWith:
            displayDoesDateCoincideWith();
            break;
        case eDisplayMonthCalendar:
            displayMonthCalendar();
            break;
        case eDisplayYearCalendar:
            displayYearCalendar();
            break;
        case eGetDateBeforeSomeDays:
            displayGetDateBeforeSomeDays();
            break;
        case eGetDateAfterSomeDays:
            displayGetDateAfterSomeDays();
            break;
        case eGetDateBeforeSomeWeeks:
            displayGetDateBeforeSomeWeeks();
            break;
        case eGetDateAfterSomeWeeks:
            displayGetDateAfterSomeWeeks();
            break;
        case eGetDateBeforeSomeMonths:
            displayGetDateBeforeSomeMonths();
            break;
        case eGetDateAfterSomeMonths:
            displayGetDateAfterSomeMonths();
            break;
        case eGetDateByDayOrderInYear:
            displayGetDateByDayOrderInYear();
            break;
        case eGetdifferenceBetweenTwoDates:
            displayGetDifferenceInDaysBetweenTwoDates();
            break;
        case eCalculateAgeUpUntilNow:
            displayGetAgeUntilNow();
            break;
        case eCalculateAgeUpUntilSomeDate:
            displayGetAgeUntilDate();
            break;
        case eDoesDateExistInPeriod:
            displayDoesDateExistInPeriod();
            break;
        case ePeriodsOverlapChecker:
            displayPeriodOverlapChecker();
            break;
        case eGetPeriodsOverlapDays:
            displayGetPeriodsOverlapDays();
            break;
        case eGetTotalDaysUntilNow:
            displayGetTotalDaysUntilNow();
            break;
        case eGetTotalDaysFromTheStartOfYear:
            displayGetTotalDaysFromTheStartOfYear();
            break;
        case eGetTotalDaysUntilTheEndOfYear:
            displayGetTotalDaysUntilTheEndOfYear();
            break;
        case eGetTotalDaysUntilTheEndOfWeek:
            displayGetTotalDaysUntilTheEndOfWeek();
            break;
        case eSettings:
            displaySettingsScreen();
    }
}

void displayMainMenu() {
    system("cls");

    printf("System Date: %s, %s", getSystemDayName().c_str(), formatDate(getSystemDate(), globalSettings.format).c_str());
    cout << "\n===================================================================================================";
    cout << "\n                                          Date Calculator";
    cout << "\n===================================================================================================";
    cout << "\n[01]: Leap Year Checker                              [02]: Compare Between Two Dates";
    cout << "\n[03]: View Year Data                                 [04]: View Month Data";
    cout << "\n[05]: View Day Data                                  [06]: Check a Weekend";
    cout << "\n[07]: Calculate Vacation Days                        [08]: Calculate Vacation Return Date";
    cout << "\n[09]: Get Day Name From Date                         [10]: Does Date Coincide With...";
    cout << "\n[11]: Display Month Calendar                         [12]: Display Year Calendar";
    cout << "\n[13]: Calaculate Date Before Some Days               [14]: Calculate Date After Some Days";
    cout << "\n[15]: Calaculate Date Before Some Weeks              [16]: Calculate Date After Some Weeks";
    cout << "\n[17]: Calaculate Date Before Some Months             [18]: Calculate Date After Some Months";
    cout << "\n[19]: Calaculate Date By Day Order in Year           [20]: Calculate difference Between Two Dates";
    cout << "\n[21]: Calculate Age Up Until Now                     [22]: Calculate Age Up Until Some Date";
    cout << "\n[23]: Does Date Exist in Period                      [24]: Periods Overlap Checker";
    cout << "\n[25]: Calculate Periods Overlap Days                 [26]: Calculate Total Days Up Until Now";
    cout << "\n[27]: Calculate Total Days From the Start of a Year";
    cout << "\n[28]: Calculate Total Days Until the End of a Year";
    cout << "\n[29]: Calculate Total Days Until The End of the Week";
    cout << "\n---------------------------------------------------------------------------------------------------";
    cout << "\n[30]: Settings";
    cout << "\n===================================================================================================" << endl;

    performMainMenuAction((enMainMenuActions)readers::readNum("Choose [01] to [30]: ", 1, 30));
}

int main() {
    globalSettings = loadSettingsFromFile();

    displayMainMenu();

    return 0;
}
