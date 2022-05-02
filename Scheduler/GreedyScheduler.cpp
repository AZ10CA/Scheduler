//
// Created by Alireza Zahedi on 2022-05-02.
//

#include "GreedyScheduler.h"

bool GreedyScheduler::is_timeline_occupied(int start, int end) {
    // O(n) where n is end - start
    for(int i = start; i < end; i++)
        if(_timeline[i])
            return true;
    return false;
}

bool GreedyScheduler::set_strict_habit(const Habit &habit) {
    // precise
    if(not is_timeline_occupied(habit.min / PRECISION, habit.max / PRECISION)) {
        set_timeline(habit.min / PRECISION, habit.max / PRECISION, true);
        _schedule.emplace_back(habit.name, habit.min / PRECISION, habit.max / PRECISION);
        return true;
    }
    return false;
}

bool GreedyScheduler::set_flexible_habit(const Habit &habit) {
    // finds the first free-range that has the length of habit.duration and then puts the flexible habit in the range
    const int &s = habit.min / PRECISION, e = habit.max / PRECISION, d = habit.duration / PRECISION;
    assert(e - s > d);
    bool found_range = false;
    for (int i = s; not found_range && i < e - d + 1; i++) {
        for (int j = 0; j < e; j++) {
            if (not _timeline[i + j]) {
                if (j == d - 1) {
                    for (int z = 0; z < d; z++)
                        _timeline[i + z] = true;
                    found_range = true;
                    _schedule.emplace_back(habit.name, i, i + d);
                    break;
                }
            } else break;
        }
    }
    return found_range;
}

void GreedyScheduler::greedy_schedule() {
    Stream<Habit> stream(_habits);
    stream
            // habits with tighter range should at the beginning of the array
            .bubble([](const auto& first, const auto& second){
                return first.max - first.min - first.duration
                       <= second.max - second.min - second.duration;
            })
            .peek([&](const auto& habit) {
                bool state = (habit.max - habit.min == habit.duration) ?
                             set_strict_habit(habit) :
                             set_flexible_habit(habit);
                if(not state)
                    throw runtime_error("There is an overlap between two habits!");
            });
}