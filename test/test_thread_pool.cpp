#include <iostream>
#include <chrono>

#include "../adv/thread_pool.h"

class A {
public:
    std::vector<int> vec;

public:
    //! Default constructor
    A() {}

    //! Copy constructor
    A(const A &other) : vec(other.vec) { std::cout << "copy" << std::endl; }

    //! Move constructor
    A(A &&other) : vec(std::move(other.vec)) { std::cout << "move" << std::endl; }

    //! Copy assignment operator
    A &operator=(const A &other) {
        vec = other.vec;
        std::cout << "copy assign" << std::endl;
        return *this;
    }

    //! Move assignment operator
    A &operator=(A &&other) {
        vec = std::move(other.vec);
        std::cout << "move assign" << std::endl;
        return *this;
    }
};

void job(const A &a) { std::cout << "in: " << a.vec.size() << std::endl; }

int main(int argc, char *argv[]) {
    ThreadPool pool(2);

    A a;
    a.vec.push_back(1);

    // job(std::move(a));

    // [a{std::move(a)}]() { job(a); };

    // return 0;

    auto result = pool.enqueue([](const A &a) { job(a); }, std::move(a));

    result.get();

    std::cout << "out: " << a.vec.size() << std::endl;

    return 0;
}
