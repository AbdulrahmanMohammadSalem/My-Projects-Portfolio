/// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

//Default date format used here is: dd/mm/yyyy

#pragma once
#pragma warning(disable: 4996)

#include "Dates.h"

using namespace dates;

class DateHandler {
private:
	short _day = 0;
	short _month = 0;
	short _year = 0;
	string _format = "dd/mm/yyyy";

public:
	///Constructors - If the input is not valid, the date will be stored as: 0/0/0
	DateHandler() {
		sDate sysDate = dates::getSystemDate();

		_day = sysDate.day;
		_month = sysDate.month;
		_year = sysDate.year;
	}
	DateHandler(const string dateString) {
		sDate date;

		if (isDateStrValid(dateString, _format, date) == enValidateDateStrResults::eNoErrors) {
			_day = date.day;
			_month = date.month;
			_year = date.year;
		}
	}
	DateHandler(const short day, const short month, const short year) {
		if (dates::isDateValid({ day, month, year })) {
			_day = day;
			_month = month;
			_year = year;
		}
	}
	DateHandler(const short dayOrder, const short year) {
		if (dayOrder <= dates::getDaysInYear(year)) {
			sDate date = getDateByTotalDaysPassed(dayOrder, year);

			_day = date.day;
			_month = date.month;
			_year = date.year;
		}
	}

	///Getters & Setter:
	short getDay() const {
		return _day;
	}
	void setDay(const short newDay) {
		_day = newDay;
	}

	short getMonth() const {
		return _month;
	}
	void setMonth(const short newMonth) {
		_month = newMonth;
	}

	short getYear() const {
		return _year;
	}
	void setYear(const short newYear) {
		_year = newYear;
	}

	string getFormat() const {
		return _format;
	}
	void setFormat(const string newFormat) {
		if (isDateFormatValid(newFormat))
			_format = newFormat;
	}

	///Declaration Specifications:
	__declspec(property(get = getDay, put = setDay)) short Day;
	__declspec(property(get = getMonth, put = setMonth)) short Month;
	__declspec(property(get = getYear, put = setYear)) short Year;
	__declspec(property(get = getFormat, put = setFormat)) string Format;

	///Methods:
	static sDate toStruct(const DateHandler date) {
		return { date.getDay(), date.getMonth(), date.getYear() };
	}
	sDate toStruct() {
		return {_day, _month, _year};
	}

	static bool isYearLeap(const short year){
		return dates::isYearLeap(year);
	}
	bool isLeap() {
		return dates::isYearLeap(_year);
	}

	static short getDaysInYear(const short year) {
		return dates::getDaysInYear(year);
	}
	short getDaysInYear() {
		return dates::getDaysInYear(_year);
	}

	static short getHoursInYear(const short year) {
		return dates::getHoursInYear(year);
	}
	short getHoursInYear() {
		return dates::getHoursInYear(_year);
	}

	static short getMinutesInYear(const short year) {
		return getMinsInYear(year);
	}
	short getMinutesInYear() {
		return getMinsInYear(_year);
	}

	static short getSecondsInYear(const short year) {
		return getSecsInYear(year);
	}
	short getSecondsInYear() {
		return getSecsInYear(_year);
	}

	static short getDaysInMonth(const short month, const short year) {
		return dates::getDaysInMonth(month, year);
	}
	short getDaysInMonth() {
		return dates::getDaysInMonth(_month, _year);
	}

	static short getHoursInMonth(const short month, const short year) {
		return dates::getHoursInMonth(month, year);
	}
	short getHoursInMonth() {
		return dates::getHoursInMonth(_month, _year);
	}

	static unsigned short getMinutesInMonth(const short month, const short year) {
		return dates::getMinsInMonth(month, year);
	}
	unsigned short getMinutesInMonth() {
		return dates::getMinsInMonth(_month, _year);
	}

	static int getSecondsInMonth(const short month, const short year) {
		return getSecsInMonth(month, year);
	}
	int getSecondsInMonth(){
		return getSecsInMonth(_month, _year);
		}

	static short getMinutesInDay() {
		return getMinsInDay();
	}

	static int getSecondsInDay() {
		return getSecsInDay();
	}

	static string getMonthName(const short month, const bool isAbbreviated = false) {
		return dates::getMonthName(month, isAbbreviated);
	}
	string getMonthName(const bool isAbbreviated = false) {
		return dates::getMonthName(_month, isAbbreviated);
	}

