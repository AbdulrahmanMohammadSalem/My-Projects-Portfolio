// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include <iostream>
#include "Constants.h"

using namespace std;

class Person {
    ///Properties:
private:
    string _FirstName;
    string _LastName;
    string _Email;
    string _Phone;

public:
    ///Constructor:
    Person(const string firstName, const string lastName,
        const string email, const string phone) {
        _FirstName = firstName;
        _LastName = lastName;
        _Email = email;
        _Phone = phone;
    }

    Person(){}

    ///Properties setting up:
    string getFirstName() const  {
        return _FirstName;
    }
    void setFirstName(const string firstName) {
        _FirstName = firstName;
    }
    __declspec(property(get = getFirstName, put = setFirstName)) string FirstName;

    string getLastName() const {
        return _LastName;
    }
    void setLastName(const string lastName) {
        _LastName = lastName;
    }
    __declspec(property(get = getLastName, put = setLastName)) string LastName;

    string getEmail() const {
        return _Email;
    }
    void setEmail(const string email) {
        _Email = email;
    }
    __declspec(property(get = getEmail, put = setEmail)) string Email;

    string getPhone() const {
        return _Phone;
    }
    void setPhone(const string phone) {
        _Phone = phone;
    }
    __declspec(property(get = getPhone, put = setPhone)) string Phone;

    string getFullName() const {
        return _FirstName + ' ' + _LastName;
    }

};
