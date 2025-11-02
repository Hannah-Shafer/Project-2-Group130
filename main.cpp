#include <iostream>
#include "trie.h"
using namespace std;

int main() {
    vector<string> words = {"cat", "act", "tac", "dog", "god", "actor"};
    Trie trie;
    trie.buildTrie(words);

    string letters = "tca";
    set<string> results = trie.searchAnagrams(letters);
    trie.printout(results, letters);
    string letters2 = "gDo"; //testing not case sensitive
    set<string> results2 = trie.searchAnagrams(letters2);
    trie.printout(results2, letters2);
    string letters3 = "tocar"; //testing if it only printed out when it was all the letters
    set<string> results3 = trie.searchAnagrams(letters3);
    trie.printout(results3, letters3);
    string letters4 = "tocar-"; //testing if it detects special character
    set<string> results4 = trie.searchAnagrams(letters4);
    trie.printout(results4, letters4);
    string letters5 = "hi"; //testing if there is no match
    set<string> results5 = trie.searchAnagrams(letters5);
    trie.printout(results5, letters5);
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.