// Owner: Usama Ahsan
// Let's just get this working for now.
class Customer {
private:
    int id;
    char name[NAME_SIZE];
    char cnic[CNIC_SIZE];
    char phone[PHONE_SIZE];
    char address[ADDRESS_SIZE];

public:
    Customer() {
        id = 0;
        name[0] = '\0';
        cnic[0] = '\0';
        phone[0] = '\0';
        address[0] = '\0';
    }

    Customer(int newId, const char newName[], const char newCnic[], const char newPhone[], const char newAddress[]) {
        id = newId;
        name[0] = '\0';
        cnic[0] = '\0';
        phone[0] = '\0';
        address[0] = '\0';
        setDetails(newName, newCnic, newPhone, newAddress);
    }

    void setDetails(const char newName[], const char newCnic[], const char newPhone[], const char newAddress[]) {
        copyText(name, newName, NAME_SIZE);
        copyText(phone, newPhone, PHONE_SIZE);
        copyText(address, newAddress, ADDRESS_SIZE);

        if (validateCNIC(newCnic)) {
            copyText(cnic, newCnic, CNIC_SIZE);
        } else {
            cout << "Invalid CNIC, keeping the old one" << endl;
        }
    }

    static bool validateCNIC(const char checkCnic[]) {
        return isValidCnicFormat(checkCnic);
    }

    int getId() const {
        return id;
    }

    void getName(char output[], int maxSize) const {
        copyText(output, name, maxSize);
    }

    void getCnic(char output[], int maxSize) const {
        copyText(output, cnic, maxSize);
    }

    void getPhone(char output[], int maxSize) const {
        copyText(output, phone, maxSize);
    }

    void getAddress(char output[], int maxSize) const {
        copyText(output, address, maxSize);
    }

    bool hasName(const char wantedName[]) const {
        return textsMatch(name, wantedName);
    }
};
