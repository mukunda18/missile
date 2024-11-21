#include "stateManager.h"

void StateManager::pushState(std::unique_ptr<screen> state) {
    states.push(std::move(state));
}

void StateManager::popState() {
    if (!states.empty()) {
        states.pop();
    }
}

void StateManager::changeState(std::unique_ptr<screen> state) {
    popState();
    pushState(std::move(state));
}

void StateManager::update() {
    if (!states.empty()) {
        states.top()->update();
    }
}

void StateManager::render() {
    if (!states.empty()) {
        states.top()->render();
    }
}

void StateManager::handleEvents() {
    if (!states.empty()) {
        states.top()->eventhandle();
    }
}