// Owner: Syed Faizan Haider
// Let's just get this working for now.
bool saveCustomers(const CustomerManager& customers) {
    ofstream file(CUSTOMER_FILE);
    if (!file) {
        return false;
    }

    file << customers.size() << endl;
    for (int index = 0; index < customers.size(); index++) {
        Customer customer = customers.getCustomer(index);
        char name[NAME_SIZE];
        char cnic[CNIC_SIZE];
        char phone[PHONE_SIZE];
        char address[ADDRESS_SIZE];
        customer.getName(name, NAME_SIZE);
        customer.getCnic(cnic, CNIC_SIZE);
        customer.getPhone(phone, PHONE_SIZE);
        customer.getAddress(address, ADDRESS_SIZE);

        file << customer.getId() << endl;
        file << name << endl;
        file << cnic << endl;
        file << phone << endl;
        file << address << endl;
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
    file << accounts.size() << endl;
    for (int index = 0; index < accounts.size(); index++) {
        const Account& account = accounts.accountAt(index);
        char type[TYPE_SIZE];
        account.getType(type, TYPE_SIZE);

        file << account.getAccountNumber() << " " << account.getOwnerId() << " " << type << " "
             << account.getBalance() << " " << account.getOverdraftLimit() << " "
             << account.getHawlStartDay() << endl;
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

bool saveTransactions(const TransactionLog& log) {
    ofstream file(TRANSACTION_FILE);
    if (!file) {
        return false;
    }

    file.precision(15);
    file << log.size() << endl;
    for (int index = 0; index < log.size(); index++) {
        Transaction item = log.getItem(index);
        char type[TYPE_SIZE];
        item.getType(type, TYPE_SIZE);

        file << item.getId() << " " << type << " " << item.getAmount() << " " << item.getDay() << " "
             << item.getSourceAccount() << " " << item.getDestinationAccount() << endl;
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

    log.setNextId(highestId + 1);
    return true;
}

bool saveUsers(const AuthManager& auth) {
    ofstream file(USER_FILE);
    if (!file) {
        return false;
    }

    file << auth.size() << endl;
    for (int index = 0; index < auth.size(); index++) {
        const User& user = auth.userAt(index);
        char name[USERNAME_SIZE];
        char role[ROLE_SIZE];
        user.getUsername(name, USERNAME_SIZE);
        user.getRole(role, ROLE_SIZE);

        file << name << " " << user.getPasswordHash() << " " << role << endl;
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
