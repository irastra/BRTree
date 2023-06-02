#pragma once
#include <iostream>
namespace Util{
    template <typename T>
    void print(T obj){
        std::cout.width(4);
        std::cout << obj;
    }
}