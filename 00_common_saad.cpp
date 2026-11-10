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
