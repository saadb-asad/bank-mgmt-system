// Owner: Mauz Ahmed Ghumman
// Let's just get this working for now.
class Account {
private:
    int accountNumber;
    int ownerId;
    char accountType[TYPE_SIZE];
    double balance;
    double overdraftLimit;
    int hawlStartDay;

public:
    Account() {
        accountNumber = 0;
        ownerId = 0;
        accountType[0] = '\0';
        balance = 0;
        overdraftLimit = 0;
        hawlStartDay = 0;
    }

    Account(int newNumber, int newOwnerId, const char newType[], double startBalance, double newLimit, int startDay) {
        accountNumber = newNumber;
        ownerId = newOwnerId;
        copyText(accountType, newType, TYPE_SIZE);
        balance = startBalance;
        overdraftLimit = newLimit;
        hawlStartDay = startDay;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    int getOwnerId() const {
        return ownerId;
    }

    double getBalance() const {
        return balance;
    }

    double getOverdraftLimit() const {
        return overdraftLimit;
    }

    int getHawlStartDay() const {
        return hawlStartDay;
    }

    void getType(char output[], int maxSize) const {
        copyText(output, accountType, maxSize);
    }

    bool isSavings() const {
        return textsMatch(accountType, TYPE_SAVINGS);
    }

    bool isZakatEligible() const {
        if (isSavings()) {
            return true;
        }
        return false;
    }

    void startHawl(int day) {
        hawlStartDay = day;
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

    void applyInterest(double rate) {
        if (!isSavings()) {
            cout << "Interest only applies to savings accounts" << endl;
            return;
        }

        if (rate < 0) {
            cout << "Interest rate cannot be negative" << endl;
            return;
        }

        balance = balance + balance * rate;
    }
};
