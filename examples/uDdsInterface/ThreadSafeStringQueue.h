#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

class ThreadSafeStringQueue {
public:
    void push(const std::string& str) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(str);
        cond_.notify_one();
    }

    std::string pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !queue_.empty(); });
        std::string str = queue_.front();
        queue_.pop();
        return str;
    }

    size_t count() const {
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    std::queue<std::string> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};