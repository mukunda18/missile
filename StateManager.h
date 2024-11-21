#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "screen.h"
#include <memory>
#include <stack>

class StateManager {
public:
    std::stack<std::unique_ptr<screen>> states;
    StateManager() =  default;
    void pushState(std::unique_ptr<screen> state);
    void popState();
    void changeState(std::unique_ptr<screen> state);
    void update();
    void render();
    void handleEvents();
};

#endif // STATE_MANAGER_H