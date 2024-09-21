// Как бы вы реализовали работу с общими и слабыми умными указателями?

#include <iostream>
#include <cassert>
#include "my_shared_ptr.h"
#include "my_weak_ptr.h"

void test_shared_creation_and_use_count() {
    my_shared_ptr<int> sp1(new int(42));
    assert(sp1.use_count() == 1);
    assert(*sp1 == 42);
    std::cout << "Test creation and use count passed!" << std::endl;
}

void test_shared_copying() {
    my_shared_ptr<int> sp1(new int(42));
    my_shared_ptr<int> sp2 = sp1;
    assert(sp1.use_count() == 2);
    assert(sp2.use_count() == 2);
    assert(*sp2 == 42);
    std::cout << "Test copying shared_ptr passed!" << std::endl;
}

void test_shared_reset_and_use_count() {
    my_shared_ptr<int> sp1(new int(42));
    sp1.reset();
    assert(sp1.use_count() == 0);
    assert(sp1.get() == nullptr);
    std::cout << "Test reset and use count passed!" << std::endl;
}

void test_shared_move() {
    my_shared_ptr<int> sp1(new int(42));
    my_shared_ptr<int> sp2 = std::move(sp1);
    assert(sp1.get() == nullptr);
    assert(sp2.use_count() == 1);
    std::cout << "Test move shared_ptr passed!" << std::endl;
}

void test_shared_swap() {
    my_shared_ptr<int> sp1(new int(42));
    my_shared_ptr<int> sp2(new int(24));

    sp1.swap(sp2);

    assert(*sp1 == 24);
    assert(*sp2 == 42);
    std::cout << "Test shared_ptr swap passed!" << std::endl;
}

void test_weak_creation_and_use_count() {
    my_shared_ptr<int> sp1(new int(42));
    my_weak_ptr<int> wp1(sp1);
    assert(sp1.use_count() == 1);
    assert(*sp1 == 42);
    std::cout << "Test creation and use count passed!" << std::endl;
}

void test_weak_copying() {
    my_shared_ptr<int> sp1(new int(42));
    my_weak_ptr<int> wp1 = sp1;
    my_weak_ptr<int> wp2 = wp1;

    assert(!wp1.expired());
    assert(!wp2.expired());

    assert(wp1.lock().get() == wp2.lock().get());
    std::cout << "Test copying weak_ptr passed!" << std::endl;
}

void test_weak_lock() {
    my_shared_ptr<int> sp1(new int(42));
    my_weak_ptr<int> wp1 = sp1;
    wp1 = sp1;

    assert(!wp1.expired());

    if (auto sp2 = wp1.lock()) {
        assert(sp2.use_count() == 2);
        assert(*sp2 == 42);
    }
    else {
        assert(false);
    }

    sp1.reset();

    assert(wp1.expired());
    assert(wp1.lock().get() == nullptr);
    std::cout << "Test weak_ptr lock passed!" << std::endl;
}

void test_weak_moving() {
    my_shared_ptr<int> sp1(new int(42));
    my_weak_ptr<int> wp1 = sp1;

    my_weak_ptr<int> wp2 = std::move(wp1);
    assert(wp1.lock().get() == nullptr);
    assert(!wp2.expired());
    assert(*wp2.lock() == 42);

    std::cout << "Test moving weak_ptr passed!" << std::endl;
}

void test_weak_expired() {
    my_shared_ptr<int> sp1(new int(42));
    my_weak_ptr<int> wp1 = sp1;

    sp1.reset();
    assert(wp1.expired());
    assert(wp1.lock().get() == nullptr);

    std::cout << "Test expired weak_ptr passed!" << std::endl;
}

void test_weak_with_multiple_shared() {
    my_shared_ptr<int> sp1(new int(42));
    my_weak_ptr<int> wp1 = sp1;

    my_shared_ptr<int> sp2 = sp1;
    assert(!wp1.expired());

    sp1.reset();
    assert(!wp1.expired());

    sp2.reset();
    assert(wp1.expired());
    assert(wp1.lock().get() == nullptr);

    std::cout << "Test weak_ptr with multiple shared_ptrs passed!" << std::endl;
}

void test_comparing_weak_ptrs() {
    my_shared_ptr<int> sp1(new int(42));
    my_weak_ptr<int> wp1 = sp1;
    my_weak_ptr<int> wp2 = sp1;
    my_shared_ptr<int> sp2(new int(24));
    my_weak_ptr<int> wp3 = sp2;

    assert(wp1 == wp2);
    assert(wp1 != wp3);

    std::cout << "Test comparing weak_ptrs passed!" << std::endl;
}

int main() {
    test_shared_creation_and_use_count();
    test_shared_copying();
    test_shared_reset_and_use_count();
    test_shared_move();
    test_shared_swap();
    test_weak_creation_and_use_count();
    test_weak_copying();
    test_weak_lock();
    test_weak_moving();
    test_weak_expired();
    test_weak_with_multiple_shared();
    test_comparing_weak_ptrs();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
