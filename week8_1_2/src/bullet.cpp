#include "bullet.hpp"
#include <mutex>
#include <condition_variable>
#include <chrono>

Bullet::Bullet(GameState *game, Buttons *buttons) : game(game), buttons(buttons) {};

void Bullet::bulletInputThread()
{
    auto shootBullet = [&]
    { game->bullets.push_back({game->shipPos.x + 1, game->shipPos.y + (shipHeight / 2)}); };

    while (game->running)
    {
        std::unique_lock<std::mutex> lock(game->gameMutex);

        game->shootButton_cv.wait(lock, [&]
                                  { return game->shootButtonLeft_flag || game->shootButtonRight_flag; });
        game->bullets.push_back({game->shipPos.x + 1, game->shipPos.y + (shipHeight / 2)});
        game->shootButtonLeft_flag = false;
        game->shootButtonRight_flag = false;
        lock.unlock();
    }
}

void Bullet::bulletUpdateThread()
{
    while (game->running)
    {
        // Running at fixed speed (bullet movement)
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        {
            std::unique_lock<std::mutex> lock(game->gameMutex);

            if (game->bulletsHasMoved_flag == false)
            {
                for (auto b = game->bullets.begin(); b != game->bullets.end();)
                {
                    b->x++;
                    ++b; // normal increment
                }
                game->bulletsHasMoved_flag = true;
                game->cv.notify_all(); // sender notify til game_controller, at vores bullet har bevæget sig
            }

            /* TODO: Update bullets (move right)
             * 1) Is locking required?
             * 2) For each game->bulles, move them 1 px horisontally
             * 3) Is sync required?
             */

            // Delete bullet when out of screen
            for (auto b = game->bullets.begin(); b != game->bullets.end();)
            {
                if (b->x > displayWidth)
                {
                    b = game->bullets.erase(b); // get next valid iterator (original b invalidated by erase)
                }
                else
                {
                    ++b; // normal increment if no erase
                }
            }
            lock.unlock();
        }
    }
}
