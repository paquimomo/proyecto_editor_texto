#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class TextBox {
public:
    TextBox() {}
    TextBox(int size, sf::Color color, bool sel, sf::Vector2f pos) {
        textbox.setCharacterSize(size);
        textbox.setFillColor(color);
        isSelected = sel;
        initialPosition = pos;
        this->setPosition(pos);
        if (sel) {
            textbox.setString("_");
        }
        else {
            textbox.setString("");
        }
    }

    void setFont(sf::Font& font) {
        textbox.setFont(font);
    }

    void setPosition(sf::Vector2f pos) {
        textbox.setPosition(pos);
    }

    void setLimit(bool ToF, int lim) {
        hasLimit = ToF;
        limit = lim;
    }

    void setSelected(bool sel) {
        isSelected = sel;
        if (!sel) {
            std::string t = text.str();
            std::string newT = "";
            for (int i = 0; i < t.length() - 1; i++) {
                newT += t[i];
            }
            textbox.setString(newT);
        }
    }

    std::string getText() {
        return text.str();
    }

    void drawTo(sf::RenderWindow& window) {
        window.draw(textbox);
    }

    void typedOn(sf::Event input) {
        if (isSelected) {
            int charTyped = input.text.unicode;
            if (charTyped < 128) {
                if (hasLimit) {
                    if (text.str().length() <= limit) {
                        inputLogic(charTyped);
                    }
                    else if (text.str().length() > limit && charTyped == DELETE_KEY) {
                        deleteChar();
                    }
                }
                else {
                    inputLogic(charTyped);
                }
            }
        }
    }

    void move(sf::Event input) {
        if (isSelected) {
            if (input.key.code == sf::Keyboard::Left) {
                if (cursorPos > 0) cursorPos--;
            }
            else if (input.key.code == sf::Keyboard::Right) {
                if (cursorPos < text.str().length()) cursorPos++;
            }

            setSelectedPos(cursorPos);
        }
    }


private:
    sf::Text textbox;
    std::ostringstream text;
    int limit;
    bool hasLimit;
    bool isSelected = false;
    sf::Vector2f initialPosition;
    int cursorPos = 0;

    void setSelectedPos(int pos) {
        cursorPos = pos;
        std::string t = text.str();
        textbox.setString(t.substr(0, cursorPos) + "_" + t.substr(cursorPos));
        sf::Vector2f newPos = initialPosition;
        if (cursorPos > 0) {
            newPos += sf::Vector2f(textbox.getFont()->getGlyph(text.str()[cursorPos - 1], textbox.getCharacterSize(), false).advance, 0);
        }
        textbox.setPosition(newPos);
    }

    void inputLogic(int charTyped) {
        if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
            text.str(text.str().insert(cursorPos, 1, static_cast<char>(charTyped)));
            cursorPos++;
            setSelectedPos(cursorPos);
        }
        else if (charTyped == DELETE_KEY) {
            deleteChar();
        }
    }

    void deleteChar() {
        if (text.str().length() > 0 && cursorPos > 0) {
            text.str(text.str().erase(cursorPos - 1, 1));
            cursorPos--;
            setSelectedPos(cursorPos);
        }
    }
};

int main() {
    sf::RenderWindow window;
    sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 445, (sf::VideoMode::getDesktopMode().height / 2) - 480);
    window.create(sf::VideoMode(900, 900), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(centerWindow);
    window.setKeyRepeatEnabled(true);
    sf::Font arial;
    arial.loadFromFile("C:/Windows/Fonts/arial.ttf"); //cargar la fuente de mi computadora
    TextBox textbox1(15, sf::Color::White, true, { 100,100 });
    textbox1.setFont(arial);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::TextEntered:
                    textbox1.typedOn(event);
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Left) {
                        textbox1.move(event);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        textbox1.move(event);
                    }
                    break;
                default:
                    break;
            }
        }
        window.clear();
        textbox1.drawTo(window);
        window.display();
    }
    return 0;
}
