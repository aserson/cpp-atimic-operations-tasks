#pragma once

#include "my_control_block.h"

template <class TClass>
class my_shared_ptr;

template <class TClass>
class my_weak_ptr {
    TClass* _ptr {nullptr};
    my_control_block* _control_block {nullptr};

public:
    my_weak_ptr() noexcept = default;

    my_weak_ptr(my_shared_ptr<TClass> const &shared) noexcept {
        _ptr = shared._ptr;
        _control_block = shared._control_block;

        if (_control_block) {
            _control_block->add_weak();
        }
    }

    my_weak_ptr<TClass>& operator=(my_shared_ptr<TClass> const& other) noexcept {
        my_weak_ptr<TClass> tmp(other);
        this->swap(tmp);

        return *this;
    }

    my_weak_ptr(my_weak_ptr<TClass> const &other) noexcept {
        if (other._control_block) {
            other._control_block->add_weak();

            _ptr = other._ptr;
            _control_block = other._control_block;
        }
    }

    my_weak_ptr<TClass>& operator=(my_weak_ptr<TClass> const& other) noexcept {
        if (this != &other) {
            my_weak_ptr<TClass> tmp(other);
            this->swap(tmp);
        }
        return *this;
    }

    my_weak_ptr(my_weak_ptr<TClass>&& other) noexcept {
        _ptr = other._ptr;
        _control_block = other._control_block;

        other._ptr = nullptr;
        other._control_block = nullptr;
    }

    my_weak_ptr<TClass>& operator=(my_weak_ptr<TClass>&& other) noexcept {
        if (this != &other) {
            my_weak_ptr<TClass> tmp(std::move(other));
            this->swap(tmp);
        }

        return *this;
    }

    ~my_weak_ptr() {
        release();
    }

    my_shared_ptr<TClass> lock() const noexcept {
        my_shared_ptr<TClass> ret;
        if (_control_block)
            ret.construnct_from_weak(*this);
        return ret;
    }

    bool expired() const noexcept {
        return !_control_block || _control_block->get_strong_count() == 0;
    }

    void reset() noexcept {
        release();

        _ptr = nullptr;
        _control_block();
    }

    std::size_t use_count() const noexcept {
        return _control_block ? _control_block->get_strong_count() : 0;
    }

    void swap(my_weak_ptr<TClass>& other) noexcept {
        std::swap(this->_ptr, other._ptr);
        std::swap(this->_control_block, other._control_block);
    }

    bool operator==(const my_weak_ptr& other) const noexcept  {
        return (_ptr == other._ptr && _control_block == other._control_block);
    }

    bool operator!=(const my_weak_ptr& other) const noexcept {
        return !(*this == other);
    }

private:
    void release() {
        if (_control_block) {
            _control_block->sub_weak();

            if (_control_block->get_strong_count() == 0 && _control_block->get_weak_count() == 0) { 
                delete _control_block;
            }
        }
    }

    friend class my_shared_ptr<TClass>;
};
