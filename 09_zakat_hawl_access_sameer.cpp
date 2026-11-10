// Owner: Sameer Baig
// Let's just get this working for now.
bool hawlComplete(int startDay, int todayDay) {
    if (startDay == 0) {
        return false;
    }
    if (todayDay - startDay >= HAWL_DAYS) {
        return true;
    }
    return false;
}

bool canRunZakat(const AuthManager& auth, const User& user) {
    return auth.hasPermission(user, "zakat");
}

bool canChangeNisab(const AuthManager& auth, const User& user) {
    return auth.hasPermission(user, "change_nisab");
}
