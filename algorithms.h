//
// Created by Alireza Zahedi on 2022-04-29.
//

#ifndef SCHEDULER_ALGORITHMS_H
#define SCHEDULER_ALGORITHMS_H

template <typename T, typename Comparator>
void bubble_sort(vector<T>& vec, Comparator comparator){
    for(int i = 0; i < vec.size() - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < vec.size() - i - 1; j++)
            if(not comparator(vec[j], vec[j + 1])) {
                std::swap(vec[j], vec[j + 1]);
                swapped = true;
            }
        if(not swapped)
            break;
    }
}
// given [min, max) and an array of occupied ranges
// return an array of all ranges that are free
vector<pair<int, int>> free_ranges(int min, int max, vector<pair<int, int>> occupied){
    // assuming free ranges are within [min, max)
    bubble_sort(occupied, [](const auto& first, const auto& second){return first.first <= second.first;});
    vector<pair<int, int>> result;
    int current_min = min;
    for(int i = 0; i < occupied.size(); i++){ // O(n) where n is number of occupied ranges
        const auto&[start, end] = occupied[i];
        if(current_min < start)
            result.emplace_back(current_min, start);
        current_min = end;
        if(i == occupied.size() - 1)
            result.emplace_back(end, max);
    }
    if(occupied.empty())
        result.emplace_back(min, max);
    return result;
}

#endif //SCHEDULER_ALGORITHMS_H
