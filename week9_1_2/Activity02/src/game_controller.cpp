#include "game_controller.hpp"
#include <mutex>
#include <chrono>
#include <iostream>
#include <unistd.h>

std::vector<Vec2> current_bullet_data;
std::vector<Vec2> current_enemies_data;

GameController::GameController(GameState *game) : game(game) {};

void GameController::gameControllerThread()
{
    while (game->running)
    {
        // Wait a fixed time
        // std::this_thread::sleep_for(std::chrono::milliseconds(125));

        std::unique_lock<std::mutex> lock(game->gameMutex);
        // Wait for enemy or bullet for max 125 ms
        // game->cv.wait_for(lock, std::chrono::milliseconds(125));

        // Wait for enemy or bullet
        game->cv.wait(lock, [&]
                      { return game->enemyHasMoved_flag || game->bulletsHasMoved_flag; });

        // Collision detection
        for (auto b = game->bullets.begin(); b != game->bullets.end();)
        {
            bool hit = false;
            for (auto e = game->enemies.begin(); e != game->enemies.end();)
            {
                if ((b->x == e->x) && (abs(b->y - e->y) < enemyHeight))
                {
                    e = game->enemies.erase(e);
                    // game->score++;
                    hit = true;
                }
                else
                {
                    ++e;
                }
            }
            if (hit)
            {
                b = game->bullets.erase(b);
            }
            else
            {
                ++b;
            }
        }

        // Game over condition
        for (auto &e : game->enemies)
        {
            if (e.x <= 0)
            {
                game->running = false;
            }
        }
        game->enemyHasMoved_flag = false;
        game->bulletsHasMoved_flag = false;
    }
}

static void handle(const BulletData &data)
{
    current_bullet_data = data.bullets;
    std::cout << "Bullet vector data: ";
    for (auto i : data.bullets)
    {
        std::cout << "(" << i.x << "," << i.y << "), ";
    }
    std::cout << "\n";
}

static void handle(const EnemiesData &data)
{
    current_enemies_data = data.enemies;
    std::cout << "Enemies vector data: ";
    for (auto i : data.enemies)
    {
        std::cout << "(" << i.x << "," << i.y << "), ";
    }
    std::cout << "\n";
}

MqGameController::MqGameController(GameState *game, MessageQueue<Message> &queue) : game(game), queue(queue) {}

void MqGameController::gameControllerThread()
{
    while (game->running)
    {
        Message temp;
        queue.pop(temp);
        std::visit([](auto &&data)
                   { handle(data); }, temp.data);
        if (!current_enemies_data.empty() && !current_bullet_data.empty())
        {
            for (auto e = current_enemies_data.begin(); e != current_enemies_data.end();)
            {
                for (auto b = current_bullet_data.begin(); b != current_bullet_data.end();)
                {
                    if ((b->x == e->x) && (abs(b->y - e->y) < enemyHeight))
                    {
                        std::cout << "Enemy hit at (" << b->x << ", " << b->y << ") \n";
                        sleep(2);
                    }
                    ++b;
                }
                ++e;
            }
        }
    }
}
