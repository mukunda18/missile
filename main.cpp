#include "StateManager.h"
#include "menu.h"
#include "missile.h"

constexpr float updateInterval = 1.f/60.f;
int main() {
    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(
    sf::VideoMode(desktopMode.width, desktopMode.height),
    "Games",
    sf::Style::Titlebar | sf::Style::Close | sf::Style::Default
    );
    StateManager state_manager;
    state_manager.pushState(std::make_unique<menu>(window));

    sf::Clock mainclock;
    float time = 0;

    while (window.isOpen()) {
        state_manager.handleEvents();
        time += mainclock.restart().asSeconds();
        while (time >= updateInterval) {
            state_manager.update();
            time -= updateInterval;
        }
        state_manager.render();
        if (!state_manager.states.empty() && state_manager.states.top()->getchangestate() !=0) {
            int change = state_manager.states.top()->getchangestate();
            if (change == 2) {
                state_manager.changeState(std::make_unique<missile>(window));
            }
            else if (change == -1) {
                state_manager.changeState(std::make_unique<menu>(window));
            }
        }
    }
}