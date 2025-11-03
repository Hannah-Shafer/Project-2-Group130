#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using Hash = std::unordered_map<std::string, std::vector<std::string>>;

std::string alphabetize(std::string word) {
    // Alphabetizes the letters
    std::sort(word.begin(), word.end());
    return word;
}


Hash build(const std::string& filename) {
    // Creates the hash list from the database
    Hash hash;
    std::ifstream file(filename);
    std::string word;

    if (!file.is_open()) {
        std::cerr << "Error" << std::endl;
        return hash; // Returns empty map
    }

    while (std::getline(file, word)) {
        if (!word.empty() && word.back() == '\r') {
            word.pop_back();
        } // Mac formatting issue
        // Lowercase all words
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        std::string key = alphabetize(word);
        hash[key].push_back(word);
        //Alphabetizes and adds to hash list keys
    }
    file.close();
    return hash;
}

// Main function must format the input into all lowercase and removes special characters
std::vector<std::string> findwords(const std::string& input, const Hash& hash) {
    std::string key = alphabetize(input);
    // Find key
    auto it = hash.find(key);
    if (it != hash.end()) {
        return it->second;
    } else {
        // No matches
        return std::vector<std::string>();
    }
}


//
// Created by Kai Patel on 11/3/25.
//

#ifndef PROJECT_2_GROUP130_HEADER_H
#define PROJECT_2_GROUP130_HEADER_H

#endif //PROJECT_2_GROUP130_HEADER_H
