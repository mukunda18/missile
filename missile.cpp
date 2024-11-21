#include "missile.h"

#include <filesystem>
#include <iostream>

#include "math.cpp"

namespace fs = std::filesystem;

void missile::restart() {
    Player.reset();
    Player.explosion_num = 0;
    enemies.clear();
    game_over = false;
    Player = player(plane_textures[0],windows_size);
    sf::Color c = game_overlay.bg_sprite.getFillColor();
    c.a = 0;
    game_overlay.bg_sprite.setFillColor(c);
    time = 0;
    clock1.restart();
    clock2.restart();
}

missile::missile(sf::RenderWindow &window): screen(window) {
    game_over = false;
    windows_size.x = static_cast<float>(this->window.getSize().x);
    windows_size.y = static_cast<float>(this->window.getSize().y);
    const fs::path fontpath = resourcepath + "fonts/";
    for (const auto& entry: fs::directory_iterator(fontpath)) {
        sf::Font font;
        font.loadFromFile(entry.path().string());
        fonts.push_back(std::move(font));
    }
    isMouseClicked = isMouseReleased = false;
    const fs::path plane_png_path = resourcepath + "missile/planes/";
    for (const auto& entry: fs::directory_iterator(plane_png_path)) {
        sf::Texture texture;
        texture.loadFromFile(entry.path().string());
        plane_textures.push_back(std::move(texture));
    }
    const fs::path enemy_png  = resourcepath + "missile/Enemies";
    for (const auto&entry : fs::directory_iterator(enemy_png)) {
        sf::Texture texture;
        texture.loadFromFile(entry.path().string());
        enemy_textures.push_back(std::move(texture));
    }
    const fs::path explosion_png = resourcepath + "missile/Explosion";
    for (const auto&entry : fs::directory_iterator(explosion_png)) {
        sf::Texture texture;
        texture.loadFromFile(entry.path().string());
        explosion_textures.push_back(std::move(texture));
    }
    const fs::path btn_png = resourcepath + "buttons";
    for (const auto&entry : fs::directory_iterator(btn_png)) {
        sf::Texture texture;
        texture.loadFromFile(entry.path().string());
        btn_textures.push_back(std::move(texture));
    }
    Player = player(plane_textures[0],windows_size);
    bg_texture.loadFromFile(resourcepath+"missile/background1.jpg");
    bg_scale = { windows_size.x/static_cast<float>(bg_texture.getSize().x),windows_size.y/static_cast<float>(bg_texture.getSize().y)};
    int i = 0;
    for (auto & bg_sprite : bg_sprites){
        bg_sprite.setTexture(bg_texture);
        bg_sprite.setScale(bg_scale);
        bg_sprite.setPosition(i%2?windows_size.x:0,i/2?windows_size.y:0);
        i++;
    }
    enemies = std::vector<enemy>(8);
    init_overlays();

    bg_music.openFromFile(resourcepath+"sounds/sound.mp3");
    bg_music.setLoop(true);
    bg_music.setVolume(10);
    bg_music.setPitch(1.0);
    bg_music.play();
}

void missile::eventhandle() {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            if (game_over) {
                changeState = -1;
            }
            else {
                option = not option;
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Left) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (not isMouseClicked) {
                    click_pos = sf::Mouse::getPosition(window);
                    isMouseClicked = true;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                isMouseReleased = true;
            }
        }
    }
}

