// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_STACKS_HPP_
#define INCLUDE_STACKS_HPP_


#include <cstddef>
#include <utility>
#include <stdexcept>

template<typename T>
class nocopy_stack {
private:
    class StackElement {
    public:
        T &get_data() {
            return _data;
        }

        void set_data(T &data) {
            _data = data;
        }

        void set_data(T &&data) {
            _data = std::move(data);
        }

        StackElement *get_next() {
            return _next;
        }

        void add_next(StackElement *next) {
            _next = next;
        }

    private:
        T _data;
        StackElement *_next = nullptr;
    };

    StackElement *_head;

public:
    nocopy_stack() {
        _head = nullptr;
    }

    nocopy_stack(const nocopy_stack &copy) = delete;

    nocopy_stack(nocopy_stack &&object) noexcept {
        _head = object._head;
        object._head = nullptr;
    }

    ~nocopy_stack() {
        while (_head != nullptr) {
            auto temp = _head->get_next();
            delete _head;
            _head = temp;
        }
    }

    void push(T &&value) {
        auto *element = new StackElement;

        element->add_next(_head);
        element->set_data(std::move(value));

        _head = element;
    }

    void push(const T &value) {
        StackElement *element = new StackElement;
        element->add_next(_head);
        element->set_data(value);
        _head = element;
    }

    void pop() {
        if (_head == nullptr)
            throw std::range_error("Stack is empty");

        auto temp = _head->get_next();

        delete _head;
        _head = temp;
    }

    const T &head() const {
        return _head->get_data();
    }
};

template<typename T>
class handicap_stack {
private:
    class StackElement {
    public:
        T _data;
        StackElement *_next;

        StackElement() {
            _next = 0;
        }
    };

    StackElement *_head;

public:
    handicap_stack() {
        if ((std::is_copy_assignable<T>::value ||
             std::is_copy_constructible<T>::value)
            || !(std::is_move_assignable<T>::value
                 && std::is_move_constructible<T>::value))
            throw std::invalid_argument("Type is not supported");

        _head = nullptr;
    }

    handicap_stack(handicap_stack &&object) noexcept {
        _head = object._head;
        object._head = nullptr;
    }

    ~handicap_stack() {
        while (_head != nullptr) {
            auto temp = _head->_next;

            delete _head;
            _head = temp;
        }
    }

    template<typename ... Args>
    void push_emplace(Args &&... value) {
        auto *element = new StackElement;

        element->_data = T(value...);
        element->_next = _head;

        _head = element;
    }

    void push(T &&value) {
        auto *element = new StackElement;

        element->_next = _head;
        element->_data = std::move(value);

        _head = element;
    }

    const T &head() const {
        return _head->_data;
    }

    T pop() {
        if (_head == nullptr)
            throw std::logic_error("Stack is empty");

        auto temp = _head->_next;
        auto result = std::move(_head->_data);

        delete _head;

        _head = temp;
        return result;
    }
};


#endif //INCLUDE_STACKS_HPP_
