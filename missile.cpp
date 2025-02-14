#include "missile.h"

#include <filesystem>
#include "math.cpp"

namespace fs = std::filesystem;

void missile::restart() {
    Player.reset();
    enemies.clear();
    abilities.clear();
    game_over = false;
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
    const fs::path ability_png = resourcepath + "abilities";
    for (const auto&entry : fs::directory_iterator(ability_png)) {
        sf::Texture texture;
        texture.loadFromFile(entry.path().string());
        ability_textures.push_back(std::move(texture));
    }
    shield_texture.loadFromFile(resourcepath+"effects/shield.png");

    Player = player(plane_textures[0],windows_size);
    Player.set_shield(shield_texture);

    bg_texture.loadFromFile(resourcepath+"missile/background1.jpg");
    bg_scale = { windows_size.x/static_cast<float>(bg_texture.getSize().x),windows_size.y/static_cast<float>(bg_texture.getSize().y)};

    int i = 0;
    for (auto & bg_sprite : bg_sprites){
        bg_sprite.setTexture(bg_texture);
        bg_sprite.setScale(bg_scale);
        bg_sprite.setPosition(i%2?windows_size.x:0,i/2?windows_size.y:0);
        i++;
    }
    init_overlays();

    boost_bar = Bar(sf::Color::Yellow,sf::Color::White,sf::Vector2f(windows_size.x/20,windows_size.y*0.75f),sf::Vector2f(windows_size.x*0.02f,windows_size.y*0.05f));
    shield_bar = Bar(sf::Color::Blue,sf::Color::White,sf::Vector2f(windows_size.x/20,windows_size.y*0.75f),sf::Vector2f(windows_size.x - ( windows_size.x*0.02f+windows_size.x/20 ),windows_size.y*0.05f));
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        Player.to_boost = true;
    }
    else {
        Player.to_boost = false;
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
            if (abilities.size()<4) {
                add_ability();
            }
            clock1.restart();
        }
        const sf::Vector2f toRot = {static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)};
        const float angle = Player.getRotation() * mul;
        if (not Player.exploded) {
            Player.moveAngle(toRot);
            update_bg(angle);
        }
        if (Player.shield) {
            Player.shield -= 1;
        }
        if (Player.to_boost and Player.boost) {
            Player.boost -= 1;
            plane_speed_multiplier = 1.5;
        }
        else {
            plane_speed_multiplier = 1;
        }
        const sf::Vector2f toMov = {-std::cos(angle)*plane_speed * plane_speed_multiplier,-std::sin(angle)*plane_speed * plane_speed_multiplier};

        for (auto en = enemies.begin(); en != enemies.end(); ) {
            if (!en->exploded) {
                en->moveAngle(Player.getPosition());
                en->move();
            }
            if (!Player.exploded) {
                en->move(toMov);
            }
            if(const float dist = distance(Player.getPosition(),en->getPosition()); dist<windows_size.x/40) {
                if (!Player.exploded and en->explosion_num<4) {
                    if (Player.shield) {
                        if (collided_enemies.find(&(*en)) == collided_enemies.end()) {
                            Player.shield = std::max(Player.shield - 600, 0);
                            collided_enemies.insert(&(*en));
                        }
                    } else if (collided_enemies.find(&(*en)) == collided_enemies.end()) {
                        Player.explode(explosion_textures[0]);
                    }
                }
                if (!en->exploded and Player.explosion_num<4) {
                    en->explode(explosion_textures[0]);
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
                collided_enemies.erase(&(*en));
                en = enemies.erase(en);
            }
            else {
                ++en;
            }
        }
        for (auto ability = abilities.begin(); ability != abilities.end();) {
            if (is_inside_window(ability->get_pos(),windows_size)) {
                ability->clock.restart();
            }
            if (!Player.exploded) {
                ability->move(toMov);
            }
            if (ability->clock.getElapsedTime().asMilliseconds()>5000) {
                ability = abilities.erase(ability);
            }
            if ( ability->check( Player.getPosition()) ) {
                if (ability->type) {
                    Player.shield = std::min(Player.shield + 600, max_shield);
                }
                else {
                    Player.boost = std::min(Player.boost + 300, max_boost);
                }
                ability = abilities.erase(ability);
            }
            else {
                ++ability;
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
        bool temp = false;
        for (const auto&en: enemies) {
            temp = temp || en.exploded;
        }
        if (Player.explosion_num>12 and !temp) {
            game_over = true;
            sf::Text& t = game_overlay.texts[0];
            const sf::Vector2f pos = t.getPosition();
            t.setString("YOUR SCORE IS "+ std::to_string(time));
            t.setOrigin(t.getLocalBounds().width/2,t.getLocalBounds().height/2);
            t.setPosition(pos);
        }
        float percentage = float(Player.boost)/float(max_boost);
        boost_bar.update(percentage);
        percentage = float(Player.shield)/float(max_shield);
        shield_bar.update(percentage);

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
    const sf::Vector2f toMove = {-plane_speed *plane_speed_multiplier* std::cos(angle) + toMoveX * windows_size.x,-plane_speed*plane_speed_multiplier * std::sin(angle) + toMoveY * windows_size.y};
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
        text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
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
        plane_speed = 4;
        Player.set_speed(4,4);
        t.setString("MEDIUM");
        for (auto&en : enemies) {
            en.set_speed(4.5,1.5);
        }
        enemy_rot_speed = 1.5;
    }
    else if (difficulty==3) {
        plane_speed = 4;
        Player.set_speed(4,4);
        for (auto&en : enemies) {
            en.set_speed(4.5,2.0);
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

bool missile::is_inside_window(const sf::Vector2f objectPos, const sf::Vector2f &windowSize) {
    return (objectPos.x >= 0 && objectPos.x <= windowSize.x &&
            objectPos.y >= 0 && objectPos.y <= windowSize.y);
}

void missile::add_ability() {

    abilities.emplace_back(ability_textures[ab],windows_size,ab);
    ab = (ab+1)%2;
    float angle = Player.getRotation() * mul;
    const float dist = std::pow( (windows_size.x*windows_size.x) + (windows_size.y*windows_size.y), 0.5f )/2;

    angle += static_cast<float>(random_num(1)) * (random_num(1)? 1.f:-1.f);
    float x = (std::cos(angle) * dist) + Player.getPosition().x;
    float y = (std::sin(angle) * dist) + Player.getPosition().y;

    abilities.back().set_pos( sf::Vector2f(x,y) );
}

void missile::render() {
    window.clear(sf::Color::Black);
    for (auto& bg_sprite: bg_sprites) {
        window.draw(bg_sprite);
    }
    for (auto& enemy: enemies) {
        window.draw(enemy);
    }
    for (auto& ability: abilities) {
        window.draw(ability);
    }
    window.draw(Player);
    window.draw(timer);
    window.draw(boost_bar);
    window.draw(shield_bar);
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