void missile::update() {
    if (not (option or game_over)) {
        if (clock2.getElapsedTime().asMilliseconds()>1000) {
            time++;
            clock2.restart();
            const int m = time/60;
            const int s = time%60;
            const std::string str = (m<10? "0":"")+ std::to_string(m) + ":" + (s<10? "0":"") + std::to_string(s);
            timer.setString(str);
        }
        if (clock1.getElapsedTime().asSeconds() > static_cast<float>(enemy_spawn_timer)) {
            add_enemy();
            clock1.restart();
        }
        const sf::Vector2f toRot = {static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)};
        const float angle = Player.getRotation() * mul;
        if (not Player.exploded) {
            Player.moveAngle(toRot);
            update_bg(angle);
        }
        const sf::Vector2f toMov = {-std::cos(angle)*plane_speed,-std::sin(angle)*plane_speed};

        float total_p = 0;

        for (auto en = enemies.begin(); en != enemies.end(); ) {
            if (!en->exploded) {
                en->moveAngle(Player.getPosition());
                en->move();
            }
            if (!Player.exploded) {
                en->move(toMov);
            }
            const float dist = distance(en->getPosition(),Player.getPosition());
            if (dist*1.5<windows_size.x/2) {
                total_p+=0.05;
            }
            if (dist < windows_size.x/40 and Player.explosion_num<5 and en->explosion_num<5) {
                if (!en->exploded) {
                    en->explode(explosion_textures[0]);
                }
                if (!Player.exploded) {
                    Player.explode(explosion_textures[0]);
                }
            }
            if (en->clock.getElapsedTime().asSeconds() > static_cast<float>(enemy_spawn_timer * 4) or (en->exploded and en->clock.getElapsedTime().asSeconds()>explosion_time)) {
                if(en->explosion_num<7) {
                    en->explode(explosion_textures[en->explosion_num]);
                }
                else {
                    en->explode();
                }
            }

            if (en->explosion_num==9) {
                en = enemies.erase(en);
            }
            else {
                ++en;
            }
        }
        if (Player.exploded and Player.clock.getElapsedTime().asSeconds()>explosion_time) {
            if (Player.explosion_num<7) {
                Player.explode(explosion_textures[Player.explosion_num]);
            }
            else {
                Player.explode();
            }
        }
        if (Player.explosion_num==12) {
            game_over = true;
            sf::Text& t = game_overlay.texts[0];
            const sf::Vector2f pos = t.getPosition();
            t.setString("YOUR SCORE IS "+ std::to_string(time));
            t.setOrigin(t.getLocalBounds().width/2,t.getLocalBounds().height/2);
            t.setPosition(pos);
        }
        if (clock3.getElapsedTime().asSeconds()>0.2) {
            total_p = std::max(total_p,2.f);
            float diff = total_p - bg_music.getPitch();
            if (std::abs(diff)<0.01) {
                bg_music.setPitch(total_p);
            }
            else {
                bg_music.setPitch(bg_music.getPitch() + (diff<0 ? -0.01f:0.01f));
            }
            clock3.restart();
        }
    }
    else if(game_over) {
        game_overlay.update();
        button &button  = game_overlay.buttons[0];
        if (isMouseReleased and button.isClicked(click_pos) and button.isClicked(sf::Mouse::getPosition(window))) {
            restart();
        }
        if (isMouseClicked and button.isClicked(sf::Mouse::getPosition(window))) {
            button.enlarge(1.1);
        }
        else {
            button.enlarge(1);
        }
        if (isMouseReleased) {
            isMouseClicked = isMouseReleased = false;
        }
    }
    else {
        option_overlay.update();
        int i = 0;
        bool toUpdate = false;
        for (auto& button: option_overlay.buttons) {
            if (isMouseReleased && button.isClicked(click_pos) && button.isClicked(sf::Mouse::getPosition(window))) {
                toUpdate = true;
                if (i/2 == 0) {
                    plane_num = (plane_num + (i%2 ? 1:-1) + 12)%12;
                }
                else if (i/2 ==1) {
                    int zero_to_two = difficulty-1;
                    zero_to_two = (zero_to_two + (i%2 ? 1:-1) + 3)%3;
                    difficulty = zero_to_two + 1;
                }
                else if (i/2 == 2) {
                    int zero_to_two = (enemy_spawn_timer/2) - 1;
                    zero_to_two = (zero_to_two + (i%2 ? 1:-1) + 3)%3;
                    enemy_spawn_timer = (zero_to_two + 2)*2;
                }
            }
            if (isMouseClicked and button.isClicked(sf::Mouse::getPosition(window))) {
                button.enlarge(1.1);
            }
            else {
                button.enlarge(1);
            }
            i++;
        }
        if (isMouseReleased) {
            isMouseClicked = isMouseReleased = false;
        }
        if (toUpdate) {
            update_setting();
        }
    }
}

void missile::update_bg(const float angle) {
    const float toMoveX = (bg_sprites[0].getPosition().x<-windows_size.x ? 1.f : (bg_sprites[0].getPosition().x>0 ? -1.f : 0));
    const float toMoveY = (bg_sprites[0].getPosition().y<-windows_size.y ? 1.f : (bg_sprites[0].getPosition().y>0 ? -1.f : 0));
    const sf::Vector2f toMove = {-plane_speed * std::cos(angle) + toMoveX * windows_size.x,-plane_speed * std::sin(angle) + toMoveY * windows_size.y};
    for (auto& bg: bg_sprites) {
        bg.move(toMove);
    }
}

