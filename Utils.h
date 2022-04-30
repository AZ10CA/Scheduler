//
// Created by Alireza Zahedi on 2022-04-25.
//

#ifndef SPECTRUM_UTILS_H
#define SPECTRUM_UTILS_H
#include <fstream>
#include "Stream.h"
using namespace std;

constexpr static int PRECISION = (5 * 60);

constexpr int operator "" _SECOND(unsigned long long seconds) {
    return (int) seconds;
}

constexpr int operator "" _MIN(unsigned long long minutes) {
    return (int) minutes * 60;
}

constexpr int operator "" _HOUR(unsigned long long hours) {
    return (int) hours * 60_MIN;
}

constexpr int operator "" _DAY(unsigned long long days) {
    return (int) days * 24_HOUR;
}

class Utils {
public:
    static vector<string> split(const string &str, char dlm);

    static Stream<string> file_to_stream(const string &filename);

    static unsigned long long pow(long long num, int power);

    static string read_token(const string &str, char dlm);

    static unsigned long long to_military(const string &standard, char dlm = ':');

    static string to_standard(int military, char dlm = ':');
};


#endif //SPECTRUM_UTILS_H
