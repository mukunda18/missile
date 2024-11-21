#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "math.cpp"

class plane: public sf::Drawable{
protected:
    sf::Sprite sprite;
    sf::Sprite explosion_sprite;
    float rotation_speed = 0, speed = 0;
public:
    void reset();

    sf::Clock clock;
    bool exploded = false;
    void move();
    void move(sf::Vector2f);
    void moveAngle(sf::Vector2f);
    void draw(sf::RenderTarget &, sf::RenderStates ) const override;
    void setPlaneTexture(const sf::Texture&);
    float getRotation() const;
    void explode(const sf::Texture&);
    void explode();
    void set_speed(float,float);
    int explosion_num = 0;
    sf::Vector2f getPosition() const;

    plane() = default;
    explicit plane(const sf::Texture&);
    ~plane() override = default;
};

class player: public plane {
public:
    player() = default;
    player(const sf::Texture &,sf::Vector2f);
};

class enemy: public plane {
public:
    enemy() = default;
    enemy(const sf::Texture &, sf::Vector2f, sf::Vector2f, float,float,float);
};

#endif //ENEMY_H