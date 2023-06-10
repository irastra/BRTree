#pragma once
#include <iostream>
namespace Util{

    int width = 4;

    template <typename T>
    void print(T obj){
        std::cout.width(width);
        std::cout << obj;
    }
    template <typename T>
    void print_fill(T obj) {
        for (int i = 0; i < width; i++) {
            cout << obj;
        }
    }
}