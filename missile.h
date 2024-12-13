#ifndef MISSILE_H
#define MISSILE_H

#include "screen.h"
#include "plane.h"
#include "overlay.h"

class missile: public screen{
    sf::Clock clock1,clock2;
    int time = 0;
    sf::Text timer;

    bool game_over=false,option=false;
    overlay game_overlay,option_overlay;
    std::vector<sf::Texture> btn_textures;
    void restart();

    const float mul = M_PI / 180 ;
    std::string resourcepath = "../resources/";
    int changeState = 0;

    int difficulty = 2;
    float plane_speed = 4;
    float enemy_speed = 4.5;
    float enemy_rot_speed = 1.5;
    float explosion_time =0.15;
    sf::Vector2f windows_size;

    bool isMouseClicked, isMouseReleased;
    sf::Vector2i click_pos;
    sf::Vector2i mouse_pos;

    sf::Texture bg_texture;
    sf::Vector2f bg_scale;
    sf::Sprite bg_sprites[4];

    std::vector<sf::Texture> plane_textures;
    std::vector<sf::Texture> explosion_textures;
    player Player;
    int plane_num = 0;

    std::vector<sf::Texture> enemy_textures;
    std::vector<enemy> enemies;
    int enemy_spawn_timer = 4;

    std::vector<sf::Font> fonts;
public:
    explicit missile(sf::RenderWindow&);
    ~missile() override = default;

    void eventhandle() override;
    void update() override;
    void render() override;

    void update_bg(float);
    void init_overlays();

    void add_enemy();
    void update_setting();

    int getchangestate() override;
};



#endif //MISSILE_H
