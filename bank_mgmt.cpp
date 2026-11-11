// Owner: Saad Bin Asad (Group Leader)
// Let's just get this working for now.
#include <iostream>
#include <fstream>
using namespace std;

const int MAX_CUSTOMERS = 100;
const int MAX_ACCOUNTS = 100;
const int MAX_TRANSACTIONS = 500;
const int MAX_USERS = 20;
const int MAX_LOGIN_TRIES = 3;

const int NAME_SIZE = 50;
const int CNIC_SIZE = 16;
const int PHONE_SIZE = 20;
const int ADDRESS_SIZE = 60;
const int TYPE_SIZE = 12;
const int USERNAME_SIZE = 30;
const int PASSWORD_SIZE = 30;
const int ROLE_SIZE = 10;

const int CNIC_LENGTH = 15;
const int CNIC_FIRST_DASH = 5;
const int CNIC_SECOND_DASH = 13;

const int HAWL_DAYS = 354;
const double ZAKAT_RATE = 0.025;
const double NISAB_SILVER_GRAMS = 612.36;
const double NISAB_GOLD_GRAMS = 87.48;
const int MONTH_TOTAL_DAYS[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

const char TYPE_SAVINGS[] = "savings";
const char TYPE_CURRENT[] = "current";
const char ROLE_ADMIN[] = "admin";
const char ROLE_STAFF[] = "staff";
const char CUSTOMER_FILE[] = "customers.txt";
const char ACCOUNT_FILE[] = "accounts.txt";
const char TRANSACTION_FILE[] = "transactions.txt";
const char USER_FILE[] = "users.txt";
const char ZAKAT_FILE[] = "zakat.txt";

// Owner: Saad Bin Asad (Group Leader)
// Let's just get this working for now.
void copyText(char destination[], const char source[], int maxSize) {
    int index = 0;
    for (index = 0; index < maxSize - 1 && source[index] != '\0'; index++) {
        destination[index] = source[index];
    }
    destination[index] = '\0';
}

bool textsMatch(const char first[], const char second[]) {
    int index = 0;
    for (index = 0; first[index] != '\0' && second[index] != '\0'; index++) {
        if (first[index] != second[index]) {
            return false;
        }
    }
    return first[index] == second[index];
}

bool isValidCnicFormat(const char cnic[]) {
    for (int position = 0; position < CNIC_LENGTH; position++) {
        bool dashExpected = (position == CNIC_FIRST_DASH || position == CNIC_SECOND_DASH);
        if (dashExpected) {
            if (cnic[position] != '-') {
                return false;
            }
        } else if (cnic[position] < '0' || cnic[position] > '9') {
            return false;
        }
    }
    return cnic[CNIC_LENGTH] == '\0';
}

int hashText(const char text[]) {
    long long hash = 7;
    for (int i = 0; text[i] != '\0'; i++) {
        hash = (hash * 31 + text[i]) % 1000000007LL;
    }
    return (int)hash;
}

// Owner: Saad Bin Asad (Group Leader)
// Let's just get this working for now.
int readInt() {
    int value = 0;
    cin >> value;
    cin.ignore(1000, '\n');
    return value;
}

double readDouble() {
    double value = 0;
    cin >> value;
    cin.ignore(1000, '\n');
    return value;
}

int dayNumber(int year, int month, int day) {
    return year * 365 + year / 4 + MONTH_TOTAL_DAYS[month] + day;
}

int readDate() {
    for (;;) {
        cout << "Year: ";
        int year = readInt();
        cout << "Month (1-12): ";
        int month = readInt();
        cout << "Day (1-31): ";
        int day = readInt();

        if (month >= 1 && month <= 12 && day >= 1 && day <= 31) {
            return dayNumber(year, month, day);
        }
        cout << "Invalid date, try again" << endl;
    }
}

// Owner: Usama Ahsan
// Let's just get this working for now.
class Customer {
public:
    int id;
    char name[NAME_SIZE];
    char cnic[CNIC_SIZE];
    char phone[PHONE_SIZE];
    char address[ADDRESS_SIZE];

    Customer(int newId = 0, const char newName[] = "", const char newCnic[] = "", const char newPhone[] = "", const char newAddress[] = "") {
        id = newId;
        copyText(name, newName, NAME_SIZE);
        copyText(cnic, newCnic, CNIC_SIZE);
        copyText(phone, newPhone, PHONE_SIZE);
        copyText(address, newAddress, ADDRESS_SIZE);

        if (newCnic[0] != '\0' && !isValidCnicFormat(cnic)) {
            cout << "Warning: invalid CNIC for customer " << id << endl;
        }
    }
};

// Owner: Hassan Abdullah
// Let's just get this working for now.
class CustomerManager {
public:
    Customer list[MAX_CUSTOMERS];
    int count = 0;

    bool addCustomer(Customer newCustomer) {
        if (count >= MAX_CUSTOMERS) {
            cout << "Customer list is full" << endl;
            return false;
        }

        if (findById(newCustomer.id) != -1) {
            cout << "A customer with this id already exists" << endl;
            return false;
        }

        list[count] = newCustomer;
        count++;
        return true;
    }

    int findById(int wantedId) const {
        for (int index = 0; index < count; index++) {
            if (list[index].id == wantedId) {
                return index;
            }
        }
        return -1;
    }

    bool removeCustomer(int wantedId) {
        int foundIndex = findById(wantedId);
        if (foundIndex == -1) {
            return false;
        }

        for (int index = foundIndex; index < count - 1; index++) {
            list[index] = list[index + 1];
        }
        count--;
        return true;
    }
};

// Owner: Mauz Ahmed Ghumman
// Let's just get this working for now.
class Account {
public:
    int accountNumber;
    int ownerId;
    char accountType[TYPE_SIZE];
    double balance;
    double overdraftLimit;
    int hawlStartDay;

    Account(int newNumber = 0, int newOwnerId = 0, const char newType[] = "", double startBalance = 0, double newLimit = 0, int startDay = 0) {
        accountNumber = newNumber;
        ownerId = newOwnerId;
        copyText(accountType, newType, TYPE_SIZE);
        balance = startBalance;
        overdraftLimit = newLimit;
        hawlStartDay = startDay;
    }

    bool isZakatEligible() const {
        return textsMatch(accountType, TYPE_SAVINGS);
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Deposit must be more than zero" << endl;
            return;
        }
        balance = balance + amount;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Withdrawal must be more than zero" << endl;
            return false;
        }

        if (balance - amount < -overdraftLimit) {
            cout << "Not enough balance" << endl;
            return false;
        }

        balance = balance - amount;
        return true;
    }
};

