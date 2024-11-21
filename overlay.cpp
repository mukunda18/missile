#include "overlay.h"

#include <iostream>


void overlay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(bg_sprite, states);
    for (const auto& but : buttons) {
        target.draw(but, states);
    }
    for (const auto& text : texts) {
        target.draw(text, states);
    }
    for (const auto& sprite: sprites) {
        target.draw(sprite,states);
    }
}

void overlay::update() {
    sf::Color cur = bg_sprite.getFillColor();
    if (fluc) {
        if (not n) {
            n = step;
        }
        if (cur.a<opacity) {
            increment = 1;
        }
        else if (cur.a>opacity+100) {
            increment = -1;
            step = 2;
        }
        n--;
        if (not n) {
            cur.a += increment;
            bg_sprite.setFillColor(cur);
        }
    }
    for (auto&sprite: sprites) {
        sprite.rotate(1);
    }
}

overlay::overlay(const sf::Vector2f size, sf::Color color ,int opacity, bool f,  sf::Vector2f pos) {
    bg_sprite.setSize(size);
    bg_sprite.setPosition(pos);
    fluc = f;
    if (fluc) {
        color.a = 0;
    }
    else {
        color.a = opacity;
    }
    bg_sprite.setFillColor(color);
    this->opacity = opacity;
}
