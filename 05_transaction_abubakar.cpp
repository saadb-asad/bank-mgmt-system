// Owner: Abubakar
// Let's just get this working for now.
class Transaction {
private:
    int id;
    char type[TYPE_SIZE];
    double amount;
    int day;
    int sourceAccount;
    int destinationAccount;

public:
    Transaction() {
        id = 0;
        type[0] = '\0';
        amount = 0;
        day = 0;
        sourceAccount = -1;
        destinationAccount = -1;
    }

    Transaction(int newId, const char newType[], double newAmount, int newDay, int newSource, int newDestination) {
        id = newId;
        copyText(type, newType, TYPE_SIZE);
        amount = newAmount;
        day = newDay;
        sourceAccount = newSource;
        destinationAccount = newDestination;
    }

    int getId() const {
        return id;
    }

    void getType(char output[], int maxSize) const {
        copyText(output, type, maxSize);
    }

    double getAmount() const {
        return amount;
    }

    int getDay() const {
        return day;
    }

    int getSourceAccount() const {
        return sourceAccount;
    }

    int getDestinationAccount() const {
        return destinationAccount;
    }
};

class TransactionLog {
private:
    Transaction items[MAX_TRANSACTIONS];
    int count;
    int nextId;

public:
    TransactionLog() {
        count = 0;
        nextId = 1;
    }

    bool isFull() const {
        if (count >= MAX_TRANSACTIONS) {
            return true;
        }
        return false;
    }

    int size() const {
        return count;
    }

    int nextTransactionId() {
        int newId = nextId;
        nextId++;
        return newId;
    }

    void setNextId(int value) {
        nextId = value;
    }

    bool record(Transaction newItem) {
        if (isFull()) {
            cout << "Transaction log is full" << endl;
            return false;
        }
        items[count] = newItem;
        count++;
        return true;
    }

    Transaction getItem(int index) const {
        return items[index];
    }

    void printHistory(int accountNumber) const {
        bool found = false;
        for (int index = 0; index < count; index++) {
            if (items[index].getSourceAccount() == accountNumber || items[index].getDestinationAccount() == accountNumber) {
                char kind[TYPE_SIZE];
                items[index].getType(kind, TYPE_SIZE);
                cout << "#" << items[index].getId() << " " << kind << " " << items[index].getAmount()
                     << " on day " << items[index].getDay() << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No transactions for this account" << endl;
        }
    }
};

bool deposit(Account& account, double amount, TransactionLog& log, int today) {
    if (log.isFull() || amount <= 0) {
        return false;
    }
    account.deposit(amount);
    Transaction item(log.nextTransactionId(), "deposit", amount, today, -1, account.getAccountNumber());
    log.record(item);
    return true;
}

bool withdraw(Account& account, double amount, TransactionLog& log, int today) {
    if (log.isFull()) {
        return false;
    }
    if (!account.withdraw(amount)) {
        return false;
    }
    Transaction item(log.nextTransactionId(), "withdraw", amount, today, account.getAccountNumber(), -1);
    log.record(item);
    return true;
}

bool transfer(Account& from, Account& to, double amount, TransactionLog& log, int today) {
    if (log.isFull()) {
        return false;
    }
    if (!from.withdraw(amount)) {
        return false;
    }
    to.deposit(amount);
    Transaction item(log.nextTransactionId(), "transfer", amount, today, from.getAccountNumber(), to.getAccountNumber());
    log.record(item);
    return true;
}
