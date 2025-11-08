#include <string>
#include <vector>



class hashlist {
private:
    // node for separate chaining
    struct node {
        std::string key;
        std::vector<std::string> values;
        node* next;

        node(std::string k, std::string v) : key(k), values({v}), next(nullptr) {}
    };

    node** table;
    int capacity;
    int size;

    // private helper functions
    unsigned int hashfunc(const std::string& key) const;
    void rehash();

public:
    // constructor
    hashlist(int initial_capacity = 500000);

    // destructor
    ~hashlist();

    // adds a word to the hash table
    void add(const std::string& key, const std::string& word);

    // finds all anagrams for a given key
    std::vector<std::string> find(const std::string& key) const;
};

using Hash = hashlist;



// creates a key by sorting the word's letters
std::string alphabetize(std::string word);

// reads dictionary and builds the hash map
Hash build(const std::string& filename);

// finds words in hash table
std::vector<std::string> findwords(const std::string& input, const Hash& hash);

//
// Created by Kai Patel on 11/3/25.
//

#ifndef PROJECT_2_GROUP130_HEADER_H
#define PROJECT_2_GROUP130_HEADER_H

#endif //PROJECT_2_GROUP130_HEADER_H
