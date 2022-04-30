#include "Scheduler.h"
#include "algorithms.h"


int main() {
    const char* out_file = "greedy_schedule.txt";
    Scheduler scheduler(
                        "habits.data.txt");
//    scheduler.output_schedule(out_file);
    scheduler.greedy_schedule();
    scheduler.output_schedule(out_file);

    return 0;
}
