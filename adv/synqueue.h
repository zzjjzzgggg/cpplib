#ifndef __CONCURRENT_QUEUE__
#define __CONCURRENT_QUEUE__

#include <mutex>
#include <condition_variable>
#include <queue>

namespace syn {

template <typename T>
class SynQueue {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable item_avail_cond_, space_avail_cond_;
    size_t capacity_;

public:
    SynQueue() : capacity_(std::numeric_limits<size_t>::max()) {}
    SynQueue(const size_t capacity) : capacity_(capacity) {}
    SynQueue(const SynQueue&) = delete;
    SynQueue& operator=(const SynQueue&) = delete;

    void setCapacity(const size_t capacity) { capacity_ = capacity; }

    /**
     * The method is protected by a mutex, which is locked using a scope guard
     * 'std::unique_lock'. If the queue is empty, we wait on the condition
     * variable 'item_avail_cond_'. This releases the lock to other threads and
     * blocks until we are notified that the condition has been met. In this
     * case, the call to wait() returns and we check the while condition again.
     * This extra check is because conditions may experience spurious wakes: we
     * could be wrongly notified while the queue is still empty.
     */
    T pop() {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) item_avail_cond_.wait(mlock);
        auto val = queue_.front();
        queue_.pop();
        mlock.unlock();
        space_avail_cond_.notify_one();
        return val;
    }

    /**
     * An different version of previous pop().
     */
    void pop(T& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) item_avail_cond_.wait(mlock);
        item = queue_.front();
        queue_.pop();
        mlock.unlock();
        space_avail_cond_.notify_one();
    }

    /**
     * Try to pop an item from the queue. Return False if no item is available.
     * (It is useful in the case of no producer threads.)
     */
    bool tryPop(T& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        if (queue_.empty()) return false;
        item = queue_.front();
        queue_.pop();
        mlock.unlock();
        space_avail_cond_.notify_one();
        return true;
    }

    /**
     * Push an item into the queue. If there is no available space, the thread
     * blocks and wait for condition 'space_avail_cond_'. The thread resumes
     * once the condition is met.
     */
    void push(const T& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.size() >= capacity_) space_avail_cond_.wait(mlock);
        queue_.push(item);
        // unlock before notificiation to minimize mutex contention
        mlock.unlock();
        // notify one waiting thread
        item_avail_cond_.notify_one();
    }

    bool tryPush(const T& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        if (queue_.size() >= capacity_) return false;
        queue_.push(item);
        // unlock before notificiation to minimize mutex contention
        mlock.unlock();
        // notify one waiting thread
        item_avail_cond_.notify_one();
    }

    /**
     * Inefficiency. Don't use this.
     */
    bool empty() {
        std::unique_lock<std::mutex> mlock(mutex_);
        return queue_.empty();
    }

    const size_t getSize() const { return queue_.size(); }
};
}  // namespace syn
#endif
