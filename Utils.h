//
// Created by Alireza Zahedi on 2022-04-25.
//

#ifndef SPECTRUM_UTILS_H
#define SPECTRUM_UTILS_H
#include <fstream>
#include "Stream.h"
using namespace std;

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
#define Timer(sth) measure_time([&](){sth})
auto measure_time =
        [](auto&& func, auto&&... params) {
            const auto& start = std::chrono::high_resolution_clock::now();
            std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
            const auto& stop = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
        };

class Utils {
public:
    static vector<string> split(const string &str, char dlm);

    static Stream<string> file_to_stream(const string &filename);

    static unsigned long long pow(long long num, int power);

    static string read_token(const string &str, char dlm);

    static unsigned long long to_military(const string &standard, char dlm = ':');

    static string to_standard(int military, char dlm = ':');

    template <typename T, typename Comparator>
    static void bubble_sort(vector<T>& vec, Comparator comparator);

    template <int T>
    static bitset<T> bitmask(int start, int end);

    template<int T>
    static void set(bitset<T>& bits, int start, int end, bool value = true);

};

template<int T>
void Utils::set(bitset<T> &bits, int start, int end, bool value) {
    auto mask = Utils::bitmask<T>(start, end);
    bits = value ? bits | mask : bits & ~mask;
}


template<int T>
bitset<T> Utils::bitmask(int start, int end) {
    assert(end <= T && end > start && start >= 0);
    bitset<T> mask(1);
    mask = ~(mask << (T - 1));
    mask[T - 1] = true;
    mask >>= T - (end - start); // setting the number of 1s we need
    // now we align the 1s
    mask <<= start;

//    bitset<T> mask = (~(1 << (end - start))) << (T - end);
    return mask;
}

template<typename T, typename Comparator>
void Utils::bubble_sort(vector<T> &vec, Comparator comparator) {
    for(int i = 0; i < vec.size() - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < vec.size() - i - 1; j++)
            if(not comparator(vec[j], vec[j + 1])) {
                std::swap(vec[j], vec[j + 1]);
                swapped = true;
            }
        if(not swapped)
            break;
    }
}


#endif //SPECTRUM_UTILS_H
