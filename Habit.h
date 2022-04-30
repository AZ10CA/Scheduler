//
// Created by Alireza Zahedi on 2022-04-29.
//

#ifndef SCHEDULER_HABIT_H
#define SCHEDULER_HABIT_H
#include <iostream>
using namespace std;

struct Habit {// precision of habit is in seconds, but it is converted to PRECISION in the Scheduler class
    string name;
    int min;
    int max;
    int duration;

    Habit(string name, int min, int max, int duration);

    explicit Habit(const string& raw);

    friend ostream &operator<<(ostream &os, const Habit &habit);
};


#endif //SCHEDULER_HABIT_H
