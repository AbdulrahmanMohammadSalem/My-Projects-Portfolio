// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

/* Abbreviations used in this file are:
* min = minute
* sec = second
* pos = position, index
* crr = current
* tmp = temporary
* struct = structure
* str = string
* msg = message
* num = number
* col = column

*/

#pragma once
#pragma warning(disable: 4996)

#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include "Strings.h"
#include "Readers.h"

using namespace std;

namespace dates_utilities {
	enum enDateFormatParts {
		eDD, eMM, eYY, eYYYY
	};

	struct sDateFormatPos {
		short pos = 0;
		enDateFormatParts formatPart = enDateFormatParts::eDD;
	};

	vector<sDateFormatPos> getFormatPartsPositions(string & format) {
		vector<sDateFormatPos> vPositions;
		sDateFormatPos tempPos;

		//Filling dd
		while ((tempPos.pos = format.find("dd", tempPos.pos)) != string::npos) {
			vPositions.push_back(tempPos);
			tempPos.pos += 2;
		}

		//Filling mm
		tempPos.pos = 0;
		tempPos.formatPart = enDateFormatParts::eMM;

		while ((tempPos.pos = format.find("mm", tempPos.pos)) != string::npos) {
			vPositions.push_back(tempPos);
			tempPos.pos += 2;
		}

		//Filling yy
		format += "  "; //This is crucial for safety
		tempPos.pos = 0;
		tempPos.formatPart = enDateFormatParts::eYY;
		
		while ((tempPos.pos = format.find("yy", tempPos.pos)) != string::npos) {
			if (format[tempPos.pos + 2] != 'y' || format[tempPos.pos + 3] != 'y') {
				vPositions.push_back(tempPos);
				tempPos.pos += 2;
			} else
				tempPos.pos += 4;
		}
		
		//Filling yyyy
		tempPos.pos = 0;
		tempPos.formatPart = enDateFormatParts::eYYYY;
		
		while ((tempPos.pos = format.find("yyyy", tempPos.pos)) != string::npos) {
			vPositions.push_back(tempPos);
			tempPos.pos += 4;
		}

		//To remove those two spaces
		format.pop_back();
		format.pop_back();

		return vPositions;
	}
	
	void sortPositions(vector<sDateFormatPos> & vPositions) {
		bool didSwapOccur = true;

		while (didSwapOccur) {
			didSwapOccur = false;

			for (short i = 0; i < vPositions.size() - 1; i++)
				if (vPositions[i].pos > vPositions[i + 1].pos) {
					swap(vPositions[i], vPositions[i + 1]);
					didSwapOccur = true;
				}
		}
	}
}

namespace dates {
	using namespace dates_utilities;

	enum enDateCompareResults { eBefore = -1, eEqual = 0, eAfter = 1 };
	enum enWeekDays { eSun, eMon, eTue, eWed, eThu, eFri, eSat };
	enum enWeekEndVariants { eFriday, eFriSat, eSatSun };
	enum enDateFormats { eDMY, eMDY, eYMD };
	enum enMonthWordingFormats { eFullMonthName, eAbbreviatedMonthName, eSuffixFormat };
	enum enMeridiemIndicators { eAM, ePM };
	enum enTimeSystem { eTwelveHour, eTwentyFourHour };
	enum enValidateDateStrResults {
		eErrorInFormat, eErrorInText, eErrorInYYValue, eErrorInValuesConsistency,
		eErrorInYYYYValue, eErrorInDateValues, eErrorInSeperatorCount, eNoErrors
	};

	// From the dates library

	struct sTime {
		short sec = 0;
		short min = 0;
		short hour = 0;

		//These default values are crucial for the work of time-related functions, never change them
		enMeridiemIndicators meridiemIndicator = ePM; 
		enTimeSystem timeSystem = eTwentyFourHour;
	};

	struct sDate {
		short day = 0;
		short month = 0;
		short year = 0;
	};

	struct sDateTime {
		sDate date;
		sTime time;
	};

	struct sPeriod {
		sDate startDate, endDate;
	};

	bool isYearLeap(const short year) {
		return year % 100 == 0 ? year % 400 == 0 : year % 4 == 0;
	}

	short getDaysInYear(const short year) {
		return isYearLeap(year) ? 366 : 365;
	}

	short getHoursInYear(const short year) {
		return isYearLeap(year) ? 8784 : 8760;
	}

	int getMinsInYear(const short year) {
		return isYearLeap(year) ? 527040 : 525600;
	}

	int getSecsInYear(const short year) {
		return isYearLeap(year) ? 31622400 : 31536000;
	}

