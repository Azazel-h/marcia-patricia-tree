//
// Created by Azazel on 21.07.2024.
//
#include "merkle-patricia-tree/trie/some.h"


namespace sm::lbr {
    void print_something() {
        std::cout << "Hello" << std::endl;
    }
}

void print_something() {
    sm::lbr::print_something();
}