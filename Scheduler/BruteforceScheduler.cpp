//
// Created by Alireza Zahedi on 2022-05-02.
//

#include "BruteforceScheduler.h"

void BruteforceScheduler::recursive_planner(int count) {
    // Memoization [timeline][count]
    if(states.contains(_timeline))
        if(states[_timeline].contains(count))
            if(not states[_timeline][count]) {
                duplicate_calls++;
                return;
            }

    if (has_solution)
        return;

    if (count == _habits.size()) {
        has_solution = true;
        results = schedule;
        return;
    }

    const auto &habit = _habits[count];

    // O(m) where m is (habit.max / PRECISION - habit.min / PRECISION)
    const auto &ranges = free_ranges(habit.min / PRECISION, habit.max / PRECISION, habit.duration / PRECISION);
    if (ranges.empty()) {
        // previous habits are located such that this habit no longer has any free ranges
        // in this case we should return and relocate previous habit(s)
        unordered_map<int, bool> map;
        map[count] = false;
        states[_timeline] = map;
        return;
    }

    for (const auto&[start, cnt]: ranges) { // O(n) where n is the number of free ranges
        auto end = start + cnt;
//            [0, 90), duration = 10 -> [0, 10), ..., [80, 90) -> [start, start + duration), ..., [end - duration, end)
// i -> [start, end - duration]

        auto copy = _timeline; // O(1) is_trivially_copyable
        for (int i = start; i <= end - habit.duration / PRECISION; i++) { // O(end - habit.duration / PRECISION - start + 1)
            // set state

            set_timeline(i, i + habit.duration / PRECISION, true);
            schedule[habit.id] = make_pair(i * PRECISION, i * PRECISION + habit.duration); // O(1)

            recursive_planner(count + 1);

            // revert state
            schedule.erase(habit.id); // O(1)
            _timeline = copy;// O(1) -> is_trivially_copyable
        }
    }
}

void BruteforceScheduler::bruteforce_schedule() {
    // so much faster with this sort
    // since this sort will give strict habits a priority and puts them into the schedule immediately
    // when they are put in the schedule, timeline is much smaller and easier to deal with
    Utils::bubble_sort(_habits, [](const auto& first, const auto& second){
        return first.max - first.min - first.duration
               <= second.max - second.min - second.duration;
    });

    reset();

    recursive_planner(0);

    if(not has_solution)
        throw runtime_error("No solutions found!");

    // only outputting the first schedule to the file
    auto first_schedule = results;
    for(const auto&[id, value]: first_schedule) {
        const auto&[start, end] = value;
        _schedule.emplace_back(get_habit_by_id(id).name, start / PRECISION, end / PRECISION);
    }
}


int BruteforceScheduler::get_duplicate_calls() const {
    return duplicate_calls;
}

void BruteforceScheduler::reset() {
    has_solution = false;
    _timeline.reset();
    schedule.clear();
    _schedule.clear();
    states.clear();
    duplicate_calls = 0;
}
