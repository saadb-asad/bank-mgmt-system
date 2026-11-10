// Owner: Saad Bin Asad (Group Leader)
// Let's just get this working for now.
void printMenu() {
    cout << endl;
    cout << "1. Create customer" << endl;
    cout << "2. Open account" << endl;
    cout << "3. Deposit" << endl;
    cout << "4. Withdraw" << endl;
    cout << "5. Transfer" << endl;
    cout << "6. View transaction history" << endl;
    cout << "7. Calculate zakat (admin)" << endl;
    cout << "8. Change nisab prices (admin)" << endl;
    cout << "9. Add user (admin)" << endl;
    cout << "10. Save and exit" << endl;
    cout << "Choice: ";
}

bool allowed(const AuthManager& auth, const User& user, const char action[]) {
    if (auth.hasPermission(user, action)) {
        return true;
    }
    cout << "You do not have permission for this" << endl;
    return false;
}

void createCustomerMenu(CustomerManager& customers) {
    char name[NAME_SIZE];
    char cnic[CNIC_SIZE];
    char phone[PHONE_SIZE];
    char address[ADDRESS_SIZE];

    cout << "Customer id: ";
    int id = readInt();
    cout << "Name: ";
    readText(name, NAME_SIZE);
    cout << "CNIC (12345-1234567-1): ";
    readText(cnic, CNIC_SIZE);
    cout << "Phone: ";
    readText(phone, PHONE_SIZE);
    cout << "Address: ";
    readText(address, ADDRESS_SIZE);

    customers.addCustomer(Customer(id, name, cnic, phone, address));
}

void openAccountMenu(AccountManager& accounts, const CustomerManager& customers, int today) {
    cout << "Account number: ";
    int number = readInt();
    cout << "Owner customer id: ";
    int ownerId = readInt();

    if (customers.findById(ownerId) == -1) {
        cout << "No customer with that id" << endl;
        return;
    }

    cout << "Type (1 = savings, 2 = current): ";
    int typeChoice = readInt();
    cout << "Opening balance: ";
    double startBalance = readDouble();

    double limit = 0;
    const char* type = TYPE_SAVINGS;
    if (typeChoice == 2) {
        type = TYPE_CURRENT;
        cout << "Overdraft limit: ";
        limit = readDouble();
    }

    int startDay = 0;
    if (startBalance > 0) {
        startDay = today;
    }

    accounts.addAccount(Account(number, ownerId, type, startBalance, limit, startDay));
}

void depositMenu(AccountManager& accounts, TransactionLog& log, int today) {
    cout << "Account number: ";
    int index = accounts.findByNumber(readInt());
    if (index == -1) {
        cout << "Account not found" << endl;
        return;
    }
    cout << "Amount: ";
    double amount = readDouble();
    deposit(accounts.accountAt(index), amount, log, today);
}

void withdrawMenu(AccountManager& accounts, TransactionLog& log, int today) {
    cout << "Account number: ";
    int index = accounts.findByNumber(readInt());
    if (index == -1) {
        cout << "Account not found" << endl;
        return;
    }
    cout << "Amount: ";
    double amount = readDouble();
    withdraw(accounts.accountAt(index), amount, log, today);
}

void transferMenu(AccountManager& accounts, TransactionLog& log, int today) {
    cout << "From account number: ";
    int fromIndex = accounts.findByNumber(readInt());
    cout << "To account number: ";
    int toIndex = accounts.findByNumber(readInt());

    if (fromIndex == -1 || toIndex == -1) {
        cout << "One of the accounts was not found" << endl;
        return;
    }
    cout << "Amount: ";
    double amount = readDouble();
    transfer(accounts.accountAt(fromIndex), accounts.accountAt(toIndex), amount, log, today);
}

void historyMenu(const TransactionLog& log) {
    cout << "Account number: ";
    log.printHistory(readInt());
}

