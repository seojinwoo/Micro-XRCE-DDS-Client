
#include <atomic>

class ThreadSafeBitChange {
public:
    ThreadSafeBitChange() : bits(0) {}

    void set_bit(int index) {
        std::atomic_fetch_or(&bits, 1 << index);
    }

    void clear_bit(int index) {
        std::atomic_fetch_and(&bits, ~(1 << index));
    }

    bool get_bit(int index) {
        return (bits & (1 << index)) != 0;
    }

    int get_int()
    {
        return bits;
    }

private:
    std::atomic<int> bits;
};