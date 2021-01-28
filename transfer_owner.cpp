//
// Created by 王强 on 2021/1/28.
//
#include <mutex>
#include <iostream>
std::mutex some_mutex;
std::unique_lock<std::mutex> get_lock()
{
    std::unique_lock<std::mutex> lk(some_mutex);
    return lk;  // 1
}

void do_something(){
    std::cout << "do something" << '\n';
}

void process_data()
{
    std::unique_lock<std::mutex> lk(get_lock());  // 2
    do_something();
}

int main(){
    process_data();
}