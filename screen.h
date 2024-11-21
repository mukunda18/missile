#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

class screen {
protected:
    sf::Event event{};
    sf::RenderWindow& window;
public:
    virtual void eventhandle();
    virtual void update();
    virtual void render();
    virtual int getchangestate();
    explicit screen(sf::RenderWindow&);
    virtual ~screen() = default;
};

#endif