#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "Enemy.h"

using namespace std;

Enemy::Enemy(unsigned char i_type, unsigned short i_x, unsigned short i_y) :
	direction(0 == (i_y) % 2 ? -1 : 1),
	health(1  + i_type),
	hit_timer(0),
	type(i_type),
	x(i_x),
	y(i_y)
{
}

unsigned char Enemy::get_health() const
{
	return health;
}

unsigned char Enemy::get_hit_timer() const
{
	return hit_timer;
}

unsigned char Enemy::get_type() const
{
	return type;
}

unsigned short Enemy::get_x() const
{
	return x;
}

unsigned short Enemy::get_y() const
{
	return y;
}


void Enemy::hit()
{
	hit_timer = ENEMY_HIT_TIMER_DURATION;
}


void Enemy::move( vector<Enemy>& all_enemies)
{
    bool changeDirection = false;

        if ((1 == direction && x == SCREEN_WIDTH - 2 * BASE_SIZE) || (-1 == direction && x == BASE_SIZE)){
            for (Enemy& enemy : all_enemies)
            {
                enemy.y += ENEMY_MOVE_SPEED; 
                enemy.direction = -enemy.direction; 
                

            }
        }
        else
        {
            //x = clamp<short>(x + ENEMY_MOVE_SPEED * direction, BASE_SIZE, SCREEN_WIDTH - 2 * BASE_SIZE);
        	x = x + ENEMY_MOVE_SPEED * direction;
		}

}





void Enemy::shoot( vector<Bullet>& i_enemy_bullets)
{
	switch (type)
	{
	case 0:
	{
		i_enemy_bullets.push_back(Bullet(0, ENEMY_BULLET_SPEED, x, y));

		break;
	}
	case 1:
	{
		i_enemy_bullets.push_back(Bullet(0.125f * ENEMY_BULLET_SPEED, ENEMY_BULLET_SPEED, x, y));
		i_enemy_bullets.push_back(Bullet(-0.125f * ENEMY_BULLET_SPEED, ENEMY_BULLET_SPEED, x, y));

		break;
	}
	case 2:
	{
		i_enemy_bullets.push_back(Bullet(0, ENEMY_BULLET_SPEED, x, y));
		i_enemy_bullets.push_back(Bullet(0.25f * ENEMY_BULLET_SPEED, ENEMY_BULLET_SPEED, x, y));
		i_enemy_bullets.push_back(Bullet(-0.25f * ENEMY_BULLET_SPEED, ENEMY_BULLET_SPEED, x, y));
	}
	}
}

void Enemy::update()
{
	if (0 < hit_timer)
	{
		if (1 == hit_timer)
		{
			health =  max(0, health - 1);
		}

		hit_timer--;
	}
}

sf::IntRect Enemy::get_hitbox() const
{
	return sf::IntRect(x + 0.25f * BASE_SIZE, y + 0.25f * BASE_SIZE, 0.5f * BASE_SIZE, 0.5f * BASE_SIZE);
}

 vector<Enemy>& Enemy::get_enemies()
{
    return enemies;
}


