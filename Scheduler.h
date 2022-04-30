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
    constexpr static int PRECISION = 5_MIN;

    bitset<1_DAY / PRECISION> timeline;
    vector<tuple<string, int, int>> schedule;

    string habits_file;
    vector<Habit> habits;

    bool has_solution{};
    unordered_map<int, pair<int, int>> bf_schedule;
    vector<unordered_map<int, pair<int, int>>> bf_results;

public:

    explicit Scheduler(string habits_file);

    void load_habits();

    void greedy_schedule();

    [[nodiscard]] auto get_schedule() const;

    void output_schedule(const char *filename);

    int allocated_time();

    void bruteforce_schedule();

    template<typename Comparator>
    void sort_data_file(Comparator comparator);

private:
    [[nodiscard]] const Habit &get_habit_by_id(int id) const;

    bool set_strict_habit(const Habit &habit);

    bool set_flexible_habit(const Habit &habit);

    vector<pair<int, int>> free_ranges(int s, int e, int min_length = 0);

    bool is_timeline_occupied(int start, int end);

    void set_timeline(int start, int end, bool value);

    void recursive_planner(int count);
};

template<typename Comparator>
void Scheduler::sort_data_file(Comparator comparator) {
    Stream<Habit> stream(habits);
    ofstream writer(habits_file);
    stream
            .bubble(comparator)
            .peek([&](const auto& habit){
                writer << '"' << habit.name << '"' << ", "
                       << Utils::to_standard(habit.min) << ", "
                       << Utils::to_standard(habit.max) << ", "
                       << habit.duration << endl;
            });

    writer.close();
}

#endif //SCHEDULER_SCHEDULER_H
