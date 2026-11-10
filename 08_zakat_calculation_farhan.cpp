// Owner: Farhan Abbass
// Let's just get this working for now.
struct ZakatSettings {
    double silverPrice;
    double goldPrice;
    bool useSilver;
};

struct ZakatResult {
    bool due;
    double zakatableWealth;
    double amount;
};

double nisabValue(double silverPrice, double goldPrice, bool useSilver) {
    if (useSilver) {
        return NISAB_SILVER_GRAMS * silverPrice;
    }
    return NISAB_GOLD_GRAMS * goldPrice;
}

ZakatResult calculateZakat(double totalWealth, double debts, double nisab) {
    ZakatResult result;
    result.due = false;
    result.zakatableWealth = totalWealth - debts;
    result.amount = 0;

    if (result.zakatableWealth < 0) {
        result.zakatableWealth = 0;
    }

    if (result.zakatableWealth >= nisab) {
        result.due = true;
        result.amount = result.zakatableWealth * ZAKAT_RATE;
    }
    return result;
}
