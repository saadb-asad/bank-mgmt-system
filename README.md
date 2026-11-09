# Bank Management System (C++)

A console-based bank management system written in C++17 as a single file. Staff log in to manage customers and accounts, record deposits, withdrawals, and transfers, view transaction history, and calculate zakat on eligible balances.

## Features

- **Customers:** add, find, and remove customers. CNIC format is checked on entry.
- **Accounts:** savings accounts (with interest) and current accounts (with an overdraft limit).
- **Transactions:** deposit, withdraw, and transfer, with a full history per account.
- **Login and permissions:** users have an `admin` or `staff` role. Only permitted roles can run zakat or change the nisab price.
- **File storage:** accounts, customers, transactions, and zakat settings are saved to text files and loaded on startup.
- **Zakat:** calculates 2.5% of zakatable wealth above the nisab threshold, and checks that the balance has been held for one lunar year (hawl).

## Project structure

    bank_system.cpp    The whole program, split into clearly marked module sections
    README.md          This file

The code is in one file on purpose. Each module has a banner section:

| Section | Purpose |
|---|---|
| Account | Base account, savings and current types |
| Customer | Customer details, CNIC validation, customer manager |
| Transaction | Transaction records, log, deposit, withdraw, transfer |
| Authentication | Users, login, password hashing, permissions |
| Storage | Saving and loading all data |
| Zakat | Nisab check, hawl check, 2.5% calculation |
| Main | Login loop, menu, module integration |

## Requirements

- A C++17 compiler, such as `g++` 7 or later, or `clang++`
- No third-party libraries

## Build and run

    g++ -std=c++17 -Wall bank_system.cpp -o bank_system
    ./bank_system

On Windows with MinGW:

    g++ -std=c++17 -Wall bank_system.cpp -o bank_system.exe
    bank_system.exe

## Usage

1. Log in with a valid username and password. You get three attempts.
2. Choose an option from the menu:
   - Create a customer
   - Open a savings or current account
   - Deposit, withdraw, or transfer
   - View transaction history
   - Calculate zakat (admin, or staff with zakat permission)
   - Change the nisab price (admin only)
   - Save and exit

Data is saved to the data files when you choose "Save and exit".

## Zakat rules used

- Zakat rate: 2.5% of zakatable wealth.
- Nisab: 612.36 g of silver or 87.48 g of gold, valued at the price you enter. Silver is the default.
- Hawl: a balance must be held for one lunar year before zakat is due.

Some rules are still being decided, so they may change:

- Which account types are zakatable (current accounts are open)
- Whether debts are tracked
- Whether zakat is deducted automatically or only shown
- The lunar year length used for the hawl check

## Team

| Member | Area | Contribution
|---|---|---|
| [Saad Bin Asad](https://github.com/saadb-asad) (Team Lead) | Integration and main menu |
| [Mauz Ahmed Ghumman](https://github.com/mauzghumman-lab) | Base Account class |
| [Usman Nadeem](https://github.com/Usmannadeemm) | Savings and current accounts |
| [Usama Ahsan](https://github.com/usamajutt05555-sketch) | Customer class and CNIC validation |
| [Hassan Abdullah](https://github.com/hassanasif78223-ux) | Customer manager |
| Muhammad Abubakar | Transactions and money functions |
| [Abdulrehman](https://github.com/ARawan007) | User class, login, password hashing |
| [Muhammad Faizan](https://github.com/Faizan-lab856) | User store and permissions |
| [Syed Faizan Haider](https://github.com/haiderig1214-eng) | File storage |
| [Farhan Abbass](https://github.com/farhan4t6) | Zakat calculation |
| Sameer Baig | Hawl tracking and zakat access |


## Contributing

1. Pull the latest version of `bank_system.cpp`.
2. Edit only your own banner section.
3. Compile the whole file before pushing.

## Notes

This is a student project. Passwords are hashed with a simple method that is not cryptographically secure, and the data files are plain text. **Do not use it to store real customer data**
