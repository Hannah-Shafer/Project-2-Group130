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

    // Load font and create text / buttons
    sf::Font font("../resources/Font-Sriracha.ttf");

    sf::Text TitleText(font, "Word Unscrambler!");
    TitleText.setCharacterSize(60.f);
    TitleText.setFillColor(sf::Color::White);
    TitleText.setOrigin(TitleText.getLocalBounds().getCenter());
    TitleText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 280));

    sf::Text enterCharsText(font, "Enter a string of characters:");
    enterCharsText.setCharacterSize(35.f);
    enterCharsText.setFillColor(sf::Color::White);
    enterCharsText.setOrigin(enterCharsText.getLocalBounds().getCenter());
    enterCharsText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 150));

    std::string scrambledString;
    sf::Text scrambledText(font, "|");
    scrambledText.setCharacterSize(28.f);
    scrambledText.setFillColor(sf::Color::Yellow);
    scrambledText.setOrigin(scrambledText.getLocalBounds().getCenter());
    scrambledText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 100));

    sf::RectangleShape trieButton({250.f, 90.f});
    trieButton.setFillColor(sf::Color(25,25,25));
    trieButton.setOutlineThickness(5.f);
    trieButton.setOutlineColor(sf::Color::White);
    trieButton.setOrigin(trieButton.getLocalBounds().getCenter());
    trieButton.setPosition(sf::Vector2f(window_width/2, window_height/2 + 80));

    sf::Text trieText(font, "Search with Trie");
    trieText.setCharacterSize(28.f);
    trieText.setFillColor(sf::Color::White);
    trieText.setOrigin(trieText.getLocalBounds().getCenter());
    trieText.setPosition(trieButton.getPosition());

    sf::RectangleShape hashButton({250.f, 90.f});
    hashButton.setFillColor(sf::Color::Transparent);
    hashButton.setOutlineThickness(5.f);
    hashButton.setOutlineColor(sf::Color::White);
    hashButton.setOrigin(hashButton.getLocalBounds().getCenter());
    hashButton.setPosition(sf::Vector2f(window_width/2, window_height/2 + 220));

    sf::Text hashText(font, "Search with Hashmap");
    hashText.setCharacterSize(23.f);
    hashText.setFillColor(sf::Color::White);
    hashText.setOrigin(hashText.getLocalBounds().getCenter());
    hashText.setPosition(hashButton.getPosition());

    bool searched = false;

    sf::RenderWindow appWindow(sf::VideoMode(sf::Vector2u(window_width, window_height)), "Word Unscrambler", sf::Style::Close, sf::State::Windowed);

    // window loop
    while (appWindow.isOpen()) {
        // event loop
        while (const std::optional<sf::Event> event = appWindow.pollEvent()) {
            // close window event
            if (event->is<sf::Event::Closed>()) {
                appWindow.close();
                break;
            }
            // text key press event
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                if (textEntered->unicode == 8 && !scrambledString.empty())
                    scrambledString.pop_back();
                if (isalpha(textEntered->unicode) && scrambledString.size() < 20)
                    scrambledString += tolower(static_cast<char>(textEntered->unicode));
            }
            // other key press event
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Enter && !scrambledString.empty()) {
                    appWindow.close();
                    break;
                }
            }
            // click event (for the buttons)
            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    if (trieButton.getGlobalBounds().contains(sf::Vector2f(mouse->position))) { // click on 'search with trie'
                        searched = true;

                    }

                }
            }

        }






        appWindow.clear(sf::Color::Black);

        // update input text and draw each text block
        scrambledText.setString(scrambledString + "|");
        scrambledText.setOrigin(scrambledText.getLocalBounds().getCenter());
        scrambledText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 100));

        appWindow.draw(TitleText);
        appWindow.draw(enterCharsText);
        appWindow.draw(scrambledText);
        if (!searched) {
            appWindow.draw(trieButton);
            appWindow.draw(hashButton);
            appWindow.draw(trieText);
            appWindow.draw(hashText);
        }

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