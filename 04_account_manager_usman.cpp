// Owner: Usman Nadeem
// Let's just get this working for now.
class AccountManager {
private:
    Account accounts[MAX_ACCOUNTS];
    int count;

public:
    AccountManager() {
        count = 0;
    }

    bool isFull() const {
        if (count >= MAX_ACCOUNTS) {
            return true;
        }
        return false;
    }

    int size() const {
        return count;
    }

    int findByNumber(int wantedNumber) const {
        for (int index = 0; index < count; index++) {
            if (accounts[index].getAccountNumber() == wantedNumber) {
                return index;
            }
        }
        return -1;
    }

    bool addAccount(Account newAccount) {
        if (isFull()) {
            cout << "Account list is full" << endl;
            return false;
        }

        if (findByNumber(newAccount.getAccountNumber()) != -1) {
            cout << "An account with this number already exists" << endl;
            return false;
        }

        accounts[count] = newAccount;
        count++;
        return true;
    }

    Account& accountAt(int index) {
        return accounts[index];
    }

    const Account& accountAt(int index) const {
        return accounts[index];
    }
};
