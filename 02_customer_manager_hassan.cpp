// Owner: Hassan Abdullah
// Let's just get this working for now.
class CustomerManager {
private:
    Customer customers[MAX_CUSTOMERS];
    int count;

public:
    CustomerManager() {
        count = 0;
    }

    bool isFull() const {
        if (count >= MAX_CUSTOMERS) {
            return true;
        }
        return false;
    }

    bool addCustomer(Customer newCustomer) {
        if (isFull()) {
            cout << "Customer list is full" << endl;
            return false;
        }

        if (findById(newCustomer.getId()) != -1) {
            cout << "A customer with this id already exists" << endl;
            return false;
        }

        customers[count] = newCustomer;
        count++;
        return true;
    }

    int findById(int wantedId) const {
        for (int index = 0; index < count; index++) {
            if (customers[index].getId() == wantedId) {
                return index;
            }
        }
        return -1;
    }

    int findByName(const char wantedName[]) const {
        for (int index = 0; index < count; index++) {
            if (customers[index].hasName(wantedName)) {
                return index;
            }
        }
        return -1;
    }

    Customer getCustomer(int index) const {
        return customers[index];
    }

    bool removeCustomer(int wantedId) {
        int foundIndex = findById(wantedId);
        if (foundIndex == -1) {
            return false;
        }

        for (int index = foundIndex; index < count - 1; index++) {
            customers[index] = customers[index + 1];
        }
        count--;
        return true;
    }

    int size() const {
        return count;
    }
};
