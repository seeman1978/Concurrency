#include <iostream>
#include <sstream>
#include "threadsafe_stack.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    std::ostringstream o;
    threadsafe_stack<std::string> mystack;
    for (int i = 0; i < 10; ++i) {
        o.str("");  //清空以前的缓存
        o<< "Hello" << i;
        mystack.push(o.str());
    }

    for (int i = 0; i < 10; ++i){
        std::cout << *mystack.pop() << std::endl;
    }
    return 0;
}
