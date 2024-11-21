#ifndef OVERLAY_H
#define OVERLAY_H

#include <SFML/Graphics.hpp>
#include "button.h"

class overlay: public sf::Drawable{
    sf::RectangleShape bg_sprite;
    bool fluc = true;
    sf::Clock clock;
    std::vector<button> buttons;
    std::vector<sf::Text> texts;
    std::vector<sf::Sprite> sprites;
    int increment = 1;
    int step = 1,n=0;
    int opacity = 0;
public:
    overlay(sf::Vector2f ,sf::Color ,int , bool, sf::Vector2f pos = sf::Vector2f(0,0));
    overlay() = default;
    friend class missile;
    void draw(sf::RenderTarget &, sf::RenderStates ) const override;
    void update();
};



#endif //OVERLAY_H
