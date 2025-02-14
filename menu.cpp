#include "menu.h"

#include <filesystem>
namespace fs = std::filesystem;

menu::menu(sf::RenderWindow &window): screen(window) {

    windows_size.x = static_cast<float>(window.getSize().x);
    windows_size.y = static_cast<float>(window.getSize().y);

    const fs::path fontpath = resourcepath + "fonts/";
    for (const auto& entry: fs::directory_iterator(fontpath)) {
        sf::Font font;
        font.loadFromFile(entry.path().string());
        fonts.push_back(std::move(font));
    }
    isMouseClicked = isMouseReleased = false;
    music.openFromFile(resourcepath+"sounds/sound.mp3");
    music.setLoop(true);
    music.setVolume(10);
    music.setPitch(1.5);
    music.play();

    bg_texture.loadFromFile(resourcepath+"menu/background.png");
    bg_sprite.setTexture(bg_texture);
    bg_sprite.setPosition(bg_pos);
    bg_sprite.setScale((windows_size.x*2)/bg_sprite.getLocalBounds().width,(windows_size.y*2)/bg_sprite.getLocalBounds().height);

    button_texture.loadFromFile(resourcepath+"menu/button.png");
    sf::Vector2f pos = {windows_size.x/2, windows_size.y/2};
    const float button_scale = (windows_size.x/8)/static_cast<float>(button_texture.getSize().x);

    const std::string t = "PLAY GAME!";
    buttons[t] = button(button_texture,pos,button_scale);
    buttons[t].setText(t,fonts[0]);

    speaker_texture.loadFromFile(resourcepath+"menu/speaker.png");
    buttons["speaker"] = button(speaker_texture,{30,30},100.f/static_cast<float>(speaker_texture.getSize().x));
}

void menu::eventhandle() {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (event.type == sf::Event::MouseButtonPressed and (not isMouseClicked)) {
                click_pos = sf::Mouse::getPosition(window);
                isMouseClicked = true;
            }
            else if (event.type == sf::Event::MouseButtonReleased){
                isMouseReleased = true;
            }
        }
    }
}

int menu::getchangestate() {
    return changeState;
}

void menu::update() {
    music.setVolume(isMute ? 0:10);
    const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    int i = 1;
    for (auto &[name, button]: buttons) {
        if (button.isClicked(mouse_pos) and isMouseReleased and button.isClicked( mouse_pos)) {
            if (name=="speaker") {
                isMute = not isMute;
                i--;
            }
            else {
                changeState = i;
            }
        }
        if (button.isClicked(mouse_pos) and isMouseClicked) {
            button.enlarge(1.1);
        }
        else {
            button.enlarge(1);
        }
        if (name!="speaker") {
            button.animate();
        }
        i++;
    }
    if (isMouseReleased) {
        isMouseClicked = isMouseReleased = false;
    }
    animate_bg();
}

void menu::render() {
    window.clear(sf::Color::Black);
    window.draw(bg_sprite);
    for (auto &[name,button]: buttons) {
        window.draw(button);
    }
    window.draw(speaker);
    window.display();
}

void menu::animate_bg() {
    if (bg_pos.x > -windows_size.x && bg_pos.y == 0) {
        bg_pos.x -= bg_speed;
    }
    else if (bg_pos.x <= -windows_size.x && bg_pos.y > -windows_size.y) {
        bg_pos.y -= bg_speed;
    }
    else if (bg_pos.y <= -windows_size.y && bg_pos.x < 0) {
        bg_pos.x += bg_speed;
    }
    else if (bg_pos.x >= 0 && bg_pos.y < 0) {
        bg_pos.y += bg_speed;
    }
    if (bg_pos.x >= 0 && bg_pos.y >= 0) {
        bg_pos = sf::Vector2f(0, 0);
    }
    bg_sprite.setPosition(bg_pos);
}


menu::~menu() {
    music.stop();
}