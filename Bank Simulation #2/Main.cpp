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
