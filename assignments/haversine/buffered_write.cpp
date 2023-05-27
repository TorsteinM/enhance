#include <mutex>
#include <condition_variable>

template <typename T>
class circular_buffer {
public:
    explicit circular_buffer(size_t size) :
        buf_(std::unique_ptr<T[]>(new T[size])),
        max_size_(size)
    {}

    void put(T item) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_full_.wait(lock, [this](){return count_ != max_size_; });
        
        buf_[head_] = item;
        if (full_) {
            tail_ = (tail_ + 1) % max_size_;
        }
        head_ = (head_ + 1) % max_size_;
        full_ = head_ == tail_;
        count_++;
        lock.unlock();
        cond_empty_.notify_one();
    }

    T get() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_empty_.wait(lock, [this](){return count_ != 0; });
        
        auto val = buf_[tail_];
        full_ = false;
        tail_ = (tail_ + 1) % max_size_;
        count_--;
        lock.unlock();
        cond_full_.notify_one();

        return val;
    }

    bool empty() const {
        return !full_ && (head_ == tail_);
    }

    bool full() const {
        return full_;
    }

    size_t capacity() const {
        return max_size_;
    }

    size_t size() const {
        return size_;
    }

private:
    std::mutex mutex_;
    std::condition_variable cond_full_;
    std::condition_variable cond_empty_;
    std::unique_ptr<T[]> buf_;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t max_size_;
    bool full_ = 0;
    size_t count_ = 0;
};
