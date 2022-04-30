//
// Created by Alireza Zahedi on 2022-04-29.
//

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H
#include <unordered_map>
#include "Utils.h"
#include "Habit.h"
/**
 * To increase performance:
 * set the precision of the timeline to minutes instead of the seconds, it is possible since 60 is the divisor of min, max, duration
 * in general you can find the greatest common divisor of min, max, duration and set it as the precision of the timeline
 * for example:
 * GCD = min(gcd(min, max, duration) for h in habits) -> assume its 5min
 * timeline -> bitset<288> since 1 day is 24 * 60 min and can be divided by 5min -> 288 --- 86400 -> 288 is huge
 *
 *  Bruteforce way of fitting all habits into the schedule
 *  right now we are just lucky because the greedy algorithm works and can fit all habits into the schedule
 *  but for tight schedules the greedy method doesn't always work
 *  so we want to use bruteforce to try all combinations of the schedule and only accepts an schedule that can contain
 *  all of the habits.
 *
 *  This can be time consuming and the process might not even finish in our lifetime, since I want to generate all
 *  possible combinations, but there are a lot of methods to early prone the bruteforce, so lets start:
 *  ------
 *  Observations:
 *  since we know in an acceptable schedule, all habits are present, we can go ahead and put all of the strict habits in the
 *  timeline first, this task is easy and also makes our timeline smaller which is great because a smaller timeline is quicker
 *  when bruteforce method is used.
 *  so step 1: put all strict habits in the timeline first, in other words:
 *      fill timeline [habit.min, habit.max) where habit.max - habit.min - duration == 0
 * -------
 * now lets say we have a habit as follow:
 * Habit t =>
 *      min = 600;
 *      max = 1400;
 *      duration: 300;
 *
 * this habit is of course a flexible habit since habit.max - habit.min - duration == 0 doesn't hold so all i's such that
 * timeline[i] is free in range [i, i + duration) and i >= min && i + duration <= max are acceptable
 * the goal is to fix habit "t" in each of these positions and then move to the other habits and see if all of them can also
 * fit in the schedule, if so we have an schedule, if not move to the next permutation
 *
 * so for habit "t" the candidate ranges (assuming that the whole range [600, 1400) is free) are:
 * <1.1>[600, 990), [601, 991), [602, 992), ..., [1100, 1400), length = 1100 - 600 + 1 = 501 acceptable range
 * so all i's in range [min, max - duration] are acceptable starting time for habit "t"
 * but what if the whole range is not free for us?
 * assume:
 * timeline[i] = occupied for i -> [600, 700) and i -> [850, 900) and i [900, 1100)
 * not there are three ranges that have an overlap with our [min, max) range, so we don't have all of the ranges
 * that were proposed at *1.1.
 * with the given occupied ranges:
 * free ranges are:
 * [700, 850), [900, 900) -> invalid, [1100, 1400) -> [700, 850), [1100, 1400)
 * use algorithm::free_ranges(min, max, occupied_ranges) to get free ranges -> O(n^2) can be reduced to O(n) where n is
 * the number of occupied ranges (occupied_ranges.size())
 * then:
 * for(free_range: free_ranges)
 *      if(length of free_range is greater than t.duration)
 *          continue;
 *      else
 *          we have a candidate
 */

template <typename A, typename B, typename C>
ostream& operator<<(ostream& os, const tuple<A, B, C>& tup){
    const auto&[a, b, c] = tup;
    os << "(" << a << ", " << b << ", " << c << ")";
    return os;
}

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& vec){
    os << '[';
    for(const auto& each: vec)
        os << each << ", ";
    os << ']';
    return os;
}

class Scheduler {
    bitset<24_HOUR> timeline;
    vector<tuple<string, int, int>> schedule;

    string habits_file;
    vector<Habit> habits;

    unordered_map<string, vector<pair<int, int>>> free_ranges;

public:
    void optimal_schedule(int count){
        if(count == habits.size()){
            cout << schedule << endl;
            return;
        }
        const auto& habit = habits[count];
        const auto& ranges = free_ranges[habit.name];
        for(int i = 0; i < ranges.size(); i++) {
            const auto&[start, end] = ranges[i];
            if (is_timeline_occupied(start, end)) {
                if (i == ranges.size() - 1)
                    // the current timeline can't fit this schedule since all the free ranges for this habit,
                    // are already occupied by other habits
                    return;
                else
                    continue;
            }
            auto copy = timeline;
            set_timeline(start, end, true); // set state

            optimal_schedule(count + 1);

            timeline = copy; // unset state
        }
    }

    bool is_timeline_occupied(int start, int end);

    void set_timeline(int start, int end, bool value);


    explicit Scheduler(string habits_file) :
            habits_file(std::move(habits_file)) {
        load_habits();
    }

    void set_free_ranges(){
        for(const auto& habit: habits){
            vector<pair<int, int>> ranges;

        }
    }

public:
    void load_habits();

    void greedy_schedule();

    private:
    bool set_strict_habit(const Habit& habit) {
        if(not is_timeline_occupied(habit.min, habit.max)) {
            set_timeline(habit.min, habit.max, true);
            schedule.emplace_back(habit.name, habit.min, habit.max);
            return true;
        }
        return false;
    }

    bool set_flexible_habit(const Habit& habit) {
        const int &s = habit.min, e = habit.max, d = habit.duration;
        assert(e - s > d);
        bool found_range = false;
        for (int i = s; not found_range && i < e - d + 1; i++) {
            for (int j = 0; j < e; j++) {
                if (not timeline[i + j]) {
                    if (j == d - 1) {
                        for (int z = 0; z < d; z++)
                            timeline[i + z] = true;
                        found_range = true;
                        schedule.emplace_back(habit.name, i, i + habit.duration);
                        break;
                    }
                } else break;
            }
        }
        return found_range;
    }

public:
    auto get_timeline();

    [[nodiscard]] auto get_schedule() const;

    void output_schedule(const char *filename);

    int allocated_time();

};



#endif //SCHEDULER_SCHEDULER_H
