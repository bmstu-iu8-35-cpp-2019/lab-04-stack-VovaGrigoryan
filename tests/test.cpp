// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>

#include <stacks.hpp>

class handicap_int {
public:
    int _number;

    handicap_int(const handicap_int &smth) = delete;

    handicap_int() {
        _number = 0;
    }

    explicit handicap_int(const int &num) {
        _number = num;
    }

    handicap_int(handicap_int &&rth) noexcept {
        _number = rth._number;
        rth._number = 0;
    }

    handicap_int &operator=(handicap_int &&rth) noexcept {
        _number = rth._number;
        rth._number = 0;
        return *this;
    }

    bool operator==(const handicap_int rhs) const {
        return _number == rhs._number;
    }
};

TEST(nocopy_stack, basics) {
    nocopy_stack<int> stack;
    stack.push(1);

    EXPECT_EQ(stack.head(), 1);
    stack.push(2);
    stack.push(3);

    stack.pop();
    EXPECT_EQ(stack.head(), 2);
}

TEST(nocopy_stack, no_copy) {
    nocopy_stack<int> stack;
    stack.push(1);
    //EXPECT_ANY_THROW(nocopy_stack<int> stack2(stack));
    // nope, compile time error
}

TEST(nocopy_stack, movable) {
    nocopy_stack<int> stack;
    stack.push(1);

    EXPECT_NO_THROW(nocopy_stack<int> stack0(std::move(stack)));

    stack.push(1);

    nocopy_stack<int> stack2(std::move(stack));
    EXPECT_EQ(stack2.head(), 1);
}

TEST(handicap_stack, datatypes) {
    EXPECT_ANY_THROW(handicap_stack<int> x);
    EXPECT_ANY_THROW(handicap_stack<char> x);

    EXPECT_NO_THROW(handicap_stack<handicap_int> x);
}

TEST(handicap_stack, emplace) {
    handicap_stack<handicap_int> stack;
    stack.push_emplace(1);

    EXPECT_TRUE(stack.head() == handicap_int(1));
}

TEST(handicap_stack, basics) {
    handicap_stack<handicap_int> stack;
    stack.push_emplace(1);

    EXPECT_TRUE(stack.head() == handicap_int(1));
    stack.push_emplace(2);
    stack.push_emplace(3);

    stack.pop();
    EXPECT_TRUE(stack.head() == handicap_int(2));
}

TEST(handicap_stack, no_copy) {
    handicap_stack<handicap_int> stack;
    stack.push(handicap_int(1));
    //EXPECT_ANY_THROW(nocopy_stack<int> stack2(stack));
    // nope, compile time error
}

TEST(handicap_stack, movable) {
    handicap_stack<handicap_int> stack;
    stack.push(handicap_int(1));

    EXPECT_NO_THROW(handicap_stack<handicap_int> stack0(std::move(stack)));

    stack.push(handicap_int(1));

    handicap_stack<handicap_int> stack2(std::move(stack));
    EXPECT_TRUE(stack2.head() == handicap_int(1));
}
