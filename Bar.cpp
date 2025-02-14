#include "Bar.h"

Bar::Bar(sf::Color fill, sf::Color border, sf::Vector2f size, sf::Vector2f pos) {
    wrapper.setSize(size);
    wrapper.setFillColor(sf::Color::Transparent);
    wrapper.setOutlineThickness(3.0f);
    wrapper.setOutlineColor(border);
    wrapper.setPosition(pos);

    fill.a = 150;
    filler.setFillColor(fill);
    filler.setSize(sf::Vector2f(0,0));
}

void Bar::update(float percentage) {
    filler.setSize(sf::Vector2f( wrapper.getSize().x,wrapper.getSize().y * percentage) );
    filler.setPosition( sf::Vector2f( wrapper.getPosition().x, wrapper.getPosition().y + wrapper.getSize().y - filler.getSize().y ) );
}

void Bar::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(wrapper,states);
    target.draw(filler,states);
}
