cmake_minimum_required(VERSION 3.20)
project(Scheduler)

set(CMAKE_CXX_STANDARD 20)

add_executable(Scheduler main.cpp Habit.cpp Scheduler/Scheduler.cpp Utils.cpp Scheduler/GreedyScheduler.cpp Scheduler/BruteforceScheduler.cpp)
