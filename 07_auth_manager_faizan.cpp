// Owner: Muhammad Faizan
// Let's just get this working for now.
bool isStaffAction(const char action[]) {
    if (textsMatch(action, "view")) {
        return true;
    }
    if (textsMatch(action, "create")) {
        return true;
    }
    if (textsMatch(action, "deposit")) {
        return true;
    }
    if (textsMatch(action, "withdraw")) {
        return true;
    }
    if (textsMatch(action, "transfer")) {
        return true;
    }
    return false;
}

class AuthManager {
private:
    User users[MAX_USERS];
    int count;

public:
    AuthManager() {
        count = 0;
    }

    int size() const {
        return count;
    }

    int findUser(const char wantedName[]) const {
        for (int index = 0; index < count; index++) {
            char name[USERNAME_SIZE];
            users[index].getUsername(name, USERNAME_SIZE);
            if (textsMatch(name, wantedName)) {
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

        char name[USERNAME_SIZE];
        newUser.getUsername(name, USERNAME_SIZE);
        if (findUser(name) != -1) {
            cout << "That username already exists" << endl;
            return false;
        }

        users[count] = newUser;
        count++;
        return true;
    }

    bool removeUser(const char wantedName[]) {
        int foundIndex = findUser(wantedName);
        if (foundIndex == -1) {
            return false;
        }

        for (int index = foundIndex; index < count - 1; index++) {
            users[index] = users[index + 1];
        }
        count--;
        return true;
    }

    bool login(const char wantedName[], const char password[]) const {
        int index = findUser(wantedName);
        if (index == -1) {
            return false;
        }
        return users[index].getPasswordHash() == hashText(password);
    }

    bool hasPermission(const User& user, const char action[]) const {
        if (user.isAdmin()) {
            return true;
        }
        if (user.isStaff() && isStaffAction(action)) {
            return true;
        }
        return false;
    }

    User& userAt(int index) {
        return users[index];
    }

    const User& userAt(int index) const {
        return users[index];
    }
};
