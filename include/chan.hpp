#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

template<class T, std::size_t N = 0>
class chan
{
    public:
        chan() {}
        int close()
        {
            int succ = 0;
            std::lock_guard<std::mutex> lock(m_mu);
            if (closed) {
                succ = -1;
                errno = EPIPE;
            } else {
                closed = true;
                r_cond.notify_all();
                w_cond.notify_all();
            }
            return succ;
        }
        int send(const T &t)
        {
            if (closed) {
                errno = EPIPE;
                return -1;
            }
            std::unique_lock<std::mutex> lock(m_mu);
            while (q.size() == capacity) {
                w_waiting++;
                w_cond.wait(lock);
                w_waiting--;
            }
            q.push(t);
            if (r_waiting > 0)
                r_cond.notify_one();

            lock.unlock();
            return 0;
        }
        int send(T &&t)
        {
            if (closed) {
                errno = EPIPE;
                return -1;
            }
            std::unique_lock<std::mutex> lock(m_mu);
            while (q.size() == capacity) {
                w_waiting++;
                w_cond.wait(lock);
                w_waiting--;
            }
            q.push(std::move(t));
            if (r_waiting > 0)
                r_cond.notify_one();

            lock.unlock();
            return 0;
        }

        chan &operator<<(T &&t)
        {
            if (closed) {
                errno = EPIPE;
            } else {

                std::unique_lock<std::mutex> lock(m_mu);
                while (q.size() == capacity) {
                    w_waiting++;
                    w_cond.wait(lock);
                    w_waiting--;
                }
                q.push(std::move(t));
                if (r_waiting > 0)
                    r_cond.notify_one();

                lock.unlock();
            }
            return *this;
        }

        int recv(T &data)
        {
            std::unique_lock<std::mutex> lock(m_mu);
            while (q.empty()) {
                if (closed) {
                    m_mu.unlock();
                    errno = EPIPE;
                    return -1;
                }

                r_waiting++;
                r_cond.wait(lock);
                r_waiting--;
            }
            data = q.front();
            q.pop();
            if (w_waiting > 0)
                w_cond.notify_one();
            lock.unlock();
            return 0;
        }
        chan &operator>>(T &t)
        {
            std::unique_lock<std::mutex> lock(m_mu);
            while (q.empty()) {
                if (closed) {
                    m_mu.unlock();
                    errno = EPIPE;
                    return *this;
                }

                r_waiting++;
                r_cond.wait(lock);
                r_waiting--;
            }
            t = q.front();
            q.pop();
            if (w_waiting > 0)
                w_cond.notify_one();
            lock.unlock();
            return *this;
        }
        int size()
        {
            std::lock_guard<std::mutex> lock(m_mu);
            return q.size();
        }
        operator bool() const
        {
            std::lock_guard<std::mutex> lock(m_mu);
            return !(closed && q.empty());
        }
    private:
        std::queue<T> q;

        std::mutex m_mu;
        std::condition_variable r_cond;
        std::condition_variable w_cond;
        std::atomic_bool closed{false};
        int r_waiting = 0;;
        int w_waiting = 0;;
        size_t capacity = N;
};

template<class T>
class chan<T, 0>
{
    public:
        chan() {}
        int close()
        {
            int succ = 0;
            std::lock_guard<std::mutex> lock(m_mu);
            if (closed) {
                succ = -1;
                errno = EPIPE;
            } else {
                closed = true;
                r_cond.notify_all();
                w_cond.notify_all();
            }
            return succ;
        }
        int send(const T &t)
        {
            std::lock_guard<std::mutex> glock(w_mu);
            std::unique_lock<std::mutex> lock(m_mu);
            if (closed) {
                lock.unlock();
                errno = EPIPE;
                return -1;
            }
            this->t = t;
            w_waiting++;

            if (r_waiting > 0)
                r_cond.notify_one();

            w_cond.wait(lock);

            lock.unlock();

            return 0;
        }

        int send(T &&t)
        {
            std::lock_guard<std::mutex> glock(w_mu);
            std::unique_lock<std::mutex> lock(m_mu);
            if (closed) {
                lock.unlock();
                errno = EPIPE;
                return -1;
            }
            this->t = std::move(t);
            w_waiting++;

            if (r_waiting > 0)
                r_cond.notify_one();

            w_cond.wait(lock);

            lock.unlock();

            return 0;
        }

        chan &operator<<(T &&t)
        {
            send(std::forward<T>(t));
            return *this;
        }

        int recv(T &data)
        {
            std::lock_guard<std::mutex> glock(r_mu);
            std::unique_lock<std::mutex> lock(m_mu);
            while (!closed && w_waiting == 0) {
                r_waiting++;
                r_cond.wait(lock);
                r_waiting--;
            }

            if (closed) {
                lock.unlock();
                errno = EPIPE;
                return -1;
            }
            data = t;
            w_waiting--;

            w_cond.notify_one();
            lock.unlock();
            return 0;
        }
        chan &operator>>(T &t)
        {
            recv(t);
            return *this;
        }
        int size()
        {
            return 0;
        }
        operator bool() const
        {
            return !closed;
        }
    private:
        T t;
        std::mutex r_mu;
        std::mutex w_mu;

        std::mutex m_mu;
        std::condition_variable r_cond;
        std::condition_variable w_cond;
        std::atomic_bool closed{false};
        int r_waiting = 0;
        int w_waiting = 0;
};
