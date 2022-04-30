//
// Created by Alireza Zahedi on 2022-04-29.
//

#include "Utils.h"
#include "Habit.h"

Habit::Habit(string name, int min, int max, int duration) : name(std::move(name)), min(min), max(max), duration(duration){set_id();}

ostream &operator<<(ostream &os, const Habit &habit) {
    os << "name: " << habit.name << " min: " << habit.min << " max: " << habit.max << " duration: " << habit.duration;
    return os;
}

Habit::Habit(const string &raw) {
    auto split = Utils::split(raw, ',');
    name = Utils::read_token(split[0], '"');
    min = (int) Utils::to_military(split[1]);
    max = (int) Utils::to_military(split[2]);
    duration = split.size() == 3 ? max - min : stoi(split[3]);
    if(duration > max - min)
        throw runtime_error("Duration is greater than the [min, max) range");
    set_id();
}

auto Habit::id_generator = 0;

