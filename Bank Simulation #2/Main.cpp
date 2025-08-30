// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#include "LoginScreen.h"
#include "Constants.h"

const std::string ENCRYPTION_KEY = "ABCDEF";

int main() {
	SystemUser::prepareUsersFile();

	while (true)
		if (!LoginScreen::displayLoginScreen())
			break;

	return 0;
}