// Owner: Usman Nadeem
// Let's just get this working for now.
class AccountManager {
public:
    Account list[MAX_ACCOUNTS];
    int count = 0;

    int findByNumber(int wantedNumber) const {
        for (int index = 0; index < count; index++) {
            if (list[index].accountNumber == wantedNumber) {
                return index;
            }
        }
        return -1;
    }

    bool addAccount(Account newAccount) {
        if (count >= MAX_ACCOUNTS) {
            cout << "Account list is full" << endl;
            return false;
        }

        if (findByNumber(newAccount.accountNumber) != -1) {
            cout << "An account with this number already exists" << endl;
            return false;
        }

        list[count] = newAccount;
        count++;
        return true;
    }
};

// Owner: Abubakar
// Let's just get this working for now.
class Transaction {
public:
    int id;
    char type[TYPE_SIZE];
    double amount;
    int day;
    int sourceAccount;
    int destinationAccount;

    Transaction(int newId = 0, const char newType[] = "", double newAmount = 0, int newDay = 0, int newSource = -1, int newDestination = -1) {
        id = newId;
        copyText(type, newType, TYPE_SIZE);
        amount = newAmount;
        day = newDay;
        sourceAccount = newSource;
        destinationAccount = newDestination;
    }
};

class TransactionLog {
public:
    Transaction list[MAX_TRANSACTIONS];
    int count = 0;
    int nextId = 1;

    bool isFull() const {
        return count >= MAX_TRANSACTIONS;
    }

