#include "plane.h"
constexpr float mul = M_PI / 180 ;

void plane::reset(){
    sprite.setRotation(0);
    explosion_num = 0;
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
    if (explosion_num<8) {
        target.draw(sprite,states);
        if (exploded) {
            target.draw(explosion_sprite,states);
        }
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
        float scale = (sprite.getLocalBounds().height/4)/explosion_sprite.getLocalBounds().height;
        explosion_sprite.setScale(scale,scale);
    }
    else {
        if (explosion_num==4) {
            sprite.setScale(0,0);
        }
        if (explosion_num<5) {
            explosion_sprite.scale(1.4,1.4);
        }
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

player::player(const sf::Texture& texture, const sf::Vector2f windows_size):
plane(texture) {
    rotation_speed = 3;
    const float scale = (windows_size.x/30)/sprite.getLocalBounds().width;
    sprite.setScale(scale,scale);
    sprite.setPosition(windows_size.x/2,windows_size.y/2);
    sprite.setRotation(0);
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

