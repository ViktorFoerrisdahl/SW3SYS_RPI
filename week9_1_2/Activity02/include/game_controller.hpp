#pragma once
#include "game_state.hpp"
#include "game_types.hpp"

class GameController {
public:
    explicit GameController(GameState *game);

    void gameControllerThread();

private:
    GameState *game;
};

class MqGameController {
public:
    explicit MqGameController(GameState *game, MessageQueue<Message> &queue);

    void gameControllerThread();

private:
    GameState *game;
    MessageQueue<Message> &queue;
};