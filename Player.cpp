#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Constants.h"
#include "Enemy.h"
#include "Ufo.h"
#include "Player.h"

using namespace std;

Player::Player() :
	explosion(EXPLOSION_ANIMATION_SPEED, BASE_SIZE, "Explosion.png")
{
	reset();

	bullet_texture.loadFromFile("PlayerBullet.png");
	texture.loadFromFile("Player.png");

	bullet_sprite.setTexture(bullet_texture);
	sprite.setTexture(texture);
}

bool Player::get_dead() const
{
	return dead;
}

bool Player::get_dead_animation_over() const
{
	return dead_animation_over;
}

unsigned short Player::get_y() const
{
	return y;
}

void Player::die()
{
	dead = 1;
}

void Player::draw(sf::RenderWindow& i_window)
{
	if (0 == dead)
	{
		sprite.setPosition(x, y);
		sprite.setTextureRect(sf::IntRect(BASE_SIZE, 0, BASE_SIZE, BASE_SIZE));

		for (const Bullet& bullet : bullets)
		{
			bullet_sprite.setPosition(bullet.x, bullet.y);

			i_window.draw(bullet_sprite);
		}

		i_window.draw(sprite);

	}
	else if (0 == dead_animation_over)
	{
		explosion.draw(x, y, i_window, sf::Color(255, 36, 0));
	}
}

void Player::reset()
{
	dead = 0;
	dead_animation_over = 0;

	current_power = 0;
	reload_timer = 0;

	x = 0.5f * (SCREEN_WIDTH - BASE_SIZE);
	y = SCREEN_HEIGHT - 2 * BASE_SIZE;

	bullets.clear();

	explosion.reset();
}

void Player::update( mt19937_64& i_random_engine,  vector<Bullet>& i_enemy_bullets,  vector<Enemy>& i_enemies, Ufo& i_ufo, unsigned short& i_score)
{
	if (0 == dead){

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{

			x =  max<int>(x - PLAYER_MOVE_SPEED, BASE_SIZE);
			
		}

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			x =  min<int>(PLAYER_MOVE_SPEED + x, SCREEN_WIDTH - 2 * BASE_SIZE);
			
		}

		if (0 == reload_timer)
		{
			if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
			    reload_timer = RELOAD_DURATION;

				bullets.push_back(Bullet(0, -PLAYER_BULLET_SPEED, x, y));
			}
		}
		else
		{
			reload_timer--;
		}

		for (Bullet& enemy_bullet : i_enemy_bullets)
		{
			if (1 == get_hitbox().intersects(enemy_bullet.get_hitbox())){
			    dead = 1;

				enemy_bullet.dead = 1;

				break;
			}
		}
	}
	else if (0 == dead_animation_over)
	{
        dead_animation_over = explosion.update();
	}

	for (Bullet& bullet : bullets)
	{
		bullet.update();

		if (0 == bullet.dead)
		{
			if (1 == i_ufo.check_bullet_collision(i_random_engine, bullet.get_hitbox(), i_score))
			{
				bullet.dead = 1;
			}
		}
	}

	for (Enemy& enemy : i_enemies)
	{
		for (Bullet& bullet : bullets)
		{
			if (0 == bullet.dead && 0 < enemy.get_health() && 1 == enemy.get_hitbox().intersects(bullet.get_hitbox()))
			{
				bullet.dead = 1;

				enemy.hit();

				break;
			}
		}
	}

	bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& i_bullet)
		{
			return 1 == i_bullet.dead;
		}), bullets.end());
}

sf::IntRect Player::get_hitbox() const
{
	return sf::IntRect(x + 0.125f * BASE_SIZE, y + 0.125f * BASE_SIZE, 0.75f * BASE_SIZE, 0.75f * BASE_SIZE);
}
