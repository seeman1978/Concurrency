//
// Created by 王强 on 2021/1/27.
//

#ifndef CONCURRENCY_THREADSAFE_STACK_H
#define CONCURRENCY_THREADSAFE_STACK_H

#include <exception>
#include <memory>  // For std::shared_ptr<>
#include <stack>
#include <mutex>

//调用者可以参考[[maybe_unused]]

struct empty_stack final : std::exception
{
    [[nodiscard]] const char* what() const noexcept override;
};

template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack():m_data{std::stack<T>()}{

    };
    threadsafe_stack(const threadsafe_stack& other){
        //这个时候新的对象还没有构建出来，所以m_data还不用保护，只保护other中的m_data即可
        std::lock_guard<std::mutex> lock(other.m_mutex);
        m_data = other.m_data;  /// 在构造函数体中的执行拷贝
    };
    threadsafe_stack& operator=(const threadsafe_stack&) = delete; // 1 赋值操作被删除
    void push(T new_value){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.push(new_value);
    };
    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_data.empty()){// 在调用pop前，检查栈是否为空
            throw empty_stack();
        }
        //这个地方 加const是什么意思？？？
        std::shared_ptr<T> const res(std::make_shared<T>(m_data.top())); // 在修改堆栈前，分配出返回值
        m_data.pop();
        return res;
    };
    void pop(T& value){
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_data.empty()){// 在调用pop前，检查栈是否为空
            throw empty_stack();
        }
        value = m_data.top();
        m_data.pop();
    };
    bool empty() const{
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data.empty();
    };

private:
    std::stack<T> m_data;
    mutable std::mutex m_mutex;
};


#endif //CONCURRENCY_THREADSAFE_STACK_H