    int nextTransactionId() {
        int newId = nextId;
        nextId++;
        return newId;
    }

    bool record(Transaction newItem) {
        if (isFull()) {
            cout << "Transaction log is full" << endl;
            return false;
        }
        list[count] = newItem;
        count++;
        return true;
    }

    void printHistory(int accountNumber) const {
        bool found = false;
        for (int index = 0; index < count; index++) {
            if (list[index].sourceAccount == accountNumber || list[index].destinationAccount == accountNumber) {
                cout << "#" << list[index].id << " " << list[index].type << " " << list[index].amount
                     << " on day " << list[index].day << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No transactions for this account" << endl;
        }
    }
};

// Owner: Abubakar
// Let's just get this working for now.
bool deposit(Account& account, double amount, TransactionLog& log, int today) {
    if (log.isFull() || amount <= 0) {
        return false;
    }
    account.deposit(amount);
    Transaction item(log.nextTransactionId(), "deposit", amount, today, -1, account.accountNumber);
    log.record(item);
    return true;
}

bool withdraw(Account& account, double amount, TransactionLog& log, int today) {
    if (log.isFull() || !account.withdraw(amount)) {
        return false;
    }
    Transaction item(log.nextTransactionId(), "withdraw", amount, today, account.accountNumber, -1);
    log.record(item);
    return true;
}

bool transfer(Account& from, Account& to, double amount, TransactionLog& log, int today) {
    if (log.isFull() || !from.withdraw(amount)) {
        return false;
    }
    to.deposit(amount);
    Transaction item(log.nextTransactionId(), "transfer", amount, today, from.accountNumber, to.accountNumber);
    log.record(item);
    return true;
}

// Owner: Abdulrehman
// Let's just get this working for now.
class User {
public:
    char username[USERNAME_SIZE];
    int passwordHash;
    char role[ROLE_SIZE];

    User(const char newUsername[] = "", int newHash = 0, const char newRole[] = "") {
        copyText(username, newUsername, USERNAME_SIZE);
        passwordHash = newHash;
        copyText(role, newRole, ROLE_SIZE);
    }

    bool isAdmin() const {
        return textsMatch(role, ROLE_ADMIN);
    }

    bool isStaff() const {
        return textsMatch(role, ROLE_STAFF);
    }
};

// Owner: Muhammad Faizan
// Let's just get this working for now.
bool isStaffAction(const char action[]) {
    return textsMatch(action, "view") || textsMatch(action, "create")
        || textsMatch(action, "deposit") || textsMatch(action, "withdraw")
        || textsMatch(action, "transfer");
}

class AuthManager {
public:
    User list[MAX_USERS];
    int count = 0;

    int findUser(const char wantedName[]) const {
        for (int index = 0; index < count; index++) {
            if (textsMatch(list[index].username, wantedName)) {
                return index;
            }
        }
        return -1;
    }

    bool addUser(User newUser) {
        if (count >= MAX_USERS) {
            cout << "User list is full" << endl;
            return false;
        }

        if (findUser(newUser.username) != -1) {
            cout << "That username already exists" << endl;
            return false;
        }

        list[count] = newUser;
        count++;
        return true;
    }

    bool login(const char wantedName[], const char password[]) const {
        int index = findUser(wantedName);
        if (index == -1) {
            return false;
        }
        return list[index].passwordHash == hashText(password);
    }

