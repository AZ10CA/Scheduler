#include "Scheduler.h"


int test() {
    constexpr int size = 1100000;
    int start = 0;
    int end = 100;
    bitset<size> bits;
    cout << Timer(Utils::set<size>(bits, start, end, true);) << endl;
    cout << Timer(
            for(int i = start; i < end; i++)
                bits[i] = true;
    ) << endl;

    return 0;
}

int main() {
//    test();
//    return 0;
    /** files **/
    const char* main_data_file = "habits.data.txt";

    Scheduler scheduler(main_data_file);
    /** greedy approach **/
    cout << "Greedy took: " << endl << Timer(try {
        scheduler.greedy_schedule();
        scheduler.output_schedule("greedy_schedule.txt");
    } catch (runtime_error& e) {
        cout << "Greedy approach failed" << endl;
    }) << endl;

    /** Bruteforce approach **/
    cout << "Bruteforce took: " << endl << Timer(try {
        scheduler.bruteforce_schedule();
        scheduler.output_schedule("bruteforce_schedule.txt");
    } catch (runtime_error& e) {
        cout << "bruteforce approach failed" << endl;
    }) << endl;

    cout << "unordered_map detected and avoided " << scheduler.get_duplicate_calls() << " duplicate calls!" << endl;

    scheduler.sort_data_file([](const auto& first, const auto& second){
        return first.min <= second.min;
    });


    return 0;
}

