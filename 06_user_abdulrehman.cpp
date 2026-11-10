// Owner: Abdulrehman
// Let's just get this working for now.
class User {
private:
    char username[USERNAME_SIZE];
    int passwordHash;
    char role[ROLE_SIZE];

public:
    User() {
        username[0] = '\0';
        passwordHash = 0;
        role[0] = '\0';
    }

    User(const char newUsername[], int newHash, const char newRole[]) {
        copyText(username, newUsername, USERNAME_SIZE);
        passwordHash = newHash;
        copyText(role, newRole, ROLE_SIZE);
    }

    void getUsername(char output[], int maxSize) const {
        copyText(output, username, maxSize);
    }

    int getPasswordHash() const {
        return passwordHash;
    }

    void getRole(char output[], int maxSize) const {
        copyText(output, role, maxSize);
    }

    bool isAdmin() const {
        return textsMatch(role, ROLE_ADMIN);
    }

    bool isStaff() const {
        return textsMatch(role, ROLE_STAFF);
    }
};
