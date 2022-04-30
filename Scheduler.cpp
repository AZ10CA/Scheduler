//
// Created by Alireza Zahedi on 2022-04-29.
//

#include "Scheduler.h"

int Scheduler::allocated_time() {
    int allocated = 0;
    for (const auto &[n, s, e]: schedule)
        allocated += e - s;
    return allocated;
}

void Scheduler::output_schedule(const char *filename) {
    // writes the schedule to a file with the given name
    // sorts habits based on their starting time
    ofstream writer(filename);
    Stream<tuple<string, int, int>> schedule_stream(schedule);
    vector<pair<int, int>> ranges;
    int s = 0;
    for(const auto&[name, start, end]: schedule){
        if(start > s)
            ranges.emplace_back(s, start);
        s = end;
    }

    writer << "name, start, end" << endl;
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

    writer << "Allocated time: " << Utils::to_standard(allocated_time() * PRECISION) << endl;

    writer << "Free ranges are: " << endl;
    for(const auto&[start, cnt]: ranges)
        writer << "[" << Utils::to_standard(start * PRECISION) << " to " << Utils::to_standard((start + cnt) * PRECISION) << ")" << endl;

    writer.close();
}

auto Scheduler::get_schedule() const {
    return schedule;
}

void Scheduler::greedy_schedule() {
    // precise
    Stream<Habit> stream(habits);
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

void Scheduler::load_habits() {
    Utils::file_to_stream(habits_file)
            .filter([](const auto &ln) {return not ln.empty();})
            .map<Habit>([](const auto &ln) {return Habit(ln);})
            .peek([&](auto &habit) {habits.push_back(std::move(habit));});
}

void Scheduler::set_timeline(int start, int end, bool value) {
    for(int i = start; i < end; i++)
        timeline[i] = value;
}

bool Scheduler::is_timeline_occupied(int start, int end) {
    // O(n) where n is end - start
    for(int i = start; i < end; i++)
        if(timeline[i])
            return true;
    return false;
}

bool Scheduler::set_strict_habit(const Habit &habit) {
    // precise
    if(not is_timeline_occupied(habit.min / PRECISION, habit.max / PRECISION)) {
        set_timeline(habit.min / PRECISION, habit.max / PRECISION, true);
        schedule.emplace_back(habit.name, habit.min / PRECISION, habit.max / PRECISION);
        return true;
    }
    return false;
}

bool Scheduler::set_flexible_habit(const Habit &habit) {
    // precise
    const int &s = habit.min / PRECISION, e = habit.max / PRECISION, d = habit.duration / PRECISION;
    assert(e - s > d);
    bool found_range = false;
    for (int i = s; not found_range && i < e - d + 1; i++) {
        for (int j = 0; j < e; j++) {
            if (not timeline[i + j]) {
                if (j == d - 1) {
                    for (int z = 0; z < d; z++)
                        timeline[i + z] = true;
                    found_range = true;
                    schedule.emplace_back(habit.name, i, i + d);
                    break;
                }
            } else break;
        }
    }
    return found_range;
}

Scheduler::Scheduler(string habits_file) :
        habits_file(std::move(habits_file)) {
    load_habits();
}

vector<pair<int, int>> Scheduler::free_ranges(int s, int e, int min_length) {
    // O(n) where n = e - s;
    const auto& range = timeline;
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

void Scheduler::recursive_planner(int count) {
    if (has_solution)
        return;

    if (count == habits.size()) {
        has_solution = true;
        bf_results.push_back(bf_schedule);
        return;
    }
    const auto &habit = habits[count];
    // O(m) where m is (habit.max - habit.min)
    const auto &ranges = free_ranges(habit.min / PRECISION, habit.max / PRECISION, habit.duration / PRECISION);
    if (ranges.empty())
        // previous habits are located such that this habit no longer has any free ranges
        // in this case we should return and relocate previous habit(s)
        return;

    for (const auto&[start, cnt]: ranges) { // O(n) where n is the number of free ranges
        auto end = start + cnt;
//            [0, 90), duration = 10 -> [0, 10), ..., [80, 90) -> [start, start + duration), ..., [end - duration, end)
// i -> [start, end - duration]

        auto copy = timeline; // O(1) is_trivially_copyable
        for (int i = start; i <= end - habit.duration / PRECISION; i++) { // O(end - habit.duration - start + 1)
            // set state
            set_timeline(i, i + habit.duration / PRECISION, true); // O(habit.duration)
            bf_schedule[habit.id] = make_pair(i * PRECISION, i * PRECISION + habit.duration);

            recursive_planner(count + 1);

            // revert state
            bf_schedule.erase(habit.id);
            timeline = copy;// O(1) -> is_trivially_copyable
        }
    }
}

void Scheduler::bruteforce_schedule() {
    Utils::bubble_sort(habits, [](const auto& first, const auto& second){
        return first.max - first.min - first.duration
               <= second.max - second.min - second.duration;
    });

    has_solution = false;
    timeline.reset();
    bf_schedule.clear();
    schedule.clear();

    recursive_planner(0);

    if(not has_solution)
        throw runtime_error("No solutions found!");

    // only outputting the first schedule to the file
    auto first_schedule = bf_results[0];
    for(const auto&[id, value]: first_schedule) {
        const auto&[start, end] = value;
        schedule.emplace_back(get_habit_by_id(id).name, start / PRECISION, end / PRECISION);
    }
}

const Habit &Scheduler::get_habit_by_id(int id) const {
    for(const auto& habit: habits)
        if(habit.id == id)
            return habit;
    throw runtime_error("invalid id");
}
