#pragma once
#include "game_state.hpp"
#include "buttons.hpp"

class Bullet {
public:
//    Bullet(GameState *game);
    Bullet(GameState *game, Buttons *buttons, MessageQueue<Message> &q);
    void bulletInputThread();
    void bulletUpdateThread();

private:
    MessageQueue<Message> &q_;
    GameState *game;
    Buttons *buttons;
    std::mutex bulletThreadLock_;
};