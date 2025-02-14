#ifndef ABILITY_H
#define ABILITY_H

#include <SFML/Graphics.hpp>
#include "math.cpp"

class ability: public sf::Drawable{
    sf::Sprite sprite;
    float dist = 0;
public:
    int type = 0;
    bool appeared =  false;
    sf::Clock clock;

    ability() = default;
    ability(const sf::Texture &,sf::Vector2f,int);

    void set_pos(sf::Vector2f);
    sf::Vector2f get_pos() const;
    void move(sf::Vector2f);
    bool check(sf::Vector2f) const;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};



#endif //ABILITY_H
