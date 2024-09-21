#pragma once

#include <atomic>

class my_control_block {
    std::atomic<std::size_t> _strong_count{ 1 };
    std::atomic<std::size_t> _weak_count{ 0 };

public:
    my_control_block() = default;
    my_control_block(my_control_block const& other) = default;
    my_control_block& operator=(my_control_block const& other) = default;

    my_control_block(my_control_block && other) = delete;
    my_control_block& operator=(my_control_block && other) = delete;

    ~my_control_block() = default;

    std::size_t get_strong_count() const {
        return _strong_count.load();
    }

    std::size_t get_weak_count() const {
        return _weak_count.load();
    }

    std::size_t get_strong_count(std::memory_order order) const {
        return _strong_count.load(order);
    }

    std::size_t get_weak_count(std::memory_order order) const {
        return _weak_count.load(order);
    }

    bool compare_and_add(std::size_t& current_count, std::memory_order order1, std::memory_order order2) {
        return _strong_count.compare_exchange_weak(current_count, current_count + 1, order1, order2);
    }

    void add_strong() {
        ++_strong_count;
    }

    void add_weak() {
        ++_weak_count;
    }

    void sub_strong() {
        --_strong_count;
    }

    void sub_weak() {
        --_weak_count;
    }


};
        