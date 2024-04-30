#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <assert.h>
#include <pthread.h>

using namespace std;

struct Barrier{
    int num_threads;
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    int round;
} barrier;

void initBarrier(int nthreads) {
    barrier.count = 0;
    barrier.round = 0;
    barrier.num_threads = nthreads;
}

typedef struct {
    int id;
    int rounds;
} ThreadArgs;

/**
 * every thread's round is sync with barrier.round
 */
void bar(ThreadArgs *args) {

    for (int i = 0; i < args->rounds; i++) {

        std::unique_lock<std::mutex> lk(barrier.mtx);
        std::cout << "thread"<< args->id << " output " << i << "\n";

        assert(barrier.round == i);

        barrier.count++;

        if (barrier.count == barrier.num_threads) {
            barrier.round++;
            barrier.count = 0;
            barrier.cv.notify_all();

        } else if (barrier.count < barrier.num_threads) {
            barrier.cv.wait(lk);
        } else {
            throw exception();
        }

        lk.unlock();
    }
}

void test_barrier() {
    int nthreads = 3;
    int nround = 5;

    initBarrier(nthreads);

    std::thread threads[nthreads];
    ThreadArgs args[nthreads];

    for (int i = 0; i < nthreads; i++) {
        args[i].id = i;
        args[i].rounds = nround;
        threads[i] = std::thread(bar, &args[i]);
    }

    for (int i = 0; i < nthreads; i++) {
        threads[i].join();
    }
    std::cout << "Passed=====" << "\n";
}


void test1() {
    std::string str1 = "hello11";
    std::string str2 = "hello11";
    int equal = str1.compare(str2);
    std::cout << equal << "\n";
}

void helper2(int i) {
    std::cout << "haha: " << i << "\n";
}

void test2() {
    std::thread thd;
    thd = std::thread(helper2, 1);
    thd.join();

    std::cout << "main" << "\n";

    thd = std::thread(helper2, 2);
    thd.join();
}

void help3() {
    std::cout << "help3 start" << "\n";
    // std::terminate();
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "help3 end" << "\n";
}

void test3() {
    std::thread thd;
    thd = std::thread(help3);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    if (thd.joinable()) {
        std::cout << "thd is joinable" << "\n";
        thd.join();
        assert(!thd.joinable());
    } else {
        std::cout << "not joinable" << "\n";
    }

}

int main(int argc, char* argv[]) {
    test3();
    return 0;
}