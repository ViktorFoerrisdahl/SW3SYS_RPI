#include "enemy.hpp"
#include <mutex>
#include <chrono>
#include <iostream>

Enemy::Enemy(GameState *game, MessageQueue<Message> &q) : game(game), q_(q) {};

void Enemy::enemyThread()
{
    while (game->running)
    {
        // Fixed speed
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        // Spawn enemies at leftmost column, random row
        if (rand() % 100 == 0)
        {
            game->enemies.push_back({displayWidth, rand() % (displayHeight - 1 - enemyHeight / 2)});
        }

        {
            // Scope must be locked since gameControllerThread
            // may erase enemies and invalidate iterator
            std::lock_guard<std::mutex> lock(game->gameMutex);

            if (game->enemyHasMoved_flag == false)
            {
                // Move enemies left
                for (auto &e : game->enemies)
                {
                    e.x -= 1;
                }
            }
            // Delete enemy when out of screen
            // vector::erase invalidates iterator
            // This pattern gets the next valid one upon erase
            for (auto e = game->enemies.begin(); e != game->enemies.end();)
            {
                if (e->x < -enemyWidth)
                {
                    e = game->enemies.erase(e); // get next valid itr
                }
                else
                {
                    ++e; // normal increment if no erase
                }
            }
            game->enemyHasMoved_flag = true;
        }
        game->cv.notify_all(); // Sender notify til game controller, at vores enemies har bevæget sig
        if (!game->enemies.empty())
        {
            EnemiesData Enemies_data_temp;
            Message tempMSG;
            for (auto b = game->enemies.begin(); b != game->enemies.end();)
            {
                Vec2 tempEnemy;
                tempEnemy.x = b->x;
                tempEnemy.y = b->y;
                Enemies_data_temp.enemies.push_back(tempEnemy);
                ++b;
            }
            tempMSG.data = Enemies_data_temp;
            q_.push(tempMSG);
        }
    }
}
