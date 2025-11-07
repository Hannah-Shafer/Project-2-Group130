#ifndef TRIE_H
#define TRIE_H
#include <string>
#include <vector>
#include <set>
using namespace std;

struct TrieNode {
    TrieNode* children[26]; // for 'a' to 'z'
    bool isWord;
    TrieNode();
};
class Trie {
private:
    TrieNode* root;
    string toLowercase(const string& str);
    void insertWord(const string& word);
    bool isValidInput(const string& str);
    void searchHelper(TrieNode* node, string& path, vector<bool>& used, const string& letters, set<string>& results);
public:
    Trie();
    ~Trie();
    void buildTrie(const vector<string>& words);
    set<string> searchAnagrams(const string& letters);
    void printout(set<string>& results, const string& letters);
};
#endif //TRIE_H
