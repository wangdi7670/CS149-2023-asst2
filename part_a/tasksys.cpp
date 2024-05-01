#include "tasksys.h"
#include <thread>
#include <assert.h>
#include <queue>

IRunnable::~IRunnable() {}

ITaskSystem::ITaskSystem(int num_threads) : num_threads_(num_threads) {}

ITaskSystem::~ITaskSystem() {}


/*
 * ================================================================
 * Serial task system implementation
 * ================================================================
 */

const char* TaskSystemSerial::name() {
    return "Serial";
}

TaskSystemSerial::TaskSystemSerial(int num_threads): ITaskSystem(num_threads) {
}

TaskSystemSerial::~TaskSystemSerial() {}

void TaskSystemSerial::run(IRunnable* runnable, int num_total_tasks) {
    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }
}

TaskID TaskSystemSerial::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                          const std::vector<TaskID>& deps) {
    // You do not need to implement this method.
    return 0;
}

void TaskSystemSerial::sync() {
    // You do not need to implement this method.
    return;
}

/*
 * ================================================================
 * Parallel Task System Implementation
 * ================================================================
 */

const char* TaskSystemParallelSpawn::name() {
    return "Parallel + Always Spawn";
}

TaskSystemParallelSpawn::TaskSystemParallelSpawn(int num_threads): ITaskSystem(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
}

TaskSystemParallelSpawn::~TaskSystemParallelSpawn() {}

void runTaskParallelSpawn(ThreadArg *arg) {
    assert(arg != nullptr && arg->runnable_ != nullptr && arg->mutex_ != nullptr);

    int i;

    arg->mutex_->lock();
    while (arg->next_task_id_ < arg->num_total_tasks_) {
        i = arg->next_task_id_;
        arg->next_task_id_++;
        arg->mutex_->unlock();

        arg->runnable_->runTask(i, arg->num_total_tasks_);
        arg->mutex_->lock();
    }
    arg->mutex_->unlock();
}

void TaskSystemParallelSpawn::run(IRunnable* runnable, int num_total_tasks) {
    //
    // TODO: CS149 students will modify the implementation of this
    // method in Part A.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

    /* for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    } */

    ThreadArg *arg = new ThreadArg(num_total_tasks, runnable);
    std::thread *threads = new std::thread[num_threads_];

    for (int i = 0; i < num_threads_; i++) {
        threads[i] = std::thread(runTaskParallelSpawn, arg);
    } 

    for (int i = 0; i < num_threads_; i++) {
        threads[i].join();
    }

    delete [] threads;
    delete arg;
}

TaskID TaskSystemParallelSpawn::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                 const std::vector<TaskID>& deps) {
    // You do not need to implement this method.
    return 0;
}

void TaskSystemParallelSpawn::sync() {
    // You do not need to implement this method.
    return;
}

/*
 * ================================================================
 * Parallel Thread Pool Spinning Task System Implementation
 * ================================================================
 */

const char* TaskSystemParallelThreadPoolSpinning::name() {
    return "Parallel + Thread Pool + Spin";
}



TaskSystemParallelThreadPoolSpinning::TaskSystemParallelThreadPoolSpinning(int num_threads): ITaskSystem(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
    threads_ = new std::thread[num_threads_];
}

TaskSystemParallelThreadPoolSpinning::~TaskSystemParallelThreadPoolSpinning() {
    delete [] threads_;
}

void TaskSystemParallelThreadPoolSpinning::runTask(ThreadArg *arg) {
    assert(arg != nullptr && arg->runnable_ != nullptr && arg->mutex_ != nullptr);

    int i;

    arg->mutex_->lock();
    while (arg->next_task_id_ < arg->num_total_tasks_) {
        i = arg->next_task_id_;
        arg->next_task_id_++;
        arg->mutex_->unlock();

        arg->runnable_->runTask(i, arg->num_total_tasks_);
        arg->mutex_->lock();
    }
    arg->mutex_->unlock();
}

void TaskSystemParallelThreadPoolSpinning::run(IRunnable* runnable, int num_total_tasks) {
    //
    // TODO: CS149 students will modify the implementation of this
    // method in Part A.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

    /* for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    } */

    ThreadArg *arg = new ThreadArg(num_total_tasks, runnable);

    for (int i = 0; i < num_threads_; i++) {
        threads_[i] = std::thread(runTask, arg);
    }

    for (int i = 0; i < num_threads_; i++) {
        threads_[i].join();
    }

    delete arg;
}

TaskID TaskSystemParallelThreadPoolSpinning::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                              const std::vector<TaskID>& deps) {
    // You do not need to implement this method.
    return 0;
}

void TaskSystemParallelThreadPoolSpinning::sync() {
    // You do not need to implement this method.
    return;
}

/*
 * ================================================================
 * Parallel Thread Pool Sleeping Task System Implementation
 * ================================================================
 */

const char* TaskSystemParallelThreadPoolSleeping::name() {
    return "Parallel + Thread Pool + Sleep";
}

TaskSystemParallelThreadPoolSleeping::TaskSystemParallelThreadPoolSleeping(int num_threads): ITaskSystem(num_threads) {
    //
    // TODO: CS149 student implementations may decide to perform setup
    // operations (such as thread pool construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
}

TaskSystemParallelThreadPoolSleeping::~TaskSystemParallelThreadPoolSleeping() {
    //
    // TODO: CS149 student implementations may decide to perform cleanup
    // operations (such as thread pool shutdown construction) here.
    // Implementations are free to add new class member variables
    // (requiring changes to tasksys.h).
    //
}

void TaskSystemParallelThreadPoolSleeping::run(IRunnable* runnable, int num_total_tasks) {


    //
    // TODO: CS149 students will modify the implementation of this
    // method in Parts A and B.  The implementation provided below runs all
    // tasks sequentially on the calling thread.
    //

    for (int i = 0; i < num_total_tasks; i++) {
        runnable->runTask(i, num_total_tasks);
    }
}

TaskID TaskSystemParallelThreadPoolSleeping::runAsyncWithDeps(IRunnable* runnable, int num_total_tasks,
                                                    const std::vector<TaskID>& deps) {


    //
    // TODO: CS149 students will implement this method in Part B.
    //

    return 0;
}

void TaskSystemParallelThreadPoolSleeping::sync() {

    //
    // TODO: CS149 students will modify the implementation of this method in Part B.
    //

    return;
}
