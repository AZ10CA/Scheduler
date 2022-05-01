#ifndef STREAM_STREAM_H
#define STREAM_STREAM_H
#include <vector>
#include <cassert>
template <typename T>
class Stream {
    /*
     * C++ Stream Class Version 1.0
     */
private:
    std::vector<T> vec;

public:
    /*** Constructors ***/
    explicit Stream(const std::vector<T>& vec);

    explicit Stream(std::vector<T>&& vec);

    Stream();

    Stream(Stream&& other) noexcept;

    Stream(const Stream& other);

    Stream& operator=(const Stream& other);

    Stream& operator=(Stream&& other) noexcept;

    /*** Methods ***/
    template <typename Predicate>
    Stream& filter(Predicate pred);

    int count();

    Stream& limit(int size);

    Stream& skip(int size);

    template<typename Consumer>
    Stream& peek(Consumer consumer);

    static Stream<int> integers(int start, int end);

    template <typename Return, typename Transformer>
    Stream<Return> map(Transformer transformer);

    Stream<std::vector<T>> partition(int parts);

    Stream& slice(int start, int end = INT32_MAX, int step = 1);

    Stream& reverse();

    template<typename Comparator>
    Stream& bubble(Comparator comparator);
};

/******************
 * Definitions
 ******************/

template<typename T>
Stream<T>::Stream(const std::vector<T> &vec) {
    this->vec = vec;
}

template<typename T>
Stream<T>::Stream(std::vector<T> &&vec) {
    this->vec = std::move(vec);
}

template<typename T>
Stream<T>::Stream() {this->vec = std::vector<T>();}

template<typename T>
Stream<T>::Stream(Stream &&other) noexcept {
    *this = std::move(other);
}

template<typename T>
Stream<T>::Stream(const Stream &other) {
    *this = other;
}

template<typename T>
Stream<T> &Stream<T>::operator=(const Stream &other) {
    if(this == &other)
        return *this;
    this->vec = other.vec;
    return *this;
}

template<typename T>
Stream<T> &Stream<T>::operator=(Stream &&other) noexcept {
    if(this == &other)
        return *this;
    this->vec = std::move(other.vec);
    return *this;
}

template<typename T>
template<typename Predicate>
Stream<T> &Stream<T>::filter(Predicate pred) {
    std::vector<T> res;
    for(T& each: vec)
        if(pred(each))
            res.push_back(std::move(each));
    this->vec = std::move(res);
    return *this;
}

template<typename T>
int Stream<T>::count() {
    return this->vec.size();
}

template<typename T>
Stream<T> &Stream<T>::limit(int size) {
    std::vector<T> res;
    bool direction = size > 0;
    int start = direction ? 0 : std::max((int) this->vec.size() + size, 0);
    int end = direction ? std::min((int) this->vec.size(), size) : this->vec.size();

    for(int i = start; i < end; i++)
        res.push_back(this->vec[i]);
    this->vec = std::move(res);
    return *this;
}

template<typename T>
Stream<T> &Stream<T>::skip(int size) {
    bool direction = size > 0;
    size = direction ? std::min((int) this->vec.size(), size) : std::min((int) this->vec.size(), -size);
    return *this = direction ? limit(size - this->vec.size()) : limit(this->vec.size() - size);
}

template<typename T>
template<typename Consumer>
Stream<T> &Stream<T>::peek(Consumer consumer) {
    for(T& each: this->vec)
        consumer(each);
    return *this;
}

template<typename T>
Stream<int> Stream<T>::integers(int start, int end) {
    std::vector<int> res;
    for(int i = start; i < end; i++)
        res.push_back(i);
    return Stream(res);
}

template<typename T>
template<typename Return, typename Transformer>
Stream<Return> Stream<T>::map(Transformer transformer) {
    std::vector<Return> res;
    for(auto each: vec)
        res.push_back(transformer(each));
    vec.clear();
    return Stream<Return>(res);
}

template<typename T>
Stream<std::vector<T>> Stream<T>::partition(int parts) {
    int partitions = this->vec.size() / parts + (this->vec.size() % parts != 0);
    std::vector<std::vector<T>> res(partitions);
    int idx = 0;
    for(int i = 0; i < partitions; i++)
        for (int j = 0; j < parts && idx < this->vec.size(); j++)
            res[i].push_back(this->vec[idx++]);

    vec.clear();
    return Stream<std::vector<T>>(res);
}

template<typename T>
Stream<T> &Stream<T>::slice(int start, int end, int step) {
    assert(start < end && start >= 0 && step);
    std::vector<T> res;
    int e = std::min((int) this->vec.size(), end);
    bool direction = step > 0; // if true go from right, left otherwise
    step = std::abs(step);
    for(int i = start; i < e; i += step) {
        int idx = direction ? i : this->vec.size() - 1 - i;
        res.push_back(std::move(this->vec[idx]));
    }
    this->vec = std::move(res);
    return *this;
}

template<typename T>
Stream<T> &Stream<T>::reverse() {
    vec = std::move(slice(0, vec.size(), -1));
    return *this;
}

template<typename T>
template<typename Comparator>
Stream<T> &Stream<T>::bubble(Comparator comparator) {
    // O(n^2)
    for(int i = 0; i < vec.size() - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < vec.size() - i - 1; j++) {
            if (not comparator(vec[j], vec[j + 1])) {
                auto copy = std::move(vec[j]);
                vec[j] = std::move(vec[j + 1]);
                vec[j + 1] = std::move(copy);
                swapped = true;
            }
        }
        if(not swapped)
            break;
    }
    return *this;
}

#endif //STREAM_STREAM_H