	//Sunday is 0
	static short getDayPositionInWeek(DateHandler & date) {
		return getDayPosInWeek(date.toStruct());
	}
	short getPositionInWeek() {
		return getDayPositionInWeek(*this);
	}

	static bool doesDateCoincideWith(DateHandler & date, const enWeekDays weekday) {
		return dates::doesDateCoincideWith(date.toStruct(), weekday);
	}
	bool doesCoincideWith(const enWeekDays weekday) {
		return doesDateCoincideWith(*this, weekday);
	}

	static string getDayName(const short posInWeek, const bool isAbbreviated = false) {
		return dates::getDayName(posInWeek, isAbbreviated);
	}
	
	static string getDayName(DateHandler & date, const bool isAbbreviated = false) {
		return dates::getDayName(date.toStruct(), isAbbreviated);
	}
	string getDayName(const bool isAbbreviated) {
		return getDayName(*this, isAbbreviated);
	}

	static bool isDateValid(DateHandler & date) {
		return dates::isDateValid(date.toStruct());
	}
	bool isValid() {
		return isDateValid(*this);
	}

	static bool isDateFormatValid(const string & format) {
		return dates::isDateFormatValid(format);
	}

	static enValidateDateStrResults isDateValid(string strDate, string format, sDate & extractTo) {
		return isDateStrValid(strDate, format, extractTo);
	}
	static enValidateDateStrResults isDateValid(string strDate, string format) {
		return isDateStrValid(strDate, format);
	}

	static string formatDate(DateHandler & date, string format) {
		return dates::formatDate(date.toStruct(), format);
	}
	string format(string format) {
		return formatDate(*this, format);
	}
	string format() {
		return formatDate(*this, _format);
	}

	static void printMonthCalendar(const short month, const short year) {
		dates::printMonthCalendar(month, year);
	}
	void printMonthCalendar() {
		dates::printMonthCalendar(_month, _year);
	}

	static void printYearCalendar(const short year) {
		dates::printYearCalendar(year);
	}
	void printYearCalendar() {
		dates::printYearCalendar(_year);
	}

	static bool isLastDayInMonth(DateHandler & date) {
		return dates::isLastDayInMonth(date.toStruct());
	}
	bool isLastDayInMonth() {
		return isLastDayInMonth(*this);
	}

	static short getTotalDaysBefore(DateHandler & date) {
		return dates::getTotalDaysBefore(date.toStruct());
	}
	short getTotalDaysBefore() {
		return getTotalDaysBefore(*this);
	}
	
	static short getTotalDaysAfter(DateHandler & date) {
		return dates::getTotalDaysAfter(date.toStruct());
	}
	short getTotalDaysAfter() {
		return getTotalDaysAfter(*this);
	}

	static DateHandler getDateByDayOrder(short dayOrder, const short year) {
		sDate result = getDateByTotalDaysPassed(dayOrder, year);

		return DateHandler(result.day, result.month, result.year);
	}

	static DateHandler getDateAfterOneDay(DateHandler date) {
		sDate result = dates::getDateAfterOneDay(date.toStruct());

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterOneDay() {
		*this = getDateAfterOneDay(*this);
	}

	static DateHandler getDateAfterManyDays(DateHandler date, unsigned short days) {
		sDate result = dates::getDateAfterManyDays(date.toStruct(), days);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterManyDays(unsigned short days) {
		*this = getDateAfterManyDays(*this, days);
	}

	static DateHandler getDateAfterFewDays(DateHandler date, short days) {
		sDate result = dates::getDateAfterFewDays(date.toStruct(), days);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterFewDays(short days) {
		*this = getDateAfterFewDays(*this, days);
	}

	static DateHandler getDateAfterSomeDays(DateHandler date, unsigned short days) {
		sDate result = dates::getDateAfterSomeDays(date.toStruct(), days);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterSomeDays(unsigned short days) {
		*this = getDateAfterSomeDays(*this, days);
	}

	static DateHandler getDateAfterOneWeek(DateHandler date) {
		sDate result = dates::getDateAfterOneWeek(date.toStruct());

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterOneWeek() {
		*this = getDateAfterOneWeek(*this);
	}

	static DateHandler getDateAfterManyWeeks(DateHandler date, unsigned short weeks) {
		sDate result = dates::getDateAfterManyWeeks(date.toStruct(), weeks);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterManyWeeks(unsigned short weeks) {
		*this = getDateAfterManyWeeks(*this, weeks);
	}

	static DateHandler getDateAfterFewWeeks(DateHandler date, short weeks) {
		sDate result = dates::getDateAfterFewWeeks(date.toStruct(), weeks);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterFewWeeks(short weeks) {
		*this = getDateAfterFewWeeks(*this, weeks);
	}

	static DateHandler getDateAfterSomeWeeks(DateHandler date, unsigned short weeks) {
		sDate result = dates::getDateAfterSomeWeeks(date.toStruct(), weeks);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterSomeWeeks(unsigned short weeks) {
		*this = getDateAfterSomeWeeks(*this, weeks);
	}

	static DateHandler getDateAfterSomeMonths(DateHandler date, unsigned short months) {
		sDate result = dates::getDateAfterSomeMonths(date.toStruct(), months);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterSomeMonths(unsigned short months) {
		*this = getDateAfterSomeMonths(*this, months);
	}

	static DateHandler getDateAfterOneYear(DateHandler date) {
		sDate result = dates::getDateAfterOneYear(date.toStruct());

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterOneYear() {
		*this = getDateAfterOneYear(*this);
	}

	static DateHandler getDateAfterSomeYears(DateHandler date, short years) {
		sDate result = dates::getDateAfterSomeYears(date.toStruct(), years);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterSomeYears(short years) {
		*this = getDateAfterSomeYears(*this, years);
	}

	static DateHandler getDateAfterOneDecade(DateHandler date) {
		sDate result = dates::getDateAfterOneDecade(date.toStruct());

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterOneDecade() {
		*this = getDateAfterOneDecade(*this);
	}

	static DateHandler getDateAfterSomeDecades(DateHandler date, short decades) {
		sDate result = dates::getDateAfterSomeDecades(date.toStruct(), decades);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterSomeDecades(short decades) {
		*this = getDateAfterSomeDecades(*this, decades);
	}

	static DateHandler getDateAfterSomeCenturies(DateHandler date, short centuries) {
		sDate result = dates::getDateAfterSomeCenturies(date.toStruct(), centuries);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterSomeCenturies(short centuries) {
		*this = getDateAfterSomeCenturies(*this, centuries);
	}

	static DateHandler getDateAfterSomeMillennia(DateHandler date, short millennia) {
		sDate result = dates::getDateAfterSomeMillennia(date.toStruct(), millennia);

		return DateHandler(result.day, result.month, result.year);
	}
	void getAfterSomeMillennia(short millennia) {
		*this = getDateAfterSomeMillennia(*this, millennia);
	}

	static DateHandler getDateBeforeOneDay(DateHandler date) {
		sDate result = dates::getDateBeforeOneDay(date.toStruct());

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeOneDay() {
		*this = getDateBeforeOneDay(*this);
	}

	static DateHandler getDateBeforeManyDays(DateHandler date, unsigned short days) {
		sDate result = dates::getDateBeforeManyDays(date.toStruct(), days);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeManyDays(unsigned short days) {
		*this = getDateBeforeManyDays(*this, days);
	}

	static DateHandler getDateBeforeFewDays(DateHandler date, short days) {
		sDate result = dates::getDateBeforeFewDays(date.toStruct(), days);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeFewDays(short days) {
		*this = getDateBeforeFewDays(*this, days);
	}

	static DateHandler getDateBeforeSomeDays(DateHandler date, unsigned short days) {
		sDate result = dates::getDateBeforeSomeDays(date.toStruct(), days);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeSomeDays(unsigned short days) {
		*this = getDateBeforeSomeDays(*this, days);
	}

	static DateHandler getDateBeforeOneWeek(DateHandler date) {
		sDate result = dates::getDateBeforeOneWeek(date.toStruct());

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeOneWeek() {
		*this = getDateBeforeOneWeek(*this);
	}

	static DateHandler getDateBeforeManyWeeks(DateHandler date, unsigned short weeks) {
		sDate result = dates::getDateBeforeManyWeeks(date.toStruct(), weeks);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeManyWeeks(unsigned short weeks) {
		*this = getDateBeforeManyWeeks(*this, weeks);
	}

	static DateHandler getDateBeforeFewWeeks(DateHandler date, short weeks) {
		sDate result = dates::getDateBeforeFewWeeks(date.toStruct(), weeks);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeFewWeeks(short weeks) {
		*this = getDateBeforeFewWeeks(*this, weeks);
	}

	static DateHandler getDateBeforeSomeWeeks(DateHandler date, unsigned short weeks) {
		sDate result = dates::getDateBeforeSomeWeeks(date.toStruct(), weeks);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeSomeWeeks(unsigned short weeks) {
		*this = getDateBeforeSomeWeeks(*this, weeks);
	}

	static DateHandler getDateBeforeSomeMonths(DateHandler date, unsigned short months) {
		sDate result = dates::getDateBeforeSomeMonths(date.toStruct(), months);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeSomeMonths(unsigned short months) {
		*this = getDateBeforeSomeMonths(*this, months);
	}

	static DateHandler getDateBeforeOneYear(DateHandler date) {
		sDate result = dates::getDateBeforeOneYear(date.toStruct());

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeOneYear() {
		*this = getDateBeforeOneYear(*this);
	}

	static DateHandler getDateBeforeSomeYears(DateHandler date, short years) {
		sDate result = dates::getDateBeforeSomeYears(date.toStruct(), years);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeSomeYears(short years) {
		*this = getDateBeforeSomeYears(*this, years);
	}

	static DateHandler getDateBeforeOneDecade(DateHandler date) {
		sDate result = dates::getDateBeforeOneDecade(date.toStruct());

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeOneDecade() {
		*this = getDateBeforeOneDecade(*this);
	}

	static DateHandler getDateBeforeSomeDecades(DateHandler date, short decades) {
		sDate result = dates::getDateBeforeSomeDecades(date.toStruct(), decades);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeSomeDecades(short decades) {
		*this = getDateBeforeSomeDecades(*this, decades);
	}

	static DateHandler getDateBeforeSomeCenturies(DateHandler date, short centuries) {
		sDate result = dates::getDateBeforeSomeCenturies(date.toStruct(), centuries);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeSomeCenturies(short centuries) {
		*this = getDateBeforeSomeCenturies(*this, centuries);
	}

	static DateHandler getDateBeforeSomeMillennia(DateHandler date, short millennia) {
		sDate result = dates::getDateBeforeSomeMillennia(date.toStruct(), millennia);

		return DateHandler(result.day, result.month, result.year);
	}
	void getBeforeSomeMillennia(short millennia) {
		*this = getDateBeforeSomeMillennia(*this, millennia);
	}

	static bool isDateBeforeDate(DateHandler & date1, DateHandler & date2) {
		return dates::isDateBeforeDate(date1.toStruct(), date2.toStruct());
	}
	bool isBeforeDate(DateHandler & date) {
		return isDateBeforeDate(*this, date);
	}

	static bool isDateAfterDate(DateHandler & date1, DateHandler & date2) {
		return dates::isDateAfterDate(date1.toStruct(), date2.toStruct());
	}
	bool isAfterDate(DateHandler & date) {
		return isDateAfterDate(*this, date);
	}

	static bool isDateEqualToDate(DateHandler & date1, DateHandler & date2) {
		return dates::isDateEqualToDate(date1.toStruct(), date2.toStruct());
	}
	bool equals(DateHandler & date) {
		return isDateEqualToDate(*this, date);
	}

	static bool isDateBetween(DateHandler & date, DateHandler date1, DateHandler date2) {
		return dates::isDateBetween(date.toStruct(), date1.toStruct(), date2.toStruct());
	}
	bool isBetween(DateHandler date1, DateHandler date2) {
		return isDateBetween(*this, date1, date2);
	}

	static int getDifferenceInDays(DateHandler date1, DateHandler date2, const bool includeEndDay = false) {
		return dates::getDifferenceInDays(date1.toStruct(), date2.toStruct(), includeEndDay);
	}
	int getDifferenceInDaysFrom(const DateHandler date, const bool includeEndDay = false) {
		return getDifferenceInDays(*this, date, includeEndDay);
	}

	static DateHandler getSystemDate() {
		sDate result = dates::getSystemDate();

		return DateHandler(result.day, result.month, result.year);
	}

	//Sunday is 0
	static short getSystemDayPositionInWeek() {
		return dates::getSystemDayPosInWeek();
	}

	static string getSystemDayName(const bool isAbbreviated = false) {
		return dates::getSystemDayName(isAbbreviated);
	}

	//This function may also be used to get total days from now until a future date, only take the absolute value of the result
	static int getTotalDaysUntilNow(DateHandler oldDate, const bool includeEndDay = false) {
		return dates::getTotalDaysUntilNow(oldDate.toStruct(), includeEndDay);
	}

	static bool isEndOfWeek(DateHandler & date, enWeekDays endOfWeek = enWeekDays::eSat) {
		return dates::isEndOfWeek(date.toStruct(), endOfWeek);
	}
	bool isEndOfWeek(enWeekDays endOfWeek = enWeekDays::eSat) {
		return isEndOfWeek(*this, endOfWeek);
	}

	static bool isWeekEnd(DateHandler & date, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		return dates::isWeekEnd(date.toStruct(), weekEndDays);
	}
	bool isWeekEnd(const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		return isWeekEnd(*this, weekEndDays);
	}

	static bool isBusinessDay(DateHandler & date, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		return dates::isBusinessDay(date.toStruct(), weekEndDays);
	}
	bool isBusinessDay(const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		return isBusinessDay(*this, weekEndDays);
	}

	static short getTotalDaysUntilEndOfWeek(DateHandler & date, enWeekDays endOfWeek = enWeekDays::eSat, const bool includeStartingDay = false) {
		return dates::getTotalDaysUntilEndOfWeek(date.toStruct(), endOfWeek, includeStartingDay);
	}
	short getTotalDaysUntilEndOfWeek(enWeekDays endOfWeek = enWeekDays::eSat, const bool includeStartingDay = false) {
		return getTotalDaysUntilEndOfWeek(*this, endOfWeek, includeStartingDay);
	}

	static short getTotalDaysUntilEndOfMonth(DateHandler & date, const bool includeStartingDay = false) {
		return dates::getTotalDaysUntilEndOfMonth(date.toStruct(), includeStartingDay);
	}
	short getTotalDaysUntilEndOfMonth(const bool includeStartingDay = false) {
		return getTotalDaysUntilEndOfMonth(*this, includeStartingDay);
	}

	static short getTotalDaysUntilEndOfYear(DateHandler & date, const bool includeStartingDay = false) {
		return dates::getTotalDaysUntilEndOfYear(date.toStruct(), includeStartingDay);
	}
	short getTotalDaysUntilEndOfYear(const bool includeStartingDay = false) {
		return getTotalDaysUntilEndOfYear(*this, includeStartingDay);
	}

	//Returns the difference in days between startDate and returnDate excluding weekends and returnDate
	static short getVacationDays(DateHandler startDate, DateHandler & returnDate, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		return dates::getVacationDays(startDate.toStruct(), returnDate.toStruct(), weekEndDays);
	}

	//Returns the returnDate after adding vacationDays plus weekends to startDate
	static DateHandler getVacationReturnDate(DateHandler startDate, short vacationDays, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		sDate result = dates::getVacationReturnDate(startDate.toStruct(), vacationDays, weekEndDays);

		return DateHandler(result.day, result.month, result.year);
	}

	static short getBusinessDays(DateHandler startDate, DateHandler & endDate, const enWeekEndVariants weekEndDays = enWeekEndVariants::eFriday) {
		return dates::getVacationDays(startDate.toStruct(), endDate.toStruct(), weekEndDays);
	}

	static DateHandler getAge(DateHandler & dateOfBirth, const bool includeEndDay = false) {
		sDate result = dates::getAge(dateOfBirth.toStruct(), includeEndDay);

		return DateHandler(result.day, result.month, result.year);
	}

	static DateHandler getAge(DateHandler & dateOfBirth, DateHandler & endDate, const bool includeEndDay = false) {
		sDate dtOfBirth = dateOfBirth.toStruct();
		sDate endDt = endDate.toStruct();
		sDate result = dates::getAge(dtOfBirth, endDt, includeEndDay);

		return DateHandler(result.day, result.month, result.year);
	}

	static string readDateFormat(const string msg, const string header = "", const bool showErrorMsg = true) {
		return dates::readDateFormat(msg, header, showErrorMsg);
	}

	static DateHandler readDateAsStr(const string msg, const string & format, const bool showErrorMsg = true) {
		sDate result = dates::readDateAsStr(msg, format, showErrorMsg);

		return DateHandler(result.day, result.month, result.year);
	}

	static DateHandler readDateOfBirth(const string msg, const string & format, const bool showErrorMsg = true) {
		sDate result = dates::readDateOfBirth(msg, format, showErrorMsg);

		return DateHandler(result.day, result.month, result.year);
	}

	static enWeekEndVariants readWeekEndDays(const string header = "") {
		return dates::readWeekEndDays(header);
	}

	static enWeekDays readWeekDay(const string msg, const string header = "") {
		return dates::readWeekDay(msg, header);
	}

};

