#include "plane.h"
#include <filesystem>
constexpr float mul = M_PI / 180 ;

namespace fs = std::filesystem;

void plane::reset(){
    sprite.setRotation(0);
    explosion_num = 0;
    exploded = false;
}

void plane::move() {
    const float angle = sprite.getRotation();
    const sf::Vector2f toMove = {speed * std::cos(angle*mul),speed * std::sin(angle*mul)};
    sprite.move(toMove);
    if (exploded) {
        explosion_sprite.move(toMove);
    }
}

void plane::move(const sf::Vector2f toMove) {
    sprite.move(toMove);
    if (exploded) {
        explosion_sprite.move(toMove);
    }
}


void plane::moveAngle(const sf::Vector2f pos) {
    const sf::Vector2f vec = {pos.x - sprite.getPosition().x, pos.y - sprite.getPosition().y};
    const float toAngle = getAngle(vec);
    const float curAngle = sprite.getRotation();
    float angleDiff = toAngle-curAngle;
    angleDiff += angleDiff>180? -360.f: (angleDiff<-180? 360.f:0);
    if (std::abs(angleDiff)<rotation_speed) {
        sprite.setRotation(toAngle);
    }
    else {
        sprite.rotate(angleDiff>0 ? rotation_speed: -rotation_speed);
    }
}

void plane::draw(sf::RenderTarget &target, const sf::RenderStates states) const {
    if (explosion_num<4) {
        target.draw(sprite,states);
    }
    if (exploded and explosion_num<8) {
        target.draw(explosion_sprite,states);
    }
}

void plane::setPlaneTexture(const sf::Texture &texture) {
    sprite.setTexture(texture);
}

float plane::getRotation() const {
    return sprite.getRotation();
}

void plane::explode(const sf::Texture& texture) {
    clock.restart();
    explosion_sprite.setTexture(texture);
    explosion_sprite.setOrigin(explosion_sprite.getLocalBounds().width/2,explosion_sprite.getLocalBounds().height/2);
    explosion_sprite.setPosition(sprite.getPosition());
    if (!exploded) {
        exploded = true;
        const float scale = (sprite.getLocalBounds().height/4)/explosion_sprite.getLocalBounds().height;
        explosion_sprite.setScale(scale,scale);
    }
    else if (explosion_num<5){
        explosion_sprite.scale(1.4,1.4);
    }
    explosion_num++;
}

void plane::explode() {
    explosion_num++;
}

void plane::set_speed(float a, float b) {
    speed = a;
    rotation_speed = b;
}

sf::Vector2f plane::getPosition() const {
    return sprite.getPosition();
}


plane::plane(const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
}

void player::set_shield(const sf::Texture &texture) {
    shield_sprite.setTexture(texture);
    shield_sprite.setOrigin(shield_sprite.getLocalBounds().width/2,shield_sprite.getLocalBounds().height/2);
    const float scale = sprite.getLocalBounds().width / shield_sprite.getLocalBounds().width;
    shield_sprite.setScale(scale,scale);
    shield_sprite.setPosition(sprite.getPosition());
    shield_sprite.setRotation(sprite.getRotation()+180);
}

player::player(const sf::Texture& texture, const sf::Vector2f windows_size):
plane(texture) {
    rotation_speed = 3;
    const float scale = (windows_size.x/30)/sprite.getLocalBounds().width;
    sprite.setScale(scale,scale);
    sprite.setPosition(windows_size.x/2,windows_size.y/2);
    sprite.setRotation(0);

    const fs::path p = "../resources/effects/boost";
    for (const auto& entry: fs::directory_iterator(p)) {
        sf::Texture t;
        t.loadFromFile(entry.path().string());
        boost_effect_textures.push_back(std::move(t));
    }

    boost_effect_sprite.setTexture(boost_effect_textures[0]);
    boost_effect_sprite.setScale(2,2);
    boost_effect_sprite.setOrigin(boost_effect_sprite.getLocalBounds().width/2,0);
    boost_effect_sprite.setPosition(sprite.getPosition());
}

void player::reset() {
    plane::reset();
    boost = 0;
    shield = 0;
    to_boost = false;
}

void player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (to_boost and boost) {
        target.draw(boost_effect_sprite,states);
    }
    if (shield>0) {
        target.draw(shield_sprite,states);
    }
    plane::draw(target, states);
}

void player::moveAngle(sf::Vector2f vector2_f) {
    plane::moveAngle(vector2_f);
    shield_sprite.setRotation(sprite.getRotation());
    boost_effect_sprite.setRotation(sprite.getRotation()+90);
    boost_no =  (boost_no+1) % static_cast<int>(boost_effect_textures.size() * 10);
    boost_effect_sprite.setTexture(boost_effect_textures[boost_no/10]);
}

enemy::enemy(const sf::Texture& texture, const sf::Vector2f windows_size, const sf::Vector2f pos, const float angle , const float speed, const float rot):
plane(texture) {
    rotation_speed = rot;
    this->speed = speed;
    const float size = (windows_size.x/50);
    sprite.setScale(size/sprite.getLocalBounds().width,size/sprite.getLocalBounds().height);
    sprite.setPosition(pos);
    sprite.setRotation(angle);
}