void zakatMenu(const AccountManager& accounts, const ZakatSettings& settings, int today) {
    double nisab = nisabValue(settings.silverPrice, settings.goldPrice, settings.useSilver);
    cout << "Nisab threshold: " << nisab << endl;

    double totalDue = 0;
    for (int index = 0; index < accounts.size(); index++) {
        const Account& account = accounts.accountAt(index);

        if (account.isZakatEligible()) {
            if (hawlComplete(account.getHawlStartDay(), today)) {
                ZakatResult result = calculateZakat(account.getBalance(), 0, nisab);
                if (result.due) {
                    cout << "Account " << account.getAccountNumber() << " zakat due: " << result.amount << endl;
                    totalDue = totalDue + result.amount;
                } else {
                    cout << "Account " << account.getAccountNumber() << " is below nisab" << endl;
                }
            } else {
                cout << "Account " << account.getAccountNumber() << " has not completed its hawl" << endl;
            }
        }
    }
    cout << "Total zakat due: " << totalDue << endl;
}

void changeNisabMenu(ZakatSettings& settings) {
    cout << "Silver price per gram: ";
    settings.silverPrice = readDouble();
    cout << "Gold price per gram: ";
    settings.goldPrice = readDouble();
    cout << "Use silver nisab? (1 = silver, 0 = gold): ";
    settings.useSilver = (readInt() == 1);
}

void addUserMenu(AuthManager& auth) {
    char name[USERNAME_SIZE];
    char password[PASSWORD_SIZE];

    cout << "New username: ";
    readText(name, USERNAME_SIZE);
    cout << "Password: ";
    readText(password, PASSWORD_SIZE);
    cout << "Role (1 = admin, 2 = staff): ";
    int roleChoice = readInt();

    const char* role = ROLE_STAFF;
    if (roleChoice == 1) {
        role = ROLE_ADMIN;
    }
    auth.addUser(User(name, hashText(password), role));
}

int main() {
    CustomerManager customers;
    AccountManager accounts;
    TransactionLog log;
    AuthManager auth;
    ZakatSettings settings;
    settings.silverPrice = 0;
    settings.goldPrice = 0;
    settings.useSilver = true;

    loadCustomers(customers);
    loadAccounts(accounts);
    loadTransactions(log);
    loadUsers(auth);
    loadZakatSettings(settings);

    if (auth.size() == 0) {
        cout << "No users found, creating default admin (admin / admin123)" << endl;
        auth.addUser(User("admin", hashText("admin123"), ROLE_ADMIN));
    }

    int loggedIndex = -1;
    for (int attempt = 0; attempt < MAX_LOGIN_TRIES; attempt++) {
        char username[USERNAME_SIZE];
        char password[PASSWORD_SIZE];
        cout << "Username: ";
        readText(username, USERNAME_SIZE);
        cout << "Password: ";
        readText(password, PASSWORD_SIZE);

        if (auth.login(username, password)) {
            loggedIndex = auth.findUser(username);
            break;
        }
        cout << "Wrong username or password" << endl;
    }

    if (loggedIndex == -1) {
        cout << "Too many failed attempts, exiting" << endl;
        return 0;
    }

    cout << "Enter today's date" << endl;
    int today = readDate();

    for (;;) {
        printMenu();
        int choice = readInt();
        const User& currentUser = auth.userAt(loggedIndex);

        if (choice == 1 && allowed(auth, currentUser, "create")) {
            createCustomerMenu(customers);
        } else if (choice == 2 && allowed(auth, currentUser, "create")) {
            openAccountMenu(accounts, customers, today);
        } else if (choice == 3 && allowed(auth, currentUser, "deposit")) {
            depositMenu(accounts, log, today);
        } else if (choice == 4 && allowed(auth, currentUser, "withdraw")) {
            withdrawMenu(accounts, log, today);
        } else if (choice == 5 && allowed(auth, currentUser, "transfer")) {
            transferMenu(accounts, log, today);
        } else if (choice == 6 && allowed(auth, currentUser, "view")) {
            historyMenu(log);
        } else if (choice == 7 && canRunZakat(auth, currentUser)) {
            zakatMenu(accounts, settings, today);
        } else if (choice == 8 && canChangeNisab(auth, currentUser)) {
            changeNisabMenu(settings);
        } else if (choice == 9 && allowed(auth, currentUser, "manage_users")) {
            addUserMenu(auth);
        } else if (choice == 10) {
            saveCustomers(customers);
            saveAccounts(accounts);
            saveTransactions(log);
            saveUsers(auth);
            saveZakatSettings(settings);
            cout << "Data saved. Goodbye." << endl;
            break;
        } else {
            cout << "Invalid choice or not allowed" << endl;
        }
    }

    return 0;
}
