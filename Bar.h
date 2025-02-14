#ifndef BAR_H
#define BAR_H
#include <SFML/Graphics.hpp>


class Bar: public sf::Drawable{
    sf::RectangleShape wrapper;
    sf::RectangleShape filler;
public:
    Bar(sf::Color,sf::Color,sf::Vector2f ,sf::Vector2f);
    Bar() = default;
    void update(float percentage);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};



#endif //BAR_H