    bool hasPermission(const User& user, const char action[]) const {
        if (user.isAdmin()) {
            return true;
        }
        return user.isStaff() && isStaffAction(action);
    }
};

// Owner: Farhan Abbass
// Let's just get this working for now.
struct ZakatSettings {
    double silverPrice = 0;
    double goldPrice = 0;
    bool useSilver = true;
};

double nisabValue(double silverPrice, double goldPrice, bool useSilver) {
    if (useSilver) {
        return NISAB_SILVER_GRAMS * silverPrice;
    }
    return NISAB_GOLD_GRAMS * goldPrice;
}

double calculateZakat(double wealth, double nisab) {
    if (wealth < nisab) {
        return 0;
    }
    return wealth * ZAKAT_RATE;
}

// Owner: Sameer Baig
// Let's just get this working for now.
bool hawlComplete(int startDay, int todayDay) {
    if (startDay == 0) {
        return false;
    }
    return todayDay - startDay >= HAWL_DAYS;
}

bool canRunZakat(const AuthManager& auth, const User& user) {
    return auth.hasPermission(user, "zakat");
}

bool canChangeNisab(const AuthManager& auth, const User& user) {
    return auth.hasPermission(user, "change_nisab");
}

// Owner: Syed Faizan Haider
// Let's just get this working for now.
bool saveCustomers(const CustomerManager& customers) {
    ofstream file(CUSTOMER_FILE);
    if (!file) {
        return false;
    }

    file << customers.count << endl;
    for (int index = 0; index < customers.count; index++) {
        const Customer& customer = customers.list[index];
        file << customer.id << endl;
        file << customer.name << endl;
        file << customer.cnic << endl;
        file << customer.phone << endl;
        file << customer.address << endl;
    }
    return true;
}

bool loadCustomers(CustomerManager& customers) {
    ifstream file(CUSTOMER_FILE);
    if (!file) {
        return false;
    }

    int total = 0;
    file >> total;
    file.ignore(1000, '\n');

    for (int index = 0; index < total; index++) {
        int id = 0;
        char name[NAME_SIZE];
        char cnic[CNIC_SIZE];
        char phone[PHONE_SIZE];
        char address[ADDRESS_SIZE];

        file >> id;
        file.ignore(1000, '\n');
        file.getline(name, NAME_SIZE);
        file.getline(cnic, CNIC_SIZE);
        file.getline(phone, PHONE_SIZE);
        file.getline(address, ADDRESS_SIZE);

        customers.addCustomer(Customer(id, name, cnic, phone, address));
    }
    return true;
}

bool saveAccounts(const AccountManager& accounts) {
    ofstream file(ACCOUNT_FILE);
    if (!file) {
        return false;
    }

    file.precision(15);
    file << accounts.count << endl;
    for (int index = 0; index < accounts.count; index++) {
        const Account& account = accounts.list[index];
        file << account.accountNumber << " " << account.ownerId << " " << account.accountType << " "
             << account.balance << " " << account.overdraftLimit << " " << account.hawlStartDay << endl;
    }
    return true;
}

bool loadAccounts(AccountManager& accounts) {
    ifstream file(ACCOUNT_FILE);
    if (!file) {
        return false;
    }

    int total = 0;
    file >> total;

    for (int index = 0; index < total; index++) {
        int number = 0;
        int ownerId = 0;
        char type[TYPE_SIZE];
        double balance = 0;
        double limit = 0;
        int startDay = 0;

        file >> number >> ownerId >> type >> balance >> limit >> startDay;
        accounts.addAccount(Account(number, ownerId, type, balance, limit, startDay));
    }
    return true;
}

// Owner: Syed Faizan Haider
// Let's just get this working for now.
bool saveTransactions(const TransactionLog& log) {
    ofstream file(TRANSACTION_FILE);
    if (!file) {
        return false;
    }

    file.precision(15);
    file << log.count << endl;
    for (int index = 0; index < log.count; index++) {
        const Transaction& item = log.list[index];
        file << item.id << " " << item.type << " " << item.amount << " " << item.day << " "
             << item.sourceAccount << " " << item.destinationAccount << endl;
    }
    return true;
}

bool loadTransactions(TransactionLog& log) {
    ifstream file(TRANSACTION_FILE);
    if (!file) {
        return false;
    }

    int total = 0;
    int highestId = 0;
    file >> total;

    for (int index = 0; index < total; index++) {
        int id = 0;
        char type[TYPE_SIZE];
        double amount = 0;
        int day = 0;
        int source = 0;
        int destination = 0;

        file >> id >> type >> amount >> day >> source >> destination;
        log.record(Transaction(id, type, amount, day, source, destination));
        if (id > highestId) {
            highestId = id;
        }
    }

    log.nextId = highestId + 1;
    return true;
}

bool saveUsers(const AuthManager& auth) {
    ofstream file(USER_FILE);
    if (!file) {
        return false;
    }

    file << auth.count << endl;
    for (int index = 0; index < auth.count; index++) {
        const User& user = auth.list[index];
        file << user.username << " " << user.passwordHash << " " << user.role << endl;
    }
    return true;
}

bool loadUsers(AuthManager& auth) {
    ifstream file(USER_FILE);
    if (!file) {
        return false;
    }

    int total = 0;
    file >> total;

    for (int index = 0; index < total; index++) {
        char name[USERNAME_SIZE];
        int hash = 0;
        char role[ROLE_SIZE];

        file >> name >> hash >> role;
        auth.addUser(User(name, hash, role));
    }
    return true;
}

bool saveZakatSettings(const ZakatSettings& settings) {
    ofstream file(ZAKAT_FILE);
    if (!file) {
        return false;
    }

    file.precision(15);
    int silverFlag = 0;
    if (settings.useSilver) {
        silverFlag = 1;
    }
    file << settings.silverPrice << " " << settings.goldPrice << " " << silverFlag << endl;
    return true;
}

bool loadZakatSettings(ZakatSettings& settings) {
    ifstream file(ZAKAT_FILE);
    if (!file) {
        return false;
    }

    int silverFlag = 0;
    file >> settings.silverPrice >> settings.goldPrice >> silverFlag;
    settings.useSilver = (silverFlag == 1);
    return true;
}

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
    cin.getline(name, NAME_SIZE);
    cout << "CNIC (12345-1234567-1): ";
    cin.getline(cnic, CNIC_SIZE);
    cout << "Phone: ";
    cin.getline(phone, PHONE_SIZE);
    cout << "Address: ";
    cin.getline(address, ADDRESS_SIZE);

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
    deposit(accounts.list[index], amount, log, today);
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
    withdraw(accounts.list[index], amount, log, today);
}

// Owner: Saad Bin Asad (Group Leader)
// Let's just get this working for now.
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
    transfer(accounts.list[fromIndex], accounts.list[toIndex], amount, log, today);
}

void historyMenu(const TransactionLog& log) {
    cout << "Account number: ";
    log.printHistory(readInt());
}

void zakatMenu(const AccountManager& accounts, const ZakatSettings& settings, int today) {
    double nisab = nisabValue(settings.silverPrice, settings.goldPrice, settings.useSilver);
    cout << "Nisab threshold: " << nisab << endl;

    double totalDue = 0;
    for (int index = 0; index < accounts.count; index++) {
        const Account& account = accounts.list[index];
        if (!account.isZakatEligible()) {
            continue;
        }
        if (!hawlComplete(account.hawlStartDay, today)) {
            cout << "Account " << account.accountNumber << " has not completed its hawl" << endl;
            continue;
        }

        double due = calculateZakat(account.balance, nisab);
        if (due > 0) {
            cout << "Account " << account.accountNumber << " zakat due: " << due << endl;
            totalDue = totalDue + due;
        } else {
            cout << "Account " << account.accountNumber << " is below nisab" << endl;
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
    cin.getline(name, USERNAME_SIZE);
    cout << "Password: ";
    cin.getline(password, PASSWORD_SIZE);
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

    loadCustomers(customers);
    loadAccounts(accounts);
    loadTransactions(log);
    loadUsers(auth);
    loadZakatSettings(settings);

    if (auth.count == 0) {
        cout << "No users found, creating default admin (admin / admin123)" << endl;
        auth.addUser(User("admin", hashText("admin123"), ROLE_ADMIN));
    }

    int loggedIndex = -1;
    for (int attempt = 0; attempt < MAX_LOGIN_TRIES; attempt++) {
        char username[USERNAME_SIZE];
        char password[PASSWORD_SIZE];
        cout << "Username: ";
        cin.getline(username, USERNAME_SIZE);
        cout << "Password: ";
        cin.getline(password, PASSWORD_SIZE);

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
        const User& currentUser = auth.list[loggedIndex];

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

