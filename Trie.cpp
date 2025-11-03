#include "trie.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
TrieNode::TrieNode(){
    isWord = false; //base case is not a word yet
    for (int i = 0; i < 26; i++){ //setting the base case as all letters being null
        children[i] = nullptr;
    }
}
Trie::Trie(){
  root = new TrieNode();
}
void deleteNode(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < 26; i++)
        deleteNode(node->children[i]);
    delete node;
}
Trie::~Trie(){
    deleteNode(root);
}
string Trie::toLowercase(const string& str) {
    string lower;
    for (int i = 0; i < str.size(); i++) {
        char c = str[i];
        if (c >= 'A' && c <= 'Z')
            c = c - 'A' + 'a';
        lower.push_back(c);
    }
    return lower;
}
bool Trie::isValidInput(const string& str) {
    for (int i = 0; i < str.size(); i++) {
        char c = str[i];
        if (!isalpha(c)) { //no special characters
            return false;
        }
    }
    return true;
}
void Trie::insertWord(const string& word){
    string lowerWord = toLowercase(word); // convert entire word to lowercase
    TrieNode* node = root;
    for (int i = 0; i < lowerWord.size(); i++) {
        char c = lowerWord[i];
        int index = c - 'a';
        if (node->children[index] == nullptr) //if that letter doesnt already exist, add it
            node->children[index] = new TrieNode();
        node = node->children[index];
    }
    node->isWord = true;
}
void Trie::searchHelper(TrieNode* node, string& path, vector<bool>& used, const string& letters, set<string>& results) {
  if (node->isWord && path.size() == letters.size()) {
      results.insert(path); //checks if its a word and if it uses all the letters to insert it
  }
  for (int i = 0; i < letters.length(); i++){
      if (!used[i]) { //if the letter in that position has not been checked yet
          int index = letters[i] - 'a';
          if (node->children[index] != nullptr) { //does the letter have any other of the letters in the trie after it
              used[i] = true; //mark the letter as used
              path.push_back(letters[i]); //add the letter to the word
              searchHelper(node->children[index], path, used, letters, results); //recursion
              path.pop_back(); //remove the letter after the recursion
              used[i] = false; //mark the letter unused so it can be tested in other words
          }
      }
  }
}
void Trie::buildTrie(const vector<string>& words) {
    for (int i = 0; i < words.size(); i++){
        insertWord(words[i]);
        }
}
set<string> Trie::searchAnagrams(const string& letters) {
    set<string> results; //create a string to store the words found
    if (!isValidInput(letters)) {
        results.insert("_"); //returns a set that has this so that way it doesnt print out the no anagrams function
        return results;
    }
    string lowerLetters = toLowercase(letters); // convert to lowercase
    string path; //the place for creating a word to see if it works
    vector<bool> used(lowerLetters.size(), false); //keep track of letters used
    searchHelper(root, path, used, lowerLetters, results); //start the recursion for all the letters
    return results; //return the words found
}
void Trie::printout(set<string>& results, const string& letters) {
    if (results.count("_")) {
        cout << "Error: Input contains invalid characters."<<endl;
        return;
    }
    if (results.empty()) { //if there are no anagrams
        cout << "No anagrams found for '" << letters << "'." << endl;
        return;
    }
    cout << "Anagrams of '" << letters << "': ";
    for (auto it = results.begin(); it != results.end(); ++it)
        cout << *it << " ";
    cout << endl;
}