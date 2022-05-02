//
// Created by Alireza Zahedi on 2022-05-02.
//

#ifndef SCHEDULER_BRUTEFORCESCHEDULER_H
#define SCHEDULER_BRUTEFORCESCHEDULER_H
#include "Scheduler.h"

class BruteforceScheduler: public Scheduler{
    unordered_map<int, pair<int, int>> schedule;
    unordered_map<int, pair<int, int>> results;

    // Memoization (for speed up purposes)
    unordered_map<bitset<BITS>, unordered_map<int, bool>> states{};

    int duplicate_calls{};
    bool has_solution{};

public:
    explicit BruteforceScheduler(string filename): Scheduler(std::move(filename)){}

    void bruteforce_schedule();

    [[nodiscard]] int get_duplicate_calls() const;

private:
    void recursive_planner(int count);

    void reset();
};


#endif //SCHEDULER_BRUTEFORCESCHEDULER_H
