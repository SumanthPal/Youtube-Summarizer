#include <SFML/Graphics.hpp>
#include <iostream>  // Include this for cout
#include <curl/curl.h>
#include "../youtube++/TextBox.h"
#include "Button.h"
#include "../youtube++/TranscriptStealer.h"
#include "openai.hpp"
#include "json.h"

using Json = nlohmann::json;

std::string summarize_GPT(std::string transcript) {
//    openai::start(); // Will use the api key provided by `OPENAI_API_KEY` environment variable
//    // openai::start("your_API_key", "optional_organization"); // Or you can handle it yourself

    std::cout << transcript << std::endl;
    openai::start("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");
    std::string prompt = "Summarize this stuff please " + transcript;
    Json jsonData = {
            {"model", "text-davinci-003"},
            {"prompt", prompt},
            {"max_tokens", 1000},
            {"temperature", 0}
    };
    auto completion = openai::completion().create(jsonData);
    //std::string output = completion.dump(2);

    //std::cout << completion.at("choices") << std::endl;
    std::string output_text = completion["choices"][0]["text"].dump();


    return output_text;
}
std::string text_wrap(const std::string& word, int wrap_num) {
    std::string reformatted;

    for (int i = 0; i < word.size(); ++i) {
        reformatted += word[i];

        if ((i + 1) % wrap_num == 0) {
            reformatted += '\n';
        }
    }

    return reformatted;
}

int main() {

    //creates the window + add the font
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "YouTube++");
    sf::Font font;
    if (!font.loadFromFile("../Inter-Bold.ttf"))
    {
        std::cout << "font unable to load" << std::endl;
    }


    //main text box to add code
    Textbox text(20, sf::Color::Black, true);
    text.setPosition({ 600, 300 });
    text.setLimit(true, 30);
    text.setFont(font);



    //btn to enter code
    Button btn1("Enter", { 200, 100 }, 30, sf::Color::Red, sf::Color::Black);
    btn1.setFont(font);
    btn1.setPosition({ 700, 500 });


    //string for input:
    sf::String playerInput;
    sf::String transcript;


    //image on app
    sf::Image pic;
    pic.loadFromFile("../title_pic.png");
    sf::Texture texture;
    texture.loadFromImage(pic);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.5,0.5);
    sprite.setPosition(250,0);

    //icon for app
    sf::Image icon;
    icon.loadFromFile("../logo.png");

    //some text for the headder
    sf::Text title;
    title.setFont(font);
    title.setPosition(500, 10);
    title.setOutlineThickness(2);
    title.setOutlineColor(sf::Color::Black);
    title.setString("Welcome to the Youtube Transcript Generator");



    sf::Text outputBox;
    outputBox.setFont(font);
    outputBox.setPosition(450, 700);
    outputBox.setOutlineThickness(2);
    outputBox.setOutlineColor(sf::Color::Black);
    outputBox.setCharacterSize(15);

    //outputBox.setString("Welocome!");



    sf::Text summary_box;
    summary_box.setFont(font);
    summary_box.setPosition(650, 650);
    summary_box.setOutlineThickness(2);
    summary_box.setOutlineColor(sf::Color::Black);
    summary_box.setCharacterSize(30);
    summary_box.setString("Here's Your Summary!");
    while (window.isOpen()) {
        //the basic event used for checking what's gonna happen
        sf::Event event;

        //Just checks if the keyboard is being used with a return key click
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            text.setSelected(true);
        }
            //gets rid of the typing feature
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            text.setSelected(false);
        }
        while (window.pollEvent(event))
        {
            //window actions
            if (event.type == sf::Event::Closed)
                window.close();

            //basic typing function
            if (event.type == sf::Event::TextEntered) {
                std::cout << "Text being entered." << std::endl;
                text.typedOn(event);
            }

            //basic button action
            if (event.type == sf::Event::MouseMoved) {
                if (btn1.isMouseOver(window)) {

                    sf::Color button_color(211,211,211);
                    btn1.setBackColor(button_color);
                } else {
                    btn1.setBackColor(sf::Color::Red);

                }
            }
            //this actually updates the text.
            text.palette(event);

            //the transcript stealer function
            if(btn1.isButtonPressed(window)) {
                std::string url = text.getText();
                TranscriptStealer stealer;

                std::string trans = stealer.getTranscript(url);

                outputBox.setString(trans);

                std::string new_summary = summarize_GPT(trans);

                outputBox.setString(text_wrap(new_summary,100));

                std::string path =stealer.findVttFile("/Users/sumanth/CLionProjects/youtube++/cmake-build-debug");
                std::__fs::filesystem::remove(path);
            }
        }

        window.setIcon(960,720,icon.getPixelsPtr());

        window.clear(sf::Color::White);
        // window.draw(inputText);
        window.draw(title);
        window.draw(outputBox);
        window.draw(summary_box);
        btn1.drawTo(window);

        text.drawTo(window);



        // window.draw(summaryText);
        window.display();
    }

    return 0;
}
