#pragma once

#include "my_control_block.h"

template <class TClass>
class my_weak_ptr;

template <class TClass>
class my_shared_ptr {
    TClass* _ptr {nullptr};
    my_control_block* _control_block {nullptr};

public:
    my_shared_ptr() noexcept = default;

    explicit my_shared_ptr(TClass* ptr) noexcept : _ptr(ptr), _control_block(new my_control_block()) { }

    my_shared_ptr(my_shared_ptr<TClass> const &other) noexcept {
        if (other._control_block) {
            other._control_block->add_strong();

            _ptr = other._ptr;
            _control_block = other._control_block;
        }
    }

    my_shared_ptr<TClass>& operator=(my_shared_ptr<TClass> const& other) noexcept {
        if (this != &other) {
            my_shared_ptr<TClass> tmp(other);
            this->swap(tmp);
        }
        return *this;
    }

    my_shared_ptr(my_shared_ptr<TClass>&& other) noexcept {
        _ptr = other._ptr;
        _control_block = other._control_block;

        other._ptr = nullptr;
        other._control_block = nullptr;
    }

    my_shared_ptr<TClass>& operator=(my_shared_ptr<TClass>&& other) noexcept {
        if (this != &other) {
            my_shared_ptr<TClass> tmp(std::move(other));
            this->swap(tmp);
        }

        return *this;
    }

    ~my_shared_ptr() {
        release();
    }

    TClass* get() const {
        return _ptr;
    }

    TClass& operator*() const {
        return *get();
    }

    TClass* operator->() const {
        return get();
    }

    void reset() {
        release();

        _ptr = nullptr;
        _control_block = nullptr;
    }

    void reset(TClass* ptr) {
        release();
        _ptr = ptr;
        _control_block = new my_control_block();
    }

    void swap(my_shared_ptr<TClass>& other) noexcept {
        std::swap(this->_ptr, other._ptr);
        std::swap(this->_control_block, other._control_block);
    }

    std::size_t use_count() const {
        return _control_block ? _control_block->get_strong_count() : 0;
    }

    bool unique() const {
        return use_count() == 1;
    }

    bool operator==(const my_shared_ptr& other) const {
        return (_ptr == other._ptr && _control_block == other._control_block);
    }

    bool operator!=(const my_shared_ptr& other) const {
        return !(*this == other);
    }

    explicit operator bool() const {
        return _ptr != nullptr;
    }

private:
    void release() {
        if (_control_block) {
            _control_block->sub_strong();

            if (_control_block->get_strong_count() == 0) {
                delete _ptr;

                if (_control_block->get_weak_count() == 0) {
                    delete _control_block;
                }
            }
        }
    }

    bool my_shared_ptr<TClass>::construnct_from_weak(const my_weak_ptr<TClass>& other) noexcept {
        size_t current_count =
            other._control_block->get_strong_count(std::memory_order_acquire);

        while (current_count > 0) {
            if (other._control_block->compare_and_add(
                current_count, std::memory_order_acq_rel, std::memory_order_acquire)) {

                _ptr = other._ptr;
                _control_block = other._control_block;
                return true;

            }
        }

        return false;
    }

    friend class my_weak_ptr<TClass>;
};

