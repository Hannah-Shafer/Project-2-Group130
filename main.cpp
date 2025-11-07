#include "hashlist.h"
#include <iostream>


int main() {
    std::string file = "../words_alpha.txt";
    Hash hash = build(file);
    std::string input;
    while (true) {
        std::cout << "\nEnter letters to unscramble (or 'q' to quit): ";
        std::cin >> input;

        if (input == "q") {
            break;
        }

        std::vector<std::string> words = findwords(input, hash);

        if (!words.empty()) {
            std::cout << "Found " << words.size() << " word(s):" << std::endl;
            for (const std::string& word : words) {
                std::cout << "  " << word << std::endl;
            }
        } else {
            std::cout << "No matching words found" << std::endl;
        }
    }

    std::cout << "Goodbye!" << std::endl;
    return 0;


}



//
// Created by Kai Patel on 11/3/25.
//