	short getDaysInMonth(const short month, const short year) {
		short monthsDays[12] {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		return month == 2 && isYearLeap(year) ? 29 : monthsDays[month - 1];
	}

	short getHoursInMonth(const short month, const short year) {
		return getDaysInMonth(month, year) * 24;
	}

	unsigned short getMinsInMonth(const short month, const short year) {
		return getHoursInMonth(month, year) * 60;
	}

	int getSecsInMonth(const short month, const short year) {
		return getMinsInMonth(month, year) * 60;
	}

	short getMinsInDay() {
		return 1440;
	}

	int getSecsInDay() {
		return 86400;
	}

	string getMonthName(const short month, const bool isAbbreviated = false) {
		switch (month) {
			case 1: return isAbbreviated ? "Jan" : "January";
			case 2: return isAbbreviated ? "Feb" : "February";
			case 3: return isAbbreviated ? "Mar" : "March";
			case 4: return isAbbreviated ? "Apr" : "April";
			case 5: return isAbbreviated ? "May" : "May";
			case 6: return isAbbreviated ? "Jun" : "June";
			case 7: return isAbbreviated ? "Jul" : "July";
			case 8: return isAbbreviated ? "Aug" : "August";
			case 9: return isAbbreviated ? "Sep" : "September";
			case 10: return isAbbreviated ? "Oct" : "October";
			case 11: return isAbbreviated ? "Nov" : "November";
			case 12: return isAbbreviated ? "Dec" : "December";
			default: return "[UnknownMonth]";
		}
	}

	//Sunday is 0
	short getDayPosInWeek(const sDate & date) {
		const short a = (14 - date.month) / 12;
		const short y = date.year - a;

		/* Don't take y as a common factor!
		   Every division is supposed to be in a floor function
		*/

		return (date.day + y + y / 4 - y / 100 + y / 400 + 31 * (date.month + 12 * a - 2) / 12) % 7;
	}

	bool doesDateCoincideWith(const sDate & date, const enWeekDays weekday) {
		return getDayPosInWeek(date) == (short) weekday;
	}

	string getDayName(const short posInWeek, const bool isAbbreviated = false) { //0 is Sunday
		switch (posInWeek) {
			case 0: return isAbbreviated ? "Sun" : "Sunday";
			case 1: return isAbbreviated ? "Mon" : "Monday";
			case 2: return isAbbreviated ? "Tue" : "Tuesday";
			case 3: return isAbbreviated ? "Wed" : "Wednesday";
			case 4: return isAbbreviated ? "Thu" : "Thursday";
			case 5: return isAbbreviated ? "Fri" : "Friday";
			case 6: return isAbbreviated ? "Sat" : "Saturday";
			default: return "[UnknownDay]";
		}
	}

	string getDayName(const sDate & date, const bool isAbbreviated = false) {
		return getDayName(getDayPosInWeek(date), isAbbreviated);
	}

	//Validates an sDate object
	bool isDateValid(const sDate & date) {
		return date.month > 0 && date.month < 13 ? date.day > 0 && date.day <= getDaysInMonth(date.month, date.year): false;
	}

	//Validates a date format string
	bool isDateFormatValid(const string & format) {
		return format.find("dd") != string::npos && format.find("mm") != string::npos && format.find("yyyy") != string::npos;
	}

	/* Validates a string as a preset date format.
	   If the string matches the format, the extracted date is stored in the last sDate parameter
	*/
	enValidateDateStrResults isDateStrValid(const string & strDate, const enDateFormats presetFormat, const char separator, sDate & extractTo) {
		if (strings::getChrCnt(strDate, separator) != 2)
			return enValidateDateStrResults::eErrorInSeperatorCount;

		vector<string> vctDateParts = strings::getVctOfTkns(strDate, separator);

		if (strings::getDigCnt(vctDateParts[0]) != vctDateParts[0].length() || strings::getDigCnt(vctDateParts[1]) != vctDateParts[1].length() || strings::getDigCnt(vctDateParts[2]) != vctDateParts[2].length())
			return enValidateDateStrResults::eErrorInText;

		sDate dateStruct;

		switch (presetFormat) {
			case enDateFormats::eDMY:
				dateStruct.day = stoi(vctDateParts[0]);
				dateStruct.month = stoi(vctDateParts[1]);
				dateStruct.year = stoi(vctDateParts[2]);
				break;
			case enDateFormats::eMDY:
				dateStruct.month = stoi(vctDateParts[0]);
				dateStruct.day = stoi(vctDateParts[1]);
				dateStruct.year = stoi(vctDateParts[2]);
				break;
			case enDateFormats::eYMD:
				dateStruct.year = stoi(vctDateParts[0]);
				dateStruct.month = stoi(vctDateParts[1]);
				dateStruct.day = stoi(vctDateParts[2]);
		}

		if (isDateValid(dateStruct)) {
			extractTo = dateStruct;
			return enValidateDateStrResults::eNoErrors;
		}

		return enValidateDateStrResults::eErrorInDateValues;
	}

	//Validates a string as a preset date format
	enValidateDateStrResults isDateStrValid(const string & strDate, const enDateFormats presetFormat = enDateFormats::eDMY, const char separator = '/') {
		sDate dummy;

		return isDateStrValid(strDate, presetFormat, separator, dummy);
	}

	/* Validates a string as a custom date format.
	   If the string matches the format, the extracted date is stored in the last sDate parameter
	*/
	enValidateDateStrResults isDateStrValid(string strDate, string format, sDate & extractTo) {
		if (!isDateFormatValid(format)) //If the format does NOT contain each of dd & mm & yyyy. yy is optional.
			return enValidateDateStrResults::eErrorInFormat;

		vector<sDateFormatPos> vPositions = getFormatPartsPositions(format);

		sortPositions(vPositions);

		short crrVals[4] = {0, 0, 0, 0};
		short oldVals[4] = {0, 0, 0, 0};
		bool hasEncounteredYYYY = false, hasEncounteredYY = false;
		string crrPrt = "";
		short firstNonDigitPos;

		//To fill the oldVals array
		for (short crrPrtPosInVct = 0; crrPrtPosInVct < vPositions.size(); crrPrtPosInVct++) {
			if (vPositions[crrPrtPosInVct].formatPart != enDateFormatParts::eYYYY) { //If the current part is NOT yyyy
				if (!hasEncounteredYY && vPositions[crrPrtPosInVct].formatPart == enDateFormatParts::eYY)
					hasEncounteredYY = true;
				
				//To check if the strDate is long enough to extract a sub-string from it:
				if (vPositions[crrPrtPosInVct].pos < strDate.length())
					crrPrt = strDate.substr(vPositions[crrPrtPosInVct].pos, 2);
				else
					return enValidateDateStrResults::eErrorInText;

				if (!isdigit(crrPrt[0])) //If the first character in crrPrt is NOT a digit
					return enValidateDateStrResults::eErrorInText;

				//To fill crrVals
				if (crrPrt.length() == 1) {
					firstNonDigitPos = 1;
					strDate.insert(strDate.begin() + vPositions[crrPrtPosInVct].pos, '0');

					crrVals[vPositions[crrPrtPosInVct].formatPart] = crrPrt[0] - 48;
				} else {
					if (isdigit(crrPrt[1])) { //If the second character in crrPrt is a digit
						firstNonDigitPos = 2;
						crrVals[vPositions[crrPrtPosInVct].formatPart] = stoi(crrPrt);
					} else { //If the second character in crrPrt is NOT a digit
						firstNonDigitPos = 1;
						strDate.insert(strDate.begin() + vPositions[crrPrtPosInVct].pos, '0');

						crrVals[vPositions[crrPrtPosInVct].formatPart] = crrPrt[0] - 48;
					}
				}

				//To validate crrVals with oldVals
				if (oldVals[vPositions[crrPrtPosInVct].formatPart] == 0) //If this is the first time we reach this format part
					oldVals[vPositions[crrPrtPosInVct].formatPart] = crrVals[vPositions[crrPrtPosInVct].formatPart];
				else if (crrVals[vPositions[crrPrtPosInVct].formatPart] != oldVals[vPositions[crrPrtPosInVct].formatPart]) //If not, check if this value is the NOT the same as the older value
					return enValidateDateStrResults::eErrorInText;
				
				//To check if the value of yyyy MOD 2 does NOT equal to the value of yy
				if (vPositions[crrPrtPosInVct].formatPart == enDateFormatParts::eYY && hasEncounteredYYYY && oldVals[2] != oldVals[3] % 100)
					return enValidateDateStrResults::eErrorInYYValue;
			} else { //If the current part is yyyy
				hasEncounteredYYYY = true;
				firstNonDigitPos = 0;

				//To find the position of the first non-digit character starting from the position of the current formatPrt:
				while (isdigit(strDate[vPositions[crrPrtPosInVct].pos + firstNonDigitPos]))
					firstNonDigitPos++;

				//To check if the strDate is long enough to extract a sub-string from it
				if (vPositions[crrPrtPosInVct].pos < strDate.length())
					crrPrt = strDate.substr(vPositions[crrPrtPosInVct].pos, firstNonDigitPos);
				else
					return enValidateDateStrResults::eErrorInText;

				if (firstNonDigitPos == 0) //If the crrPrt is empty
					return enValidateDateStrResults::eErrorInText;

				crrVals[3] = stoi(crrPrt.substr(0, firstNonDigitPos));

				if (oldVals[3] == 0) //If this is the first time we reach this format part
					oldVals[3] = crrVals[3];
				else if (crrVals[3] != oldVals[3]) //If not, check if this value is the NOT the same as the older value
					return enValidateDateStrResults::eErrorInValuesConsistency;
				
				//To check if the value of yyyy MOD 2 does NOT equal to the value of yy
				if (hasEncounteredYY && crrVals[3] % 100 != oldVals[2])
					return enValidateDateStrResults::eErrorInYYYYValue;
			}

			//To extract parts from the format and compare them with their corresponding parts in the strDate string:
			const short crrFormatPrtLength = vPositions[crrPrtPosInVct].formatPart == enDateFormatParts::eYYYY ? 4 : 2;
			short chrsToExtract;

			//To calculate the number of characters to extract:
			if (crrPrtPosInVct == vPositions.size() - 1) //If this formatPrt was the last element in the vPositions vector
				chrsToExtract = format.length() - vPositions[crrPrtPosInVct].pos - crrFormatPrtLength;
			else //If this formatPrt was NOT the last element in the vPositions vector
				chrsToExtract = vPositions[crrPrtPosInVct + 1].pos - vPositions[crrPrtPosInVct].pos - crrFormatPrtLength;

			//To compare extracted substrings:
			if (vPositions[crrPrtPosInVct].formatPart == enDateFormatParts::eYYYY) {
				if (format.substr(vPositions[crrPrtPosInVct].pos + crrFormatPrtLength, chrsToExtract) != strDate.substr(vPositions[crrPrtPosInVct].pos + firstNonDigitPos, chrsToExtract)) //If the rest of the format string is NOT the same as the rest of the strDate string
					return enValidateDateStrResults::eErrorInText;
			} else
				if (format.substr(vPositions[crrPrtPosInVct].pos + crrFormatPrtLength, chrsToExtract) != strDate.substr(vPositions[crrPrtPosInVct].pos + (firstNonDigitPos == crrPrt.length() ? firstNonDigitPos : firstNonDigitPos + 1), chrsToExtract)) //If the rest of the format string is NOT the same as the rest of the strDate string
					return enValidateDateStrResults::eErrorInText;
		}

		//Storing in an sDate object:
		sDate date;

		date.day = oldVals[0];
		date.month = oldVals[1];
		date.year = oldVals[3];

		if (isDateValid(date)) {
			extractTo = date;
			return enValidateDateStrResults::eNoErrors;
		}

		return enValidateDateStrResults::eErrorInDateValues;
	}

	//Validates a string as a custom date format
	enValidateDateStrResults isDateStrValid(string strDate, string format) {
		sDate dummy;

		return isDateStrValid(strDate, format, dummy);
	}

	//Preset Number-Based Formatting
	string formatDate(const sDate & date, const enDateFormats presetFormat = enDateFormats::eDMY, const char separator = '/') {
		switch (presetFormat) {
			case enDateFormats::eDMY:
				return to_string(date.day) + separator + to_string(date.month) + separator + to_string(date.year);

			case enDateFormats::eMDY:
				return to_string(date.month) + separator + to_string(date.day) + separator + to_string(date.year);

			case enDateFormats::eYMD:
				return to_string(date.year) + separator + to_string(date.month) + separator + to_string(date.day);

			default:
				return "[InvalidDateFormat]";
		}
	}

	//Preset Text-Based Formatting
	string formatDate(const sDate & date, enDateFormats presetFormat, enMonthWordingFormats monthWordingFormat) {
		switch (presetFormat) {
			case enDateFormats::eDMY:
				switch (monthWordingFormat) {
					case enMonthWordingFormats::eFullMonthName:
						return to_string(date.day) + ' ' + getMonthName(date.month) + ' ' + to_string(date.year);
					case enMonthWordingFormats::eAbbreviatedMonthName:
						return to_string(date.day) + ' ' + getMonthName(date.month, true) + ' ' + to_string(date.year);
					case enMonthWordingFormats::eSuffixFormat:
					{
						string suffix[4] {"th", "st", "nd", "rd"};
						return to_string(date.day) + suffix[(date.day / 10 != 1 && date.day % 10 < 4) ? date.day % 10 : 0] + " of " + getMonthName(date.month) + ", " + to_string(date.year);
					}
					default:
						return "[InvalidDateFormat]";
				}

			case enDateFormats::eMDY:
				if (monthWordingFormat == enMonthWordingFormats::eFullMonthName)
					return getMonthName(date.month) + ' ' + to_string(date.day) + ", " + to_string(date.year);
				else if (monthWordingFormat == enMonthWordingFormats::eSuffixFormat) {
					string suffix[4] {"th", "st", "nd", "rd"};
					return getMonthName(date.month) + ' ' + to_string(date.day) + suffix[(date.day / 10 != 1 && date.day % 10 < 4) ? date.day % 10 : 0] + ", " + to_string(date.year);
				} else
					return "[InvalidDateFormat]";

			default:
				return "[InvalidDateFormat]";
		}
	}

	//Custom Number-Based Formatting
	string formatDate(const sDate & date, string format) {
		format = strings::replaceAll(format, "dd", to_string(date.day));
		format = strings::replaceAll(format, "mm", to_string(date.month));
		format = strings::replaceAll(format, "yyyy", to_string(date.year));
		format = strings::replaceAll(format, "yy", to_string(date.year % 100));
		
		return format;
	}

	string formatTime(const sTime time, bool includeSecs = false, bool forceTwoDigitHours = false) {
		ostringstream result;

		//Formatting hours
		if (time.timeSystem == enTimeSystem::eTwentyFourHour || forceTwoDigitHours)
			result << setw(2) << setfill('0') << time.hour << ':';
		else
			result << time.hour << ':';

		//Formatting minutes
		result << setw(2) << setfill('0') << time.min;

		//Formatting seconds
		if (includeSecs)
			result << ':' << setw(2) << setfill('0') << time.sec;

		//Formatting AM, PM
		if (time.timeSystem == enTimeSystem::eTwelveHour)
			result << ((time.meridiemIndicator == enMeridiemIndicators::eAM) ? " AM" : " PM");

		return result.str();
	}

	void printMonthCalendar(const short month, const short year) {
		printf("---------------%s---------------", getMonthName(month, true).c_str());
		printf("\nSun  Mon  Tue  Wed  Thu  Fri  Sat\n");

		const short firstDayPos = getDayPosInWeek({1, month, year});
		const short totalDays = getDaysInMonth(month, year);
		short slotsCounter = 0;

		//This for loop prints the empty spaces before the first number
		for (slotsCounter; slotsCounter < firstDayPos; slotsCounter++)
			printf("     ");

		//This for loop prints the rest of the calendar
		for (short dayCounter = 1; dayCounter <= totalDays; dayCounter++) {
			if (slotsCounter == 7) {
				slotsCounter = 0;
				cout << '\n';
			}

			printf("%3d  ", dayCounter);
			slotsCounter++;
		}

		printf("\n---------------------------------\n");
	}

	void printYearCalendar(const short year) {
		cout << "---------------------------------\n";
		cout << "         Calendar - " << year << '\n';
		cout << "---------------------------------\n\n";

		for (short monthCounter = 1; monthCounter < 13; monthCounter++) {
			printMonthCalendar(monthCounter, year);
			cout << endl;
		}
	}

	bool isLastDayInMonth(const sDate & date) {
		return date.day == getDaysInMonth(date.month, date.year);
	}

	short getTotalDaysBefore(const sDate & date) {
		short totalDays = 0;

		//This for loop counts the days before the month considered
		for (short crrMonth = 1; crrMonth < date.month; crrMonth++)
			totalDays += getDaysInMonth(crrMonth, date.year);

		totalDays += date.day;

		return totalDays;
	}

	short getTotalDaysAfter(const sDate & date) {
		short totalDays = 0;

		//This for loop counts the days after the month considered
		for (short crrMonth = date.month + 1; crrMonth < 13; crrMonth++)
			totalDays += getDaysInMonth(crrMonth, date.year);

		totalDays += (getDaysInMonth(date.month, date.year) - date.day + 1);

		return totalDays;
	}

	sDate getDateByTotalDaysPassed(short totalDays, const short year) {
		sDate date;
		short daysInCrrMonth = 0;

		date.year = year;
		date.month = 1; //This just the intial value, it will be soon updated in the loop

		while (true) {
			daysInCrrMonth = getDaysInMonth(date.month, year);

			if (totalDays > daysInCrrMonth) {
				totalDays -= daysInCrrMonth;
				date.month++; //Here is the update
			} else {
				date.day = totalDays;

				return date;
			}
		}
	}

	sDate getDateAfterOneDay(sDate date) {
		if (isLastDayInMonth(date)) {
			if (date.month == 12) {
				date.day = 1;
				date.month = 1;
				date.year++;
			} else {
				date.day = 1;
				date.month++;
			}
		} else
			date.day++;

		return date;
	}

	//When adding so many days, potentially changing the year by a lot
	sDate getDateAfterManyDays(sDate date, unsigned int days) {
		unsigned int remainingDays = days + getTotalDaysBefore(date);
		short daysInCrrYear = 0;

		while (true) {
			daysInCrrYear = getDaysInYear(date.year);

			if (remainingDays > daysInCrrYear) {
				remainingDays -= daysInCrrYear;
				date.year++;
			} else
				return getDateByTotalDaysPassed(remainingDays, date.year);
		}
	}

	//When adding a few days, merely changing the month to some extent
	sDate getDateAfterFewDays(sDate date, short days) {
		for (days; days > 0; days--)
			date = getDateAfterOneDay(date);

		return date;
	}
	
	sDate getDateAfterSomeDays(sDate date, unsigned int days) {
		if (days <= 90)
			return getDateAfterFewDays(date, days);
		
		return getDateAfterManyDays(date, days);
	}

	sDate getDateAfterOneWeek(const sDate & date) {
		return getDateAfterFewDays(date, 7);
	}

	//When adding so many weeks, potentially changing the year by a lot
	sDate getDateAfterManyWeeks(const sDate & date, unsigned short weeks) {
		return getDateAfterManyDays(date, weeks * 7);
	}

	//When adding a few weeks, merely changing the month to some extent
	sDate getDateAfterFewWeeks(sDate date, short weeks) {
		for (weeks; weeks > 0; weeks--)
			date = getDateAfterOneWeek(date);

		return date;
	}

	sDate getDateAfterSomeWeeks(sDate date, unsigned short weeks) {
		if (weeks <= 13)
			return getDateAfterFewWeeks(date, weeks);

		return getDateAfterManyWeeks(date, weeks);
	}

	sDate getDateAfterSomeMonths(sDate date, unsigned short months = 1) {
		date.month += months;

		while (date.month > 12) {
			date.month -= 12;
			date.year++;
		}

		short totalDaysInCrrMonth = getDaysInMonth(date.month, date.year);
		if (date.day > totalDaysInCrrMonth)
			date.day = totalDaysInCrrMonth;

		return date;
	}

	sDate getDateAfterOneYear(sDate date) {
		date.year++;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			date.day--;

		return date;
	}

	sDate getDateAfterSomeYears(sDate date, unsigned short years) {
		date.year += years;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			if (!isYearLeap(date.year))
				date.day--;

		return date;
	}

	sDate getDateAfterOneDecade(sDate date) {
		date.year += 10;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			date.day--;

		return date;
	}

	sDate getDateAfterSomeDecades(sDate date, unsigned short decades) {
		date.year += 10 * decades;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			if (!isYearLeap(date.year))
				date.day--;

		return date;
	}

	sDate getDateAfterSomeCenturies(sDate date, short centuries = 1) {
		date.year += centuries * 100;

		//If the input is a leap year:
		if (date.month == 2 && date.day == 29)
			if (!isYearLeap(date.year))
				date.day--;

		return date;
	}

	sDate getDateAfterSomeMillennia(sDate date, short millennia = 1) {
		date.year += millennia * 1000;

		//If the input is a leap year:
		if (date.month == 2 && date.day == 29)
			if (!isYearLeap(date.year))
				date.day--;

		return date;
	}

	sDate getDateBeforeOneDay(sDate date) {
		if (date.day == 1) {
			if (date.month == 1) {
				date.day = 31;
				date.month = 12;
				date.year--;
			} else {
				date.month--;
				date.day = getDaysInMonth(date.month, date.year);
			}
		} else
			date.day--;

		return date;
	}

	//When subtracting so many days, potentially changing the year by a lot
	sDate getDateBeforeManyDays(sDate date, unsigned int days) {
		unsigned int remainingDays = days + getTotalDaysAfter(date);
		short daysInCrrYear = 0;

		while (true) {
			daysInCrrYear = getDaysInYear(date.year);

			if (remainingDays > daysInCrrYear) {
				remainingDays -= daysInCrrYear;
				date.year--;
			} else
				return getDateByTotalDaysPassed(daysInCrrYear - remainingDays + 1, date.year);
		}
	}

	//When subtracting few days, merely changing the month to some extent
	sDate getDateBeforeFewDays(sDate date, short days) {
		for (days; days > 0; days--)
			date = getDateBeforeOneDay(date);

		return date;
	}

	sDate getDateBeforeSomeDays(sDate date, unsigned int days) {
		if (days <= 90)
			return getDateBeforeFewDays(date, days);

		return getDateBeforeManyDays(date, days);
	}

	sDate getDateBeforeOneWeek(const sDate & date) {
		return getDateBeforeFewDays(date, 7);
	}

	//When subtracting so many weeks, potentially changing the year by a lot
	sDate getDateBeforeManyWeeks(const sDate & date, short weeks) {
		return getDateBeforeManyDays(date, weeks * 7);
	}

	//When subtracting few weeks, merely chaning the month to some extent
	sDate getDateBeforeFewWeeks(sDate date, short weeks) {
		for (weeks; weeks > 0; weeks--)
			date = getDateBeforeOneWeek(date);

		return date;
	}

	sDate getDateBeforeSomeWeeks(sDate date, unsigned short weeks) {
		if (weeks <= 13)
			return getDateBeforeFewWeeks(date, weeks);

		return getDateBeforeManyWeeks(date, weeks);
	}

	sDate getDateBeforeSomeMonths(sDate date, short months = 1) {
		date.month -= months;

		while (date.month < 1) {
			date.month += 12;
			date.year--;
		}

		short daysInCrrMonth = getDaysInMonth(date.month, date.year);
		if (date.day > daysInCrrMonth)
			date.day = daysInCrrMonth;

		return date;
	}

	sDate getDateBeforeOneYear(sDate date) {
		date.year--;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			date.day--;

		return date;
	}

	sDate getDateBeforeSomeYears(sDate date, short years) {
		date.year -= years;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			if (!isYearLeap(date.year))
				date.day--;

		return date;
	}

	sDate getDateBeforeOneDecade(sDate date) {
		date.year -= 10;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			date.day--;

		return date;
	}

	sDate getDateBeforeSomeDecades(sDate date, short decades) {
		date.year -= decades * 10;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			if (!isYearLeap(date.year))
				date.day--;

		return date;
	}

	sDate getDateBeforeSomeCenturies(sDate date, short centuries = 1) {
		date.year -= centuries * 100;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			if (!isYearLeap(date.year))
				date.day--;

		return date;
	}

	sDate getDateBeforeSomeMillennia(sDate date, short milleniums = 1) {
		date.year -= milleniums * 1000;

		//If the input was a leap year:
		if (date.month == 2 && date.day == 29)
			if (!isYearLeap(date.year))
				date.day--;

		return date;
	}

	bool isDateBeforeDate(const sDate & date1, const sDate & date2) {
		if (date1.year != date2.year)
			return date1.year < date2.year;

		if (date1.month != date2.month)
			return date1.month < date2.month;

		return date1.day < date2.day;
	}

	bool isDateAfterDate(const sDate & date1, const sDate & date2) {
		if (date1.year != date2.year)
			return date1.year > date2.year;

		if (date1.month != date2.month)
			return date1.month > date2.month;

		return date1.day > date2.day;
	}

	bool isDateEqualToDate(const sDate & date1, const sDate & date2) {
		return date1.day == date2.day && date1.month == date2.month && date1.year == date2.year;
	}

	bool isDateBetween(const sDate & date, sDate date1, sDate date2) {
		if (isDateAfterDate(date1, date2))
			swap(date1, date2);
		
		return !(isDateAfterDate(date, date2) || isDateBeforeDate(date, date1));
	}

	enDateCompareResults compareTwoDates(const sDate & date1, const sDate & date2) {
		if (isDateBeforeDate(date1, date2))
			return enDateCompareResults::eBefore;

		if (isDateEqualToDate(date1, date2))
			return enDateCompareResults::eEqual;

		return enDateCompareResults::eAfter;
	}

	int getDifferenceInDays(sDate date1, sDate date2, const bool includeEndDay = false) {
		bool shouldReturnNeg = false;

		if (shouldReturnNeg = isDateBeforeDate(date2, date1))
			swap(date1, date2);

	    int difference = 0;
		short date1Year = date1.year;

		while (date1Year < date2.year) {
			difference += getDaysInYear(date1Year);
			date1Year++;
		}

		difference += getTotalDaysBefore(date2) - getTotalDaysBefore(date1) + 1;

		if (!includeEndDay)
			difference--;

		return shouldReturnNeg ? -difference : difference;
	}

	sDate getSystemDate() {
		time_t timeNow = time(0);
		tm * timeNowStruct = localtime(&timeNow);
		sDate myDate;

		myDate.day = timeNowStruct->tm_mday;
		myDate.month = timeNowStruct->tm_mon + 1;
		myDate.year = timeNowStruct->tm_year + 1900;

		return myDate;
	}

	sTime getSystemTime(const enTimeSystem timeSystem = enTimeSystem::eTwentyFourHour) {
		time_t timeNow = time(0);
		tm * timeNowStruct = localtime(&timeNow);
		sTime myTime;

		myTime.sec = timeNowStruct->tm_sec;
		myTime.min = timeNowStruct->tm_min;
		myTime.hour = timeNowStruct->tm_hour; //[0 to 23]

		if (timeSystem == enTimeSystem::eTwelveHour) {
			myTime.timeSystem = enTimeSystem::eTwelveHour;

			if (myTime.hour < 12)
				myTime.meridiemIndicator = enMeridiemIndicators::eAM;
			else
				myTime.hour -= 12;

			if (myTime.hour == 0)
				myTime.hour = 12;
		}
	
		return myTime;
	}

	sDateTime getSystemDateTime(const enTimeSystem timeSystem = enTimeSystem::eTwentyFourHour) {
		time_t timeNow = time(0);
		tm * timeNowStruct = localtime(&timeNow);

		//Date:
		sDate myDate;
		myDate.day = timeNowStruct->tm_mday;
		myDate.month = timeNowStruct->tm_mon + 1;
		myDate.year = timeNowStruct->tm_year + 1900;

		//Time:
		sTime myTime;
		myTime.sec = timeNowStruct->tm_sec;
		myTime.min = timeNowStruct->tm_min;
		myTime.hour = timeNowStruct->tm_hour; //[0 to 23]

		if (timeSystem == enTimeSystem::eTwelveHour) {
			myTime.timeSystem = enTimeSystem::eTwelveHour;

			if (myTime.hour < 12)
				myTime.meridiemIndicator = enMeridiemIndicators::eAM;
			else
				myTime.hour -= 12;

			if (myTime.hour == 0)
				myTime.hour = 12;
		}

		//DateTime Struct:
		return {myDate, myTime};
	}

	//Sunday is 0
	short getSystemDayPosInWeek() {
		time_t timeNow = time(0);
		return localtime(&timeNow)->tm_wday;
	}

	string getSystemDayName(const bool isAbbreviated = false) {
		return getDayName(getSystemDayPosInWeek(), isAbbreviated);
	}

	//This function may also be used to get total days from now until a future date, only take the absolute value of the result
	int getTotalDaysUntilNow(const sDate oldDate, const bool includeEndDay = false) {
		return getDifferenceInDays(oldDate, getSystemDate(), includeEndDay);
	}

	bool isEndOfWeek(const sDate & date, enWeekDays endOfWeek = enWeekDays::eSat) {
		return doesDateCoincideWith(date, endOfWeek);
	}

	bool isWeekEnd(const sDate & date, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		enWeekDays crrWeekDay = (enWeekDays) getDayPosInWeek(date);

		switch (weekEndDays) {
			case enWeekEndVariants::eFriday:
				return crrWeekDay == enWeekDays::eFri;
			case enWeekEndVariants::eFriSat:
				return crrWeekDay == enWeekDays::eFri || crrWeekDay == enWeekDays::eSat;
			case enWeekEndVariants::eSatSun:
				return crrWeekDay == enWeekDays::eSat || crrWeekDay == enWeekDays::eSun;
			default:
				return false;
		}	
	}

	bool isBusinessDay(const sDate & date, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		return !isWeekEnd(date, weekEndDays);
	}

	short getTotalDaysUntilEndOfWeek(const sDate & date, enWeekDays endOfWeek = enWeekDays::eSat, const bool includeStartingDay = false) {
		short daysDifference = (short) endOfWeek - getDayPosInWeek(date);

		if (daysDifference < 0)
			daysDifference += 7;

		return includeStartingDay ? daysDifference + 1 : daysDifference;
	}

	short getTotalDaysUntilEndOfMonth(const sDate & date, const bool includeStartingDay = false) {
		short daysDifference = getDaysInMonth(date.month, date.year) - date.day;
		return includeStartingDay ? daysDifference + 1 : daysDifference;
	}

	short getTotalDaysUntilEndOfYear(const sDate & date, const bool includeStartingDay = false) {
		short daysDifference = getDaysInYear(date.year) - getTotalDaysBefore(date);
		return includeStartingDay ? daysDifference + 1 : daysDifference;
	}

	//Returns the difference in days between startDate and returnDate excluding weekends and returnDate
	short getVacationDays(sDate startDate, const sDate & returnDate, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		short vacationDaysCounter = 0;

		while (isDateBeforeDate(startDate, returnDate)) {
			if (isBusinessDay(startDate, weekEndDays))
				vacationDaysCounter++;

			startDate = getDateAfterOneDay(startDate);
		}

		return vacationDaysCounter;
	}

	short getBusinessDays(sDate startDate, const sDate & endDate, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		return getVacationDays(startDate, endDate, weekEndDays);
	}

	//Returns the returnDate after adding vacationDays plus weekends to startDate
	sDate getVacationReturnDate(sDate startDate, short vacationDays, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		while (vacationDays > 0) {
			if (isBusinessDay(startDate, weekEndDays))
				vacationDays--;
			
			startDate = getDateAfterOneDay(startDate);
		}

		return startDate;
	}
	
	bool doesTwoPeriodsOverlap(const sPeriod & period1, const sPeriod & period2) {
		return !(isDateAfterDate(period2.startDate, period1.endDate) || isDateAfterDate(period1.startDate, period2.endDate));
	}

	//Returns the length in days
	int getPeriodLength(const sPeriod & period, const bool includeEndDay = false) {
		return getDifferenceInDays(period.startDate, period.endDate, includeEndDay);
	}

	bool doesDateExistInPeriod(const sDate & date, const sPeriod & period) {
		return !(isDateBeforeDate(date, period.startDate) || isDateAfterDate(date, period.endDate));
	}

	unsigned short getPeriodsOverlapDays(const sPeriod & period1, const sPeriod & period2) {
		if (doesTwoPeriodsOverlap(period1, period2)) {
			unsigned short sumOfPeriods = getPeriodLength(period1, true) + getPeriodLength(period2, true);

			sumOfPeriods -= abs(getDifferenceInDays(period1.startDate, period2.startDate));
			sumOfPeriods -= abs(getDifferenceInDays(period1.endDate, period2.endDate));

			return sumOfPeriods / 2; //sumOfPeriods is always even
		} else
			return 0;
	}

	sDate getAge(const sDate & dateOfBirth, const bool includeEndDay = false) {
		sDate age, systemDate = getSystemDate();

		//Calculating days:
		while (systemDate.day < dateOfBirth.day) { //DON'T replace this while with an if!
			systemDate.day += getDaysInMonth(systemDate.month, systemDate.year);
			systemDate.month--;
		}
		age.day = systemDate.day - dateOfBirth.day;

		//Calculating months:
		if (systemDate.month < dateOfBirth.month) {
			systemDate.month += 12;
			systemDate.year--;
		}
		age.month = systemDate.month - dateOfBirth.month;

		//Calculating year:
		age.year = systemDate.year - dateOfBirth.year;

		if (includeEndDay)
			age = getDateAfterOneDay(age);

		return age;
	}

	sDate getAge(const sDate & dateOfBirth, sDate & endDate, const bool includeEndDay = false) {
		sDate age;

		//Calculating days:
		while (endDate.day < dateOfBirth.day) { //DON'T replace this while with an if!
			endDate.day += getDaysInMonth(endDate.month, endDate.year);
			endDate.month--;
		}
		age.day = endDate.day - dateOfBirth.day;

		//Calculating months:
		if (endDate.month < dateOfBirth.month) {
			endDate.month += 12;
			endDate.year--;
		}
		age.month = endDate.month - dateOfBirth.month;

		//Calculating year:
		age.year = endDate.year - dateOfBirth.year;

		if (includeEndDay)
			age = getDateAfterOneDay(age);

		return age;
	}

	string readDateFormat(const string msg, const string header = "", const bool showErrorMsg = true) {
		string format;
		bool hasFailed = false;

		while (true) {
			system("cls");

			cout << header;
			cout << "* Optionally, you can insert [yy] to indicate the first 2 digits of the year.\n";
			cout << "* You can repeat date parts (dd, mm, yyyy).\n\n";

			if (hasFailed && showErrorMsg)
				cout << "Error: The format must contain at least one of every (dd, mm, yyyy).\n\n";

			format = readers::readString(msg, true);
			
			if (isDateFormatValid(format))
				return format;
			else
				hasFailed = true;
		}
	}

	sDate readDateAsStr(const string msg, const string & format, const bool showErrorMsg = true) {
		sDate date;
		string strDate;
		enValidateDateStrResults validationResult;

		while (true) {
			strDate = readers::readString(msg, true);
			validationResult = isDateStrValid(strDate, format, date);

			if (showErrorMsg) {
				switch (validationResult) {
					case dates::eErrorInText:
						cout << "Error: Please obey the accepted format.\n\n";
						break;
					case dates::eErrorInYYValue:
						cout << "Error: yy value inconsistency.\n\n";
						break;
					case dates::eErrorInValuesConsistency:
						cout << "Error: Values inconsistency.\n\n";
						break;
					case dates::eErrorInYYYYValue:
						cout << "Error: yyyy value inconsistency.\n\n";
						break;
					case dates::eErrorInDateValues:
						cout << "Error: This date doesn't exist.\n\n";
						break;
					case dates::eNoErrors:
						return date;
				}
			} else if (validationResult == dates::eNoErrors)
				return date;
		}
	}

	sDate readDateOfBirth(const string msg, const string & format, const bool showErrorMsg = true) {
		sDate systemDate = getSystemDate();
		sDate dateOfBirth;

		while (true) {
			dateOfBirth = readDateAsStr(msg, format, showErrorMsg);

			if (isDateAfterDate(dateOfBirth, systemDate))
				cout << "Error: You must enter a date in the past.\n\n";
			else
				return dateOfBirth;
		}
	}

	enWeekEndVariants readWeekEndDays(const string header = "") {
		cout << header;
		cout << "* Here are the available weekend variants:\n";
		cout << "-----------------------------------------------\n";
		cout << "[1]: Friday\n";
		cout << "[2]: Friday-Saturday\n";
		cout << "[3]: Saturday-Sunday\n";
		cout << "--------------------------\n";

		return static_cast<enWeekEndVariants>(readers::readNumber<short>("Choose [1] to [3]: ", 1, 3) - 1);
	}

	enWeekDays readWeekDay(const string msg, const string header = "") {
		cout << header << msg;
		cout << "-----------------------------------\n";
		cout << "[1]: Sunday      [2]: Monday\n";
		cout << "[3]: Tuesday     [4]: Wednesday\n";
		cout << "[5]: Thursday    [6]: Friday\n";
		cout << "[7]: Saturday\n";
		cout << "-----------------------------------\n";
		
		return static_cast<enWeekDays>(readers::readNumber<short>("Choose [1] to [7]: ", 1, 7) - 1);
	}

}


