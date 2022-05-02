//
// Created by Alireza Zahedi on 2022-05-02.
//

#ifndef SCHEDULER_GREEDYSCHEDULER_H
#define SCHEDULER_GREEDYSCHEDULER_H
#include "Scheduler.h"

class GreedyScheduler: public Scheduler {
public:
    explicit GreedyScheduler(string filename) : Scheduler(std::move(filename)) {}

    void greedy_schedule();

private:
    bool set_strict_habit(const Habit &habit);

    bool set_flexible_habit(const Habit &habit);

    bool is_timeline_occupied(int start, int end);
};

#endif //SCHEDULER_GREEDYSCHEDULER_H
