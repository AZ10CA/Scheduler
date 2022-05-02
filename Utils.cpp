//
// Created by Alireza Zahedi on 2022-04-25.
//

#include "Utils.h"


vector<string> Utils::split(const string &str, char dlm) {
    vector<string> result;
    string acc;
    for (int i = 0; i < str.length(); i++) {
        char letter = str[i];
        if (letter == dlm) {
            result.push_back(std::move(acc));
            acc = "";
        } else if (i == str.length() - 1) {
            acc += letter;
            result.push_back(std::move(acc));
            acc = "";
        } else acc += letter;
    }
    return result;
}

Stream<string> Utils::file_to_stream(const string &filename) {
    ifstream reader(filename);
    vector<string> lines;
    while (reader) {
        string line;
        if (getline(reader, line))
            lines.push_back(std::move(line));
    }
    reader.close();
    return Stream<string>(std::move(lines));
}

unsigned long long Utils::pow(long long int num, int power) {
    if (not power)
        return 1;
    unsigned long long cache = pow(num, power >> 1);
    return 1ULL * ((power & 1) ? num : 1) * cache * cache;
}

string Utils::read_token(const string &str, char dlm) {
    // tested
    int indices[2]{};
    for (int i = 0, idx = 0; idx < 2 && i < str.length(); i++)
        if (str[i] == dlm)
            indices[idx++] = i;
    if (not indices[1])
        throw runtime_error("Delimiters were not found!");
    return str.substr(indices[0] + 1, indices[1] - indices[0] - 1);
}

unsigned long long Utils::to_military(const string &standard, char dlm) {
    vector<string> spl = Utils::split(standard, dlm);
    auto is_empty = [](const string &str) { return str.empty(); };
    if (spl.empty() or any_of(spl.begin(), spl.end(), is_empty))
        throw runtime_error("Invalid time parameter");
    std::reverse(spl.begin(), spl.end());

    unsigned long long acc{};
    for (int i = 0; i < spl.size(); i++)
        acc += stoi(spl[i]) * Utils::pow(60, i);
    return acc;
}

string Utils::to_standard(int military, char dlm) {
    // military has to be in seconds
    assert(military >= 0);
    int hours = military / 1_HOUR;
    military %= 1_HOUR;
    int minutes = military / 1_MIN;
    military %= 1_MIN;
    return (hours < 10 ? "0" : "") + std::to_string(hours)
           + dlm + (minutes < 10 ? "0" : "") + std::to_string(minutes)
           + dlm + (military < 10 ? "0" : "") + std::to_string(military);
}

