//
// Created by 王强 on 2021/1/31.
//
#include <thread>
#include <queue>

template<typename T>
class threadsafe_queue{
public:
    threadsafe_queue()=default;
    threadsafe_queue(const threadsafe_queue&)=default;
    threadsafe_queue& operator=(const threadsafe_queue&) = delete;

    void push(T new_value){
        std::lock_guard lk(m_mutex);
        m_queue.push(new_value);
        m_cv.notify_one();
    };
    bool try_pop(T& value){
        std::unique_lock lk(m_mutex);
        if (m_queue.empty()){
            return false;
        }

        value = m_queue.front();
        m_queue.pop();
        return true;
    };
    std::shared_ptr<T> try_pop(){
        std::unique_lock lk(m_mutex);
        if (m_queue.empty()){
            return nullptr;
        }

        std::shared_ptr<T> const res{std::make_shared<T>(m_queue.front())}; // 在修改堆栈前，分配出返回值
        m_queue.pop();
        return res;
    };
    void wait_and_pop(T& value){
        std::unique_lock lk(m_mutex);
        m_queue.wait(lk, [this]{return !m_queue.empty();});
        value = m_queue.front();
        m_queue.pop();
    };
    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock lk(m_mutex);
        m_queue.wait(lk, [this]{return !m_queue.empty();});
        std::shared_ptr<T> const res{std::make_shared<T>(m_queue.front())}; // 在修改堆栈前，分配出返回值
        m_queue.pop();
        return res;
    };
    bool empty() const{
        std::lock_guard lk(m_mutex);
        return m_queue.empty();
    };

private:
    std::mutex m_mutex;
    std::queue<T> m_queue;
    std::condition_variable m_cv;
};