void missile::init_overlays() {
    const float align_x = windows_size.x/2;
    game_overlay = overlay(windows_size,sf::Color::Black,100,true);
    sf::Text text("YOUR SCORE WAS",fonts[1]);
    text.setFillColor(sf::Color::White);
    text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
    text.setPosition(align_x,windows_size.y/2);
    game_overlay.buttons.emplace_back(btn_textures[6],sf::Vector2f(align_x,windows_size.y * 0.6f),.1);
    game_overlay.buttons[0].setText("NEW GAME",fonts[0]);
    game_overlay.texts.push_back(std::move(text));

    timer = sf::Text("00:00",fonts[1],60);
    timer.setFillColor(sf::Color(255,255,255,150));
    timer.setOrigin(timer.getLocalBounds().width/2,timer.getLocalBounds().height/2);
    timer.setPosition(windows_size.x/18,windows_size.y*0.85f);

    option_overlay = overlay(sf::Vector2f(align_x,windows_size.y),sf::Color::Black,180,false,sf::Vector2f(align_x/2,0));
    const float diff_y = windows_size.y/4;

    sf::Sprite sprite(plane_textures[plane_num]);
    sprite.setOrigin(sprite.getLocalBounds().width/2,sprite.getLocalBounds().height/2);
    float scale = (diff_y*0.6f)/sprite.getLocalBounds().height;
    sprite.setScale(scale,scale);
    sprite.setPosition(align_x,windows_size.y/4);
    option_overlay.sprites.push_back(sprite);

    std::vector<std::string> titles = {"PLAYER SPRITE","skip","DIFFICULTY","MEDIUM","ENEMY SPAWN TIMER","4"};
    float i = -2.0;
    bool place_arrow = true;
    for (auto& t:titles) {
        place_arrow= not place_arrow;
        text.setFillColor((text.getFillColor()==sf::Color::White)? sf::Color::Red: sf::Color::White);
        i+=0.5;
        if (place_arrow) {
            option_overlay.buttons.emplace_back(btn_textures[0],sf::Vector2f(align_x*0.8f,windows_size.y/2 + (i * diff_y)),0.2);
            option_overlay.buttons.emplace_back(btn_textures[1],sf::Vector2f(align_x*1.2f,windows_size.y/2 + (i * diff_y)),0.2);
        }
        if (t=="skip") {
            continue;
        }
        text.setString(t);
        text.setOrigin(text.getLocalBounds().getSize().x/2,text.getLocalBounds().getSize().y/2);
        text.setPosition(align_x,windows_size.y/2 + (i * diff_y));
        option_overlay.texts.push_back(std::move(text));
    }
}


void missile::add_enemy() {
    const int rand1 = random_num(1);
    const int rand2 = random_num(1);
    sf::Vector2f pos = {rand1 ? static_cast<float>(random_num(static_cast<int>(windows_size.x))) : windows_size.x * (rand2? -0.1f:1.1f),
        (!rand1) ? static_cast<float>(random_num(static_cast<int>(windows_size.y))) : windows_size.x * (rand2? -0.1f:1.1f)};
    float angle = getAngle(sf::Vector2f(Player.getPosition().x-pos.x,Player.getPosition().y-pos.y));
    enemies.emplace_back(enemy_textures[random_num(19)],windows_size,pos,angle,enemy_speed,enemy_rot_speed);
}

void missile::update_setting() {
    sf::Text &t = option_overlay.texts[2];
    sf::Vector2f pos = t.getPosition();
    if (difficulty==1) {
        plane_speed = 5;
        Player.set_speed(5,4);
        t.setString("EASY");
        for (auto&en : enemies) {
            en.set_speed(4.5,1.5);
        }
        enemy_rot_speed = 1.5;
    }
    else if (difficulty==2) {
        Player.set_speed(4,4);
        t.setString("MEDIUM");
        for (auto&en : enemies) {
            en.set_speed(4.5,1.5);
        }
        enemy_rot_speed = 1.5;
    }
    else if (difficulty==3) {
        Player.set_speed(4,4);
        for (auto&en : enemies) {
            en.set_speed(4.5,2);
        }
        enemy_rot_speed = 2;
        t.setString("HARD");
    }
    t.setOrigin(t.getLocalBounds().width/2,t.getLocalBounds().height/2);
    t.setPosition(pos);
    Player.setPlaneTexture(plane_textures[plane_num]);
    option_overlay.sprites[0].setTexture(plane_textures[plane_num]);
    option_overlay.texts[4].setString(std::to_string(enemy_spawn_timer));
}


void missile::render() {
    window.clear(sf::Color::Black);
    for (auto& bg_sprite: bg_sprites) {
        window.draw(bg_sprite);
    }
    for (auto& enemy: enemies) {
        window.draw(enemy);
    }
    window.draw(Player);
    window.draw(timer);
    if (game_over) {
        window.draw(game_overlay);
    }
    else if (option) {
        window.draw(option_overlay);
    }
    window.display();
}

int missile::getchangestate() {
    return changeState;
}
