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
                       Utils::to_standard(start) << ", " << Utils::to_standard(end) << endl;
            });

    writer << "Allocated time: " << Utils::to_standard(allocated_time()) << endl;

    writer.close();
}

auto Scheduler::get_schedule() const {
    return schedule;
}

auto Scheduler::get_timeline() {
    // O(T)
    vector<pair<int, bool>> sum;
    int cnt = 0;
    bool prev = timeline[0];
    for (int i = 0; i < timeline.size(); i++) {
        if (timeline[i] == prev)
            cnt++;
        else {
            sum.emplace_back(cnt, prev);
            cnt = 1;
        }
        if (i == timeline.size() - 1)
            sum.emplace_back(cnt, timeline[i]);
        prev = timeline[i];
    }
    return sum;
}

void Scheduler::greedy_schedule() {
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
