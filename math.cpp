#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <SFML/Graphics.hpp>
#include <random>


static float getAngle(const sf::Vector2f& pos) {
    return static_cast<float>(std::atan2(pos.y, pos.x) * (180 / M_PI));
}

static int random_num(const int n) {
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, n);
    return distribution(generator);
}

static float distance(const sf::Vector2f vec1, const sf::Vector2f vec2) {
    const auto x2 = static_cast<float>(pow(vec1.x - vec2.x, 2));
    const auto y2 = static_cast<float>(pow(vec1.y - vec2.y, 2));
    return static_cast<float>(pow(x2+y2,0.5));
}



#endif // MATH_UTILS_H
