// Owner: Saad Bin Asad (Group Leader)
// Let's just get this working for now.
#include <iostream>
#include <fstream>
using namespace std;

const int MAX_CUSTOMERS = 100;
const int MAX_ACCOUNTS = 100;
const int MAX_TRANSACTIONS = 500;
const int MAX_USERS = 20;

const int NAME_SIZE = 50;
const int CNIC_SIZE = 16;
const int PHONE_SIZE = 20;
const int ADDRESS_SIZE = 60;
const int CNIC_LENGTH = 15;
const int CNIC_FIRST_DASH = 5;
const int CNIC_SECOND_DASH = 13;

const int TYPE_SIZE = 12;
const int USERNAME_SIZE = 30;
const int PASSWORD_SIZE = 30;
const int ROLE_SIZE = 10;
const int ACTION_SIZE = 20;

const int HAWL_DAYS = 354;
const double ZAKAT_RATE = 0.025;
const double NISAB_SILVER_GRAMS = 612.36;
const double NISAB_GOLD_GRAMS = 87.48;
const int MAX_LOGIN_TRIES = 3;
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

int lengthOfText(const char text[]) {
    int length = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        length++;
    }
    return length;
}

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

bool isDigitCharacter(char symbol) {
    if (symbol >= '0' && symbol <= '9') {
        return true;
    }
    return false;
}

bool isDashPosition(int position) {
    if (position == CNIC_FIRST_DASH || position == CNIC_SECOND_DASH) {
        return true;
    }
    return false;
}

bool isValidCnicFormat(const char cnic[]) {
    if (lengthOfText(cnic) != CNIC_LENGTH) {
        return false;
    }

    for (int position = 0; position < CNIC_LENGTH; position++) {
        if (isDashPosition(position)) {
            if (cnic[position] != '-') {
                return false;
            }
        } else {
            if (!isDigitCharacter(cnic[position])) {
                return false;
            }
        }
    }

    return true;
}

int hashText(const char text[]) {
    long long hash = 7;
    for (int i = 0; text[i] != '\0'; i++) {
        hash = (hash * 31 + text[i]) % 1000000007LL;
    }
    return (int)hash;
}

void readText(char output[], int maxSize) {
    cin.getline(output, maxSize);
}

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
