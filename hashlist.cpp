#include "hashlist.h"
#include <iostream>  // for std::cerr
#include <fstream>   // for std::ifstream
#include <algorithm> // for std::sort, std::transform
#include <cctype>    // for std::tolower




unsigned int hashlist::hashfunc(const std::string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

// doubles table size and re-inserts all elements
void hashlist::rehash() {
    int old_capacity = capacity;
    node** old_table = table;

    capacity *= 2;
    size = 0; // gets reset as we add
    table = new node*[capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }

    // go through old table and reinsert all items
    for (int i = 0; i < old_capacity; ++i) {
        node* current = old_table[i];
        while (current != nullptr) {
            node* next = current->next;
            for (const std::string& word : current->values) {
                add(current->key, word);
            }
            delete current;
            current = next;
        }
    }
    delete[] old_table;
}

// constructor
hashlist::hashlist(int initial_capacity) : capacity(initial_capacity), size(0) {
    table = new node*[capacity];
    for (int i = 0; i < capacity; ++i) {
        table[i] = nullptr;
    }
}

// destructor
hashlist::~hashlist() {
    for (int i = 0; i < capacity; ++i) {
        node* current = table[i];
        while (current != nullptr) {
            node* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] table;
}

// adds word to the hash table
void hashlist::add(const std::string& key, const std::string& word) {
    // rehash if load factor is over 0.75
    if ((double)size / capacity > 0.75) {
        rehash();
    }

    unsigned int index = hashfunc(key);
    node* current = table[index];

    // search for the key
    while (current != nullptr) {
        if (current->key == key) {
            current->values.push_back(word);
            return;
        }
        current = current->next;
    }

    // if key not found creates new node
    node* new_node = new node(key, word);
    new_node->next = table[index];
    table[index] = new_node;
    size++;
}

// finds all anagrams for a given key
std::vector<std::string> hashlist::find(const std::string& key) const {
    unsigned int index = hashfunc(key);
    node* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            return current->values;
        }
        current = current->next;
    }

    return std::vector<std::string>();
}

// alphabetizes word's letters
std::string alphabetize(std::string word) {
    std::sort(word.begin(), word.end());
    return word;
}

// builds hash map
Hash build(const std::string& filename) {
    Hash hash;
    std::ifstream file(filename);
    std::string word;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << std::endl;
        return hash;
    }

    while (std::getline(file, word)) {
        // had to adjust for windows/mac line endings
        if (!word.empty() && word.back() == '\r') {
            word.pop_back();
        }

        // lowercase everything
        std::string cleaned_word = word;
        std::transform(cleaned_word.begin(), cleaned_word.end(), cleaned_word.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        if (cleaned_word.empty()) continue;

        std::string key = alphabetize(cleaned_word);

        hash.add(key, cleaned_word);
    }
    file.close();
    return hash;
}

// finds words
std::vector<std::string> findwords(const std::string& input, const Hash& hash) {
    // lowercase input to match keys
    std::string lower_input = input;
    std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    std::string key = alphabetize(lower_input);

    return hash.find(key);
}

//
// Created by Kai Patel on 11/7/25.
//
