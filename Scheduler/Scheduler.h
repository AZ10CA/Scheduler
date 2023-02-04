//
// Created by Alireza Zahedi on 2022-04-29.
//

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H
#include <unordered_map>
#include "../Utils.h"
#include "../Habit.h"


class Scheduler {
protected:
    constexpr static int PRECISION = 15_MIN;
    constexpr static int BITS = 1_DAY / PRECISION;

    bitset<BITS> _timeline;
    vector<tuple<string, int, int>> _schedule;

    string _habits_file;
    vector<Habit> _habits;
public:
    explicit Scheduler(string habits_file);

    void output_schedule(const char *filename);

    int get_alloc_time();

    template<typename Comparator>
    void sort_data_file(Comparator comparator);

private:
    static void check_precision(const Habit& habit);

protected:
    [[nodiscard]] const Habit &get_habit_by_id(int id) const;

    vector<pair<int, int>> free_ranges(int s, int e, int min_length = 0);

    void set_timeline(int start, int end, bool value);
};

template<typename Comparator>
void Scheduler::sort_data_file(Comparator comparator) {
    Stream<Habit> stream(_habits);
    ofstream writer(_habits_file);
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
