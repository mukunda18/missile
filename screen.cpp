#include "screen.h"

void screen::eventhandle() {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void screen::update() {
}

void screen::render() {
    window.clear(sf::Color::Black);
    window.display();
}

int screen::getchangestate() {
    return 0;
}

screen::screen(sf::RenderWindow &window) : window(window) {
};