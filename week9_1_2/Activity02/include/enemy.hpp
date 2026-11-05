#pragma once
#include "game_state.hpp"

class Enemy {
public:
    Enemy(GameState *game, MessageQueue<Message> &q);
    void enemyThread();

private:
    GameState *game;
    MessageQueue<Message> &q_;
};