#include "Scheduler.h"

int main() {
    /** files **/
    const char* main_data_file = "habits.data.txt";

    Scheduler scheduler(main_data_file);
    /** greedy approach **/
    try {
        scheduler.greedy_schedule();
        scheduler.output_schedule("greedy_schedule.txt");
    } catch (runtime_error& e) {
        cout << "Greedy approach failed" << endl;
    }

    /** Bruteforce approach **/
    try {
        scheduler.bruteforce_schedule();
        scheduler.output_schedule("bruteforce_schedule.txt");
    } catch (runtime_error& e) {
        cout << "bruteforce approach failed" << endl;
    }

    scheduler.sort_data_file([](const auto& first, const auto& second){
        return first.min <= second.min;
    });

    return 0;
}

