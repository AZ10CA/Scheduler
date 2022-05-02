#include "Scheduler/GreedyScheduler.h"
#include "Scheduler/BruteforceScheduler.h"

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

void test_greedy_scheduler(const char* filename){
    GreedyScheduler scheduler1(filename);
    /** greedy approach **/
    cout << "Greedy took: " << endl << Timer(try {
        scheduler1.greedy_schedule();
        scheduler1.output_schedule("greedy_schedule.txt");
    } catch (runtime_error& e) {
        cout << "Greedy approach failed" << endl;
    }) << endl;
}

void test_bruteforce_scheduler(const char* filename){
    BruteforceScheduler scheduler2(filename);
    /** Bruteforce approach **/
    cout << "Bruteforce took: " << endl << Timer(try {
        scheduler2.bruteforce_schedule();
        scheduler2.output_schedule("bruteforce_schedule.txt");
    } catch (runtime_error& e) {
        cout << "bruteforce approach failed" << endl;
    }) << endl;

    cout << "unordered_map detected and avoided " << scheduler2.get_duplicate_calls() << " duplicate calls!" << endl;

    scheduler2.sort_data_file([](const auto& first, const auto& second){
        return first.min <= second.min;
    });
}

int main() {
    const char* main_data_file = "habits.data.txt";
    test_greedy_scheduler(main_data_file);
    test_bruteforce_scheduler(main_data_file);

    return 0;
}

