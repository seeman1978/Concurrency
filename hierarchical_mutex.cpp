//
// Created by 王强 on 2021/1/28.
// 简单的层级互斥量实现

#include <mutex>
#include <iostream>
#include <thread>

class hierarchical_mutex{
public:
    explicit hierarchical_mutex(unsigned long value):
        hierarchy_value(value){

    }

    void lock(){
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }

    bool try_lock(){
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock()){
            return false;
        }
        update_hierarchy_value();
        return true;
    }

    void unlock(){
        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }
private:
    void check_for_hierarchy_violation() const{
        if (this_thread_hierarchy_value <= hierarchy_value){
            throw std::logic_error("mutex hierarchy violated");
        }
    }

    void update_hierarchy_value(){
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }
private:
    std::mutex internal_mutex;

    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value=0;

    static thread_local unsigned long this_thread_hierarchy_value;

};

thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

hierarchical_mutex high_level_mutex{10000};
hierarchical_mutex low_level_mutex{5000};
hierarchical_mutex other_mutex{100};

int do_low_level_stuff(){
    std::cout << " do_low_level_stuff " << '\n';
    return 1;
}

void do_high_level_stuff(int param){
    std::cout << " do_high_level_stuff " << '\n';
}

void do_other_stuff(){
    std::cout << " do_other_stuff " << '\n';
}

int low_level_func(){
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
    return do_low_level_stuff();
}

void high_level_func(){
    std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
    do_high_level_stuff(low_level_func());
}

void other_stuff(){
    high_level_func();
    do_other_stuff();
}

void thread_a()  // 6
{
    high_level_func();
}

void thread_b(){
    std::lock_guard<hierarchical_mutex> lk(other_mutex);
    other_stuff();
}

int main()
{
    std::thread t1{thread_a};
    t1.join();
    std::thread t2{thread_b};
    t2.join();  //这句话将产生异常
}
