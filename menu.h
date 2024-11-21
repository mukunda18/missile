#ifndef MENU_H
#define MENU_H

#include "screen.h"
#include "button.h"

class menu: public screen {
    std::string resourcepath = "../resources/";

    sf::Vector2f bg_pos = {0,0};
    float bg_speed = 4;

    sf::Vector2f windows_size;

    bool isMouseClicked, isMouseReleased;
    int changeState = 0;

    sf::Texture bg_texture;
    sf::Sprite bg_sprite;

    sf::Texture button_texture;
    int num_button = 4;
    sf::Vector2u button_spacing;
    std::map<std::string,button> buttons;
    button speaker;
    sf::Texture speaker_texture;

    std::vector<sf::Font> fonts;
    sf::Music music;
    bool isMute = false;

    void animate_bg();
    std::map<std::string,sf::Clock> clocks;
    sf::Vector2i click_pos;

public:
    explicit menu(sf::RenderWindow &);
    ~menu() override;
    void update() override;
    void render() override;
    void eventhandle() override;

    int getchangestate() override;

};

#endif //MENU_H
