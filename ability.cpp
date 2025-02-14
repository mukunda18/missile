#include "ability.h"

ability::ability(const sf::Texture & texture, sf::Vector2f windows_size, int t) {
    sprite.setTexture(texture);
    type = t;
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    const float scale = (windows_size.x/60)/sprite.getLocalBounds().width;
    sprite.setScale(scale,scale);

    dist = windows_size.x/30;
}

void ability::set_pos(const sf::Vector2f pos) {
    sprite.setPosition(pos);
}

sf::Vector2f ability::get_pos() const {
    return sprite.getPosition();
}

void ability::move(const sf::Vector2f toMov) {
    sprite.move(toMov);
}

bool ability::check(const sf::Vector2f a) const {
    return distance(a,sprite.getPosition()) < dist;
}

void ability::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite,states);
}
