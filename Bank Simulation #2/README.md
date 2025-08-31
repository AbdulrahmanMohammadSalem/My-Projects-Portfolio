# Bank Simulation #2
## License — Personal Use Only
- *This project is © 2025 Abdulrahman Mohammad Salem and is licensed under the [SALEM PERSONAL-USE LICENSE (SPUL) v1.0](https://github.com/AbdulrahmanMohammadSalem/My-Projects-Portfolio/blob/main/LICENSE).*
- *You may view and use the code for private, non-commercial projects only. You may NOT fork, modify, redistribute, or claim ownership.*
- *For permission beyond personal use contact: abdulrahman.mohammad.salem@gmail.com.*

## Project Structure
The project consists of the following categories of header files:

### 1. Core Entities
These define the main objects in the system.

- **BankClient.h**  
    - Represents a bank client with details such as account number, balance, PIN code, etc.  
    - Includes methods for performing transactions (deposit, withdraw, transfer) and secure persistence of client data.

- **SystemUser.h**  
    - Represents a bank employee (system user).  
    - Handles authentication, authorization, and permission control.

- **Currency.h**  
    - Represents currency information and exchange rates.  
    - Works with the currency exchange module.

- **Constants.h**  
    - Contains global constants used across the system (e.g., file paths, permissions, formatting).

### 2. Screens (UI Modules)
Each "screen" is a dedicated user interface for a specific feature of the system.

- **CurrencyExchangeScreen.h**  
    - Displays currency rates, allows searching and browsing currencies.

- **CurrencyCalculatorScreen.h**  
    - Provides conversion functionality between currencies based on exchange rates.

- **AddNewClientScreen.h**  
    - Screen for creating and saving new client records into the system.

- *(Other screens exist for listing, updating, and managing clients and users.)*

### 3. Data & Persistence
Files for storing and retrieving system data.

- **Currencies.txt**  
    - Contains the list of available currencies and their exchange rates.  
    - Read and written by `Currency.h` and currency-related screens.

- **Clients.txt / Users.txt** (not shown in preview)  
    - Store serialized client and user records.

- **Logs.txt**  
    - Records login attempts and important system events for auditing.

### 4. Security
Handles protection of sensitive information.

- **Password encryption (inside SystemUser.h & utilities)**  
    - Ensures user and client passwords are not stored in plain text.  
    - Uses simple encryption/decryption functions when writing/reading from files.

- **Permissions management**
    - Each user has an assigned set of permissions (add clients, manage users, view logs, currency exchange, etc.), enforced throughout the system.

---

## *For more information about this project, please visit [its respective page](https://abdulrahmanmohammadsalem.github.io/CppConsoleApps/Bank-Simulation-1/) on [my website](https://abdulrahmanmohammadsalem.github.io).*

