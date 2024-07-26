//
// Created by Azazel on 21.07.2024.
//
#ifndef SILKWORM_TRIE_SOME_HPP
#define SILKWORM_TRIE_SOME_HPP

#ifdef __cplusplus

#include <iomanip>
#include <iostream>


namespace sm::lbr {
    void print_something();
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

void print_something() { sm::lbr::print_something(); } ;
#ifdef __cplusplus
}
#endif

#endif // SILKWORM_TRIE_SOME_HPP

