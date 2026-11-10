#include <fstream>
#include <sstream>
#include <iomanip>

namespace {

vector<string> sto_split(const string& line) {
    vector<string> fields;
    stringstream ss(line);
    string field;
    while (getline(ss, field, '|')) fields.push_back(field);
    if (!line.empty() && line.back() == '|') fields.push_back("");
    return fields;
}

bool sto_skip(const string& line) {
    return line.empty() || line[0] == '#';
}

double sto_to_double(const string& s, double fallback = 0.0) {
    try { return stod(s); } catch (...) { return fallback; }
}

int sto_to_int(const string& s, int fallback = 0) {
    try { return stoi(s); } catch (...) { return fallback; }
}

}

class FileStorage {
public:
    bool saveAccounts(const vector<Account*>& list, const string& path) {
        ofstream out(path);
        if (!out) return false;
        out << setprecision(17);
        out << "# type|account_no|owner_id|balance|hawl_start_day|overdraft_limit\n";
        for (const Account* acc : list) {
            if (!acc) continue;
            const CurrentAccount* cur = dynamic_cast<const CurrentAccount*>(acc);
            out << (cur ? 'C' : 'S') << '|'
                << acc->getAccountNumber() << '|'
                << acc->getOwnerId() << '|'
                << acc->getBalance() << '|'
                << acc->getHawlStartDay() << '|'
                << (cur ? cur->getOverdraftLimit() : 0.0) << '\n';
        }
        cout << "[DEBUG] saveAccounts wrote " << list.size() << " accounts to " << path << endl;
        return out.good();
    }

    vector<Account*> loadAccounts(const string& path) {
        vector<Account*> result;
        ifstream in(path);
        if (!in) return result;
        string line;
        while (getline(in, line)) {
            if (sto_skip(line)) continue;
            vector<string> f = sto_split(line);
            if (f.size() < 6) continue;
            double balance = sto_to_double(f[3]);
            int hawl_start_day = sto_to_int(f[4]);
            if (f[0] == "C")
                result.push_back(new CurrentAccount(f[1], f[2], balance, hawl_start_day, sto_to_double(f[5])));
            else if (f[0] == "S")
                result.push_back(new SavingsAccount(f[1], f[2], balance, hawl_start_day));
        }
        return result;
    }

    bool saveCustomers(const vector<Customer>& list, const string& path) {
        ofstream out(path);
        if (!out) return false;
        out << "# id|name|cnic|phone|address\n";
        for (const Customer& c : list)
            out << c.getId() << '|' << c.getName() << '|' << c.getCnic() << '|'
                << c.getPhone() << '|' << c.getAddress() << '\n';
        return out.good();
    }

    vector<Customer> loadCustomers(const string& path) {
        vector<Customer> result;
        ifstream in(path);
        if (!in) return result;
        string line;
        while (getline(in, line)) {
            if (sto_skip(line)) continue;
            vector<string> f = sto_split(line);
            if (f.size() < 5) continue;
            result.push_back(Customer(f[0], f[1], f[2], f[3], f[4]));
        }
        return result;
    }

    bool saveTransactions(const vector<Transaction>& list, const string& path) {
        ofstream out(path);
        if (!out) return false;
        out << setprecision(17);
        out << "# id|type|amount|date|source|destination\n";
        for (const Transaction& t : list)
            out << t.getId() << '|' << t.getType() << '|' << t.getAmount() << '|'
                << t.getDate() << '|' << t.getSource() << '|' << t.getDestination() << '\n';
        return out.good();
    }

    vector<Transaction> loadTransactions(const string& path) {
        vector<Transaction> result;
        ifstream in(path);
        if (!in) return result;
        string line;
        while (getline(in, line)) {
            if (sto_skip(line)) continue;
            vector<string> f = sto_split(line);
            if (f.size() < 6) continue;
            result.push_back(Transaction(f[0], f[1], sto_to_double(f[2]), f[3], f[4], f[5]));
        }
        return result;
    }

    bool saveZakatSettings(double silverPricePerGram, double goldPricePerGram, bool useSilver, const string& path) {
        ofstream out(path);
        if (!out) return false;
        out << setprecision(17);
        out << "# silver_price_per_gram|gold_price_per_gram|use_silver\n";
        out << silverPricePerGram << '|' << goldPricePerGram << '|' << (useSilver ? 1 : 0) << '\n';
        return out.good();
    }

    bool loadZakatSettings(double& silverPricePerGram, double& goldPricePerGram, bool& useSilver, const string& path) {
        ifstream in(path);
        if (!in) return false;
        string line;
        while (getline(in, line)) {
            if (sto_skip(line)) continue;
            vector<string> f = sto_split(line);
            if (f.size() < 3) return false;
            silverPricePerGram = sto_to_double(f[0]);
            goldPricePerGram = sto_to_double(f[1]);
            useSilver = sto_to_int(f[2]) != 0;
            return true;
        }
        return false;
    }
};

inline void sto_free_accounts(vector<Account*>& list) {
    for (Account* acc : list) delete acc;
    list.clear();
}
