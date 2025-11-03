#include "hashlist.h"


int main() {
    std::string file = "../words_alpha.txt";
    Hash hash = build(file);
    if (hash.empty()) {
        std::cerr << "Error" << std::endl;
        return 1;
    }


}



//
// Created by Kai Patel on 11/3/25.
//
