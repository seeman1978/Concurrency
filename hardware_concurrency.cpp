//
// Created by 王强 on 2021/1/27.
//

#include <numeric>
#include <thread>
#include <vector>
#include <iostream>

///运行时决定线程数量

template<typename Iterator, typename T>
struct accumulate_block{
    void operator()(Iterator first, Iterator last, T& result){
        result = std::accumulate(first, last , result);
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init){
    unsigned long const length = std::distance(first, last);
    if (length == 0){
        return init;
    }

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length+min_per_thread-1)/min_per_thread;

    unsigned long const hardware_threads =
            std::thread::hardware_concurrency();
    unsigned long const num_threads =
            std::min(hardware_threads!=0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length/num_threads;

    std::vector<T> results(num_threads);

    std::vector<std::thread> threads;
    threads.reserve(num_threads-1); //有一个计算可以在主线程里做

    Iterator block_start = first;
    for (unsigned long i=0; i<(num_threads-1); ++i){
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads.emplace_back(std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i])));
        block_start = block_end;
    }

    accumulate_block<Iterator, T>()(block_start, last, results[num_threads-1]);

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(results.begin(), results.end(), init);
}

int main(){
    std::vector<int> vec;
    vec.reserve(1000);
    for (int i = 1; i < 1001; ++i) {
        vec.emplace_back(i);
    }

    int result = parallel_accumulate(vec.begin(), vec.end(), 0);
    std::cout << result;
}