// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include <string>
#include <vector>
#include "B:\PROGRAMMING\Abu-Hadhoud\My Classes\StringHandler.h"

using namespace std;

class Currency {
	enum enModes { eEmptyMode, eUpdateMode };

private:
	enModes _Mode;
	string _Country; //Read-Only
	string _CurrencyCode; //Read-Only
	string _CurrencyName; //Read-Only
	float _Rate;

	static string _convertCurrencyObjectToLine(const Currency & C, const string & _separator = "#//#") {
		string _result = "";

		_result += C.getCountry() + _separator;
		_result += C.getCurrencyCode() + _separator;
		_result += C.getCurrencyName() + _separator;
		_result += to_string(C.getRate());

		return _result;
	}

	static Currency _convertCurrencyLineToObject(const string & _line, const string & _separator = "#//#") {
		const vector<string> _vLineParts = StringHandler::getVectorOfTokens(_line, _separator);
		
		return Currency(enModes::eUpdateMode, _vLineParts[0], _vLineParts[1], _vLineParts[2], stof(_vLineParts[3]));
	}

	static Currency _getEmptyCurrency() {
		return Currency(enModes::eEmptyMode, "", "", "", 0);
	}

	static vector<Currency> _loadCurrenciesFromFile() {
		fstream _currenciesFile("Currencies.txt", ios::in);
		vector<Currency> _vCurrencies;

		if (_currenciesFile.is_open()) {
			string _line;

			while (getline(_currenciesFile, _line))
				_vCurrencies.push_back(_convertCurrencyLineToObject(_line));

			_currenciesFile.close();
		}
		else
			cerr << "\nError: Cannot open file: Currencies.txt [getAllCurrencies()]";

		return _vCurrencies;
	}

	void _saveCurrenciesToFile(const vector<Currency> & _vCurrencies) {
		fstream _currenciesFile("Currencies.txt", ios::out);

		if (_currenciesFile.is_open()) {
			for (const Currency & C : _vCurrencies)
				_currenciesFile << _convertCurrencyObjectToLine(C) << '\n';

			_currenciesFile.close();
		}
		else
			cerr << "\nError: Cannot open file: Currencies.txt [_saveCurrenciesToFile()]";
	}

public:
	Currency(const enModes & mode, const string & country, const string & currencyCode,
		const string & currencyName, const float & rate)
	{
		_Mode = mode;
		_Country = country;
		_CurrencyCode = currencyCode;
		_CurrencyName = currencyName;
		_Rate = rate;
	}

	Currency(){}

	bool isEmpty() const {
		return _Mode == enModes::eEmptyMode;
	}

	string getCountry() const {
		return _Country;
	}
	__declspec(property(get = getCountry)) string Country;
	
	string getCurrencyCode() const {
		return _CurrencyCode;
	}
	__declspec(property(get = getCurrencyCode)) string CurrencyCode;
	
	string getCurrencyName() const {
		return _CurrencyName;
	}
	__declspec(property(get = getCurrencyName)) string CurrencyName;

	float getRate() const {
		return _Rate;
	}
	void setRate(const float & rate, const bool & refreshFile) {
		_Rate = rate;

		if (refreshFile) {
			vector<Currency> _vCurrencies = _loadCurrenciesFromFile();

			for (Currency & C : _vCurrencies)
				if (C.getCurrencyCode() == _CurrencyCode) {
					C.setRate(_Rate, false);
					break;
				}

			_saveCurrenciesToFile(_vCurrencies);
		}
	}
	__declspec(property(get = getRate, put = putRate)) float Rate;

	static vector<Currency> getAllCurrencies() {
		return _loadCurrenciesFromFile();
	}

	static Currency findByCountry(const string & country) {
		fstream _currenciesFile("Currencies.txt", ios::in);

		if (_currenciesFile.is_open()) {
			string _line;
			Currency _currency;

			while (getline(_currenciesFile, _line)) {
				_currency = _convertCurrencyLineToObject(_line);

				if (strings::lowAllLtrs(_currency.getCountry()) == strings::lowAllLtrs(country)) {
					_currenciesFile.close();
					return _currency;
				}
			}

			_currenciesFile.close();
			return _getEmptyCurrency();
		}
		else
			cerr << "Error: Cannot open file: Currencies.txt [findByCountry()]";
	}

	static Currency findByCode(const string & code) {
		fstream _currenciesFile("Currencies.txt", ios::in);

		if (_currenciesFile.is_open()) {
			string _line;
			Currency _currency;

			while (getline(_currenciesFile, _line)) {
				_currency = _convertCurrencyLineToObject(_line);

				if (_currency.getCurrencyCode() == strings::capAllLtrs(code)) {
					_currenciesFile.close();
					return _currency;
				}
			}

			_currenciesFile.close();
			return _getEmptyCurrency();
		}
		else
			cerr << "Erro: Cannot open file: Currencies.txt [findByCountry()]";
	}

	float convertTo(const Currency & currencyTo, const float & amount) const {
		return amount * currencyTo.getRate() / _Rate;
	}

};
