#include <chrono>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "hashlist.h"
#include "trie.h"


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

    wordsFile.close();

    // creating the Trie from the words file
    Trie trie;
    trie.buildTrie(wordsVec);

    // creating the Hashmap from the words file
    hashlist hash = build("../resources/words_alpha.txt");


    int window_width = 720;
    int window_height = 850;

    // Load font and create text / buttons
    sf::Font font("../resources/Sriracha-Regular.ttf");

    sf::Text titleText(font, "Word Unscrambler!", 60.f);
    titleText.setFillColor(sf::Color::White);
    titleText.setOrigin(titleText.getLocalBounds().getCenter());
    titleText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 320));

    sf::Text subtitleText(font, "Solve any anagram", 32.f);
    subtitleText.setFillColor(sf::Color::White);
    subtitleText.setOrigin(subtitleText.getLocalBounds().getCenter());
    subtitleText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 260));

    sf::Text enterCharsText(font, "Enter a string of characters:", 35.f);
    enterCharsText.setFillColor(sf::Color::White);
    enterCharsText.setOrigin(enterCharsText.getLocalBounds().getCenter());
    enterCharsText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 170));

    sf::Text scrambledText(font, "|", 28.f);
    scrambledText.setFillColor(sf::Color::Yellow);
    scrambledText.setOrigin(scrambledText.getLocalBounds().getCenter());
    scrambledText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 120));

    sf::RectangleShape trieButton({250.f, 90.f});
    trieButton.setFillColor(sf::Color::Transparent);
    trieButton.setOutlineThickness(5.f);
    trieButton.setOutlineColor(sf::Color::White);
    trieButton.setOrigin(trieButton.getLocalBounds().getCenter());
    trieButton.setPosition(sf::Vector2f(window_width/2, window_height/2 + 80));

    sf::Text trieText(font, "Search with Trie", 28.f);
    trieText.setFillColor(sf::Color::White);
    trieText.setOrigin(trieText.getLocalBounds().getCenter());
    trieText.setPosition(trieButton.getPosition());

    sf::RectangleShape hashButton({250.f, 90.f});
    hashButton.setFillColor(sf::Color::Transparent);
    hashButton.setOutlineThickness(5.f);
    hashButton.setOutlineColor(sf::Color::White);
    hashButton.setOrigin(hashButton.getLocalBounds().getCenter());
    hashButton.setPosition(sf::Vector2f(window_width/2, window_height/2 + 220));

    sf::Text hashText(font, "Search with Hashmap", 23.f);
    hashText.setFillColor(sf::Color::White);
    hashText.setOrigin(hashText.getLocalBounds().getCenter());
    hashText.setPosition(hashButton.getPosition());

    sf::Text anagramsFor_Text(font, "Anagrams for", 35.f);
    anagramsFor_Text.setFillColor(sf::Color::White);
    anagramsFor_Text.setOrigin(anagramsFor_Text.getLocalBounds().getCenter());
    anagramsFor_Text.setPosition(sf::Vector2f(window_width/2, window_height/2 - 250));

    sf::Text durationText(font, "Time elapsed using", 25.f);
    durationText.setFillColor(sf::Color::White);
    durationText.setOrigin(durationText.getLocalBounds().getCenter());
    durationText.setPosition(sf::Vector2f(window_width/2, window_height - 40));

    sf::RectangleShape resetButton({100.f, 60.f});
    resetButton.setFillColor(sf::Color(25,25,25));
    resetButton.setOutlineThickness(5.f);
    resetButton.setOutlineColor(sf::Color::White);
    resetButton.setOrigin(resetButton.getLocalBounds().getCenter());
    resetButton.setPosition(sf::Vector2f(window_width/2, window_height - 100));

    sf::Text resetText(font, "RESET", 25.f);
    resetText.setFillColor(sf::Color::White);
    resetText.setOrigin(resetText.getLocalBounds().getCenter());
    resetText.setPosition(resetButton.getPosition());

    std::string scrambledString;
    bool searched = false;
    bool trieSearched = false;
    bool trieButtonSelected = true; // trie button selected by default
    set<string> trieResults;
    vector<string> hashResults;
    vector<sf::Text> resultTextObjs;
    std::chrono::duration<double, std::milli> duration{};

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
                if (textEntered->unicode == 8 && !scrambledString.empty() && !searched) // backspace pressed
                    scrambledString.pop_back();
                if (isalpha(textEntered->unicode) && scrambledString.size() < 25 && !searched) // alphabetical keys pressed
                    scrambledString += tolower(static_cast<char>(textEntered->unicode));
            }
            // other key press event
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Enter && !scrambledString.empty()) { // enter key pressed
                    if (trieButtonSelected && !searched) { // enter pressed while trie is selected
                        searched = true;
                        trieSearched = true;

                        auto start = std::chrono::high_resolution_clock::now();
                        trieResults = trie.searchAnagrams(scrambledString);
                        auto end = std::chrono::high_resolution_clock::now();
                        duration = end - start;

                        int offset = -5;
                        for (const string& word : trieResults) {
                            sf::Text resultWord(font, word, 28.f);
                            resultWord.setFillColor(sf::Color::White);
                            resultWord.setOrigin(resultWord.getLocalBounds().getCenter());
                            resultWord.setPosition(sf::Vector2f(window_width/2, window_height/2 + offset*30));
                            resultTextObjs.push_back(resultWord);
                            offset++;
                        }
                    } else if (!trieButtonSelected && !searched) {  // enter pressed while hash is selected
                        searched = true;

                        auto start = std::chrono::high_resolution_clock::now();
                        hashResults = findwords(scrambledString, hash);
                        auto end = std::chrono::high_resolution_clock::now();
                        duration = end - start;

                        int offset = -5;
                        for (const string& word : hashResults) {
                            sf::Text resultWord(font, word, 28.f);
                            resultWord.setFillColor(sf::Color::White);
                            resultWord.setOrigin(resultWord.getLocalBounds().getCenter());
                            resultWord.setPosition(sf::Vector2f(window_width/2, window_height/2 + offset*30));
                            resultTextObjs.push_back(resultWord);
                            offset++;
                        }
                    } else { // enter pressed after a search is done (reset button)
                        searched = false;
                        trieSearched = false;
                        scrambledString.clear();
                        trieResults.clear();
                        hashResults.clear();
                        resultTextObjs.clear();
                    }
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::Up && !searched) { // up arrow key pressed
                    trieButtonSelected = true;
                }
                if (keyPressed->scancode == sf::Keyboard::Scan::Down && !searched) { // down arrow key pressed
                    hashButton.setFillColor(sf::Color::Cyan);
                    trieButtonSelected = false;
                }
            }

            // click event (for the buttons)
            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    if (trieButton.getGlobalBounds().contains(sf::Vector2f(mouse->position)) && !scrambledString.empty() && !searched) { // click on 'search with trie'
                        searched = true;
                        trieSearched = true;

                        auto start = std::chrono::high_resolution_clock::now();
                        trieResults = trie.searchAnagrams(scrambledString);
                        auto end = std::chrono::high_resolution_clock::now();
                        duration = end - start;

                        int offset = -5;
                        for (const string& word : trieResults) {
                            sf::Text resultWord(font, word, 28.f);
                            resultWord.setFillColor(sf::Color::White);
                            resultWord.setOrigin(resultWord.getLocalBounds().getCenter());
                            resultWord.setPosition(sf::Vector2f(window_width/2, window_height/2 + offset*30));
                            resultTextObjs.push_back(resultWord);
                            offset++;
                        }
                    }
                    if (hashButton.getGlobalBounds().contains(sf::Vector2f(mouse->position)) && !scrambledString.empty() && !searched) { // click on 'search with hash'
                        searched = true;

                        auto start = std::chrono::high_resolution_clock::now();
                        hashResults = findwords(scrambledString, hash);
                        auto end = std::chrono::high_resolution_clock::now();
                        duration = end - start;

                        int offset = -5;
                        for (const string& word : hashResults) {
                            sf::Text resultWord(font, word, 28.f);
                            resultWord.setFillColor(sf::Color::White);
                            resultWord.setOrigin(resultWord.getLocalBounds().getCenter());
                            resultWord.setPosition(sf::Vector2f(window_width/2, window_height/2 + offset*30));
                            resultTextObjs.push_back(resultWord);
                            offset++;
                        }
                    }
                    if (resetButton.getGlobalBounds().contains(sf::Vector2f(mouse->position)) && searched) { // click on 'RESET'
                        searched = false;
                        trieSearched = false;
                        scrambledString.clear();
                        trieResults.clear();
                        hashResults.clear();
                        resultTextObjs.clear();
                    }
                }
            }

        }



        appWindow.clear(sf::Color::Black);

        if (trieButtonSelected) {
            trieButton.setFillColor(sf::Color(25,25,25));
            hashButton.setFillColor(sf::Color::Transparent);
        } else {
            trieButton.setFillColor(sf::Color::Transparent);
            hashButton.setFillColor(sf::Color(25,25,25));
        }

        // update input text and draw each text block
        if (!searched) { // before either button has been pressed
            scrambledText.setString(scrambledString + '|');
            scrambledText.setOrigin(scrambledText.getLocalBounds().getCenter());
            scrambledText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 120));

            appWindow.draw(subtitleText);
            appWindow.draw(enterCharsText);
            appWindow.draw(trieButton);
            appWindow.draw(hashButton);
            appWindow.draw(trieText);
            appWindow.draw(hashText);
        }
        else { // after either button pressed (a search has been made)
            scrambledText.setString(scrambledString + ':');
            scrambledText.setPosition(sf::Vector2f(window_width/2, window_height/2 - 200));
            appWindow.draw(anagramsFor_Text);
            for (const sf::Text& result : resultTextObjs) {
                appWindow.draw(result);
            }
            if (trieSearched)
                durationText.setString("Time elapsed using Trie: " + to_string(duration.count()) + "ms");
            else
                durationText.setString("Time elapsed using Hashmap: " + to_string(duration.count()) + "ms");

            durationText.setOrigin(durationText.getLocalBounds().getCenter());
            durationText.setPosition(sf::Vector2f(window_width/2, window_height - 40));

            appWindow.draw(durationText);
            appWindow.draw(resetButton);
            appWindow.draw(resetText);
        }

        appWindow.draw(titleText);
        appWindow.draw(scrambledText); // the input characters

        appWindow.display();
    }


    return 0;
}
