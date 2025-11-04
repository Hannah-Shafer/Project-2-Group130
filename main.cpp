#include <iostream>
#include <SFML/Graphics.hpp>
#include "hashlist.h"
#include "trie.h"
using namespace std;


int main() {
    // open words file
    ifstream wordsFile("../resources/words_alpha.txt");
    if (!wordsFile.is_open()) {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    // insert each word into vector
    std::vector<std::string> wordsVec;
    std::string line;
    while(std::getline( wordsFile, line ))
        wordsVec.push_back(std::move(line));

    // Hash hash = build("../words_alpha.txt");
    // if (hash.empty()) {
    //     std::cerr << "Error" << std::endl;
    //     return 1;
    // }

    // creating the Trie from the words file
    Trie trie;
    trie.buildTrie(wordsVec);


    int window_width = 720;
    int window_height = 850;

    // Load font and create text
    sf::Font font("../resources/Font-Sriracha.ttf");

    sf::Text TitleText(font, "Word Unscrambler!");
    TitleText.setCharacterSize(60);
    TitleText.setFillColor(sf::Color::White);
    //TitleText.setStyle(sf::Text::Bold);
    TitleText.setOrigin(TitleText.getLocalBounds().getCenter());
    TitleText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 280));

    sf::Text enterCharsText(font, "Enter a string of characters:");
    enterCharsText.setCharacterSize(35);
    enterCharsText.setFillColor(sf::Color::White);
    //enterCharsText.setStyle(sf::Text::Bold);
    enterCharsText.setOrigin(enterCharsText.getLocalBounds().getCenter());
    enterCharsText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 150));

    std::string scrambledString;
    sf::Text scrambledText(font, "|");
    scrambledText.setCharacterSize(28);
    scrambledText.setFillColor(sf::Color::Yellow);
    //scrambledText.setStyle(sf::Text::Bold);
    scrambledText.setOrigin(scrambledText.getLocalBounds().getCenter());
    scrambledText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 100));

    sf::RenderWindow appWindow(sf::VideoMode(sf::Vector2u(window_width, window_height)), "Word Unscrambler", sf::Style::Close, sf::State::Windowed);

    // window loop
    while (appWindow.isOpen()) {
        // event loop
        while (const std::optional<sf::Event> event = appWindow.pollEvent()) {
            // close event
            if (event->is<sf::Event::Closed>()) {
                appWindow.close();
                break;
            }
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                if (textEntered->unicode == 8 && !scrambledString.empty())
                    scrambledString.pop_back();
                if (isalpha(textEntered->unicode) && scrambledString.size() < 20)
                    scrambledString += tolower(static_cast<char>(textEntered->unicode));
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Enter && !scrambledString.empty()) {
                    appWindow.close();
                    break;
                }
            }

        }
        appWindow.clear(sf::Color::Black);

        // draw each text block and update input text
        appWindow.draw(TitleText);
        appWindow.draw(enterCharsText);
        scrambledText.setString(scrambledString + "|");
        scrambledText.setOrigin(scrambledText.getLocalBounds().getCenter());
        scrambledText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 100));
        appWindow.draw(scrambledText);

        appWindow.display();
    }



    string letters = "tca";
    set<string> results = trie.searchAnagrams(letters);
    trie.printout(results, letters);
    string letters2 = "gDo"; //testing not case-sensitive
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