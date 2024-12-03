#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

constexpr ushort MIN_SAFE = 1;
constexpr ushort MAX_SAFE = 3;

bool checkDifference(const int current, const int previous) {
    const int diff = abs(current - previous);
    return diff >= MIN_SAFE && diff <= MAX_SAFE;
}

bool checkIncreasing(const bool isIncreasing, const int current, const int previous) {
    const bool isItemIncreasing = current > previous;
    if (isIncreasing && isItemIncreasing) return true;
    if (!isIncreasing && !isItemIncreasing) return true;
    return false;
}

bool checkReports(const std::vector<int> *reports) {
    int previousItem = (*reports)[0];
    const bool isIncreasing = previousItem < (*reports)[1];

    for (int i = 1; i < reports->size(); i++) {
        const int item = (*reports)[i];
        if (!checkIncreasing(isIncreasing, item, previousItem)) return false;
        if (!checkDifference(item, previousItem)) return false;

        previousItem = item;
    }

    return true;
}

bool checkIfReportsSafe(const std::vector<int> *origReports) {
    if (checkReports(origReports)) return true;

    const size_t reportsSize = origReports->size();
    for (int i = 0; i < origReports->size(); i++) {
        std::vector<int> reports = *origReports;
        reports.erase(reports.begin() + i);
        if (checkReports(&reports)) return true;
    }

    return false;
}

std::vector<int> convertLine(const std::string *str) {
    std::stringstream ss(*str);

    std::vector<int> reports;
    std::string token;
    while (std::getline(ss, token, ' '))
        reports.push_back(std::stoi(token));

    return reports;
}

int main() {
    std::ifstream input("input.txt");
    std::string str;

    size_t totalSafeReports = 0;
    while (std::getline(input, str)) {
        auto reports = convertLine(&str);
        if (checkIfReportsSafe(&reports)) totalSafeReports++;
    }

    std::cout << "Total Safe Reports: " << totalSafeReports << std::endl;
}