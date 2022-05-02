//
// Created by Alireza Zahedi on 2022-04-29.
//

#include "Scheduler.h"

int Scheduler::get_alloc_time() {
    int allocated = 0;
    for (const auto &[n, s, e]: _schedule)
        allocated += e - s;
    return allocated;
}

void Scheduler::output_schedule(const char *filename) {
    // writes the schedule to a file with the given name
    // sorts habits based on their starting time
    ofstream writer(filename);
    Stream<tuple<string, int, int>> schedule_stream(_schedule);

    writer << "name, start, end" << endl << endl;
    schedule_stream
            .bubble([](const auto &first, const auto &second) {
                const auto&[n1, s1, e1] = first;
                const auto&[n2, s2, e2] = second;
                return s1 <= s2;
            })
            .peek([&](auto &habit) {
                const auto&[name, start, end] = habit;
                writer << '"' << name << '"' << ", " <<
                       Utils::to_standard(start * PRECISION) << ", " << Utils::to_standard(end * PRECISION) << endl;
            });

    writer << endl << "Allocated time: " << Utils::to_standard(get_alloc_time() * PRECISION) << endl;

    writer.close();
}

void Scheduler::set_timeline(int start, int end, bool value) {
    for(int i = start; i < end; i++)
        _timeline[i] = value;
}


Scheduler::Scheduler(string habits_file) :
        _habits_file(std::move(habits_file)) {
    Utils::file_to_stream(this->_habits_file)
            .filter([](const auto &ln) { return not ln.empty(); })
            .map<Habit>([](const auto &ln) { return Habit(ln); })
            .peek([&](auto &habit) {
                check_precision(habit);
                _habits.push_back(std::move(habit));
            });
}

void Scheduler::check_precision(const Habit& habit){
    if (habit.duration % PRECISION || habit.min % PRECISION || habit.max % PRECISION)
        throw runtime_error("duration, min, max must be multiples of " + std::to_string(PRECISION) +
        " The problem occurred while processing the following habit: " + habit.to_string());
}

vector<pair<int, int>> Scheduler::free_ranges(int s, int e, int min_length) {
    // O(n) where n = e - s;

    const auto& range = _timeline;
    int start = s;
    int cnt = 0;
    bool value = range[s];
    vector<pair<int, int>> results;
    for(int i = s; i < e + 1; i++){
        const auto& element = (i == e ? !range[i - 1] : range[i]);
        if(element == value)
            cnt++;
        else {
            if(not value && cnt >= min_length)
                results.emplace_back(start, cnt);
            value = !value;
            cnt = 1;
            start = i;
        }
    }
    return results;
}

const Habit &Scheduler::get_habit_by_id(int id) const {
    for(const auto& habit: _habits)
        if(habit.id == id)
            return habit;
    throw runtime_error("invalid id");
}
