// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

#pragma once

#include "Readers.h"

class ConsoleReader {
public:
    template <typename Number>
    Number readNumber(string msg, const string errMsg = "") const {
        return readers::readNumber<Number>(msg, errMsg);
    }

    template <typename Number>
    Number readNumber(string msg, const Number from, const Number to, const string errMsg = "") const {
        return readers::readNumber<Number>(msg, from, to, errMsg);
    }

    template <typename Number>
    Number readPositiveNumber(string msg, const string errMsg = "") const {
        return readers::readPositiveNumber<Number>(msg, errMsg);
    }

    template <typename Number>
    Number readNegativeNumber(string msg, const string errMsg = "") const {
        return readers::readNegativeNumber<Number>(msg, errMsg);
    }

    template <typename Number>
    Number readNonPositveNumber(string msg, const string errMsg = "") const {
        return readers::readNonPositveNumber<Number>(msg, errMsg);
    }

    template <typename Number>
    Number readNonNegativeNumber(string msg, const string errMsg = "") const {
        return readers::readNonNegativeNumber<Number>(msg, errMsg);
    }

    template <typename Number>
    Number readNonZeroNumber(string msg, const string errMsg = "") const {
        return readers::readNonZeroNumber<Number>(msg, errMsg);
    }

    string readWord(string msg, const string errMsg = "") const {
        return readers::readWord(msg, errMsg);
    }

    string readString(const string msg, const bool isAfterCin = false) const {
        return readers::readString(msg, isAfterCin);
    }

    char readCharacter(string msg, const string errMsg = "") const {
        return readers::readCharacter(msg, errMsg);
    }

    bool readBoolean(string msg, const string errMsg = "") const {
        return readers::readBoolean(msg, errMsg);
    }
};

