#pragma once

using namespace std;

class Ufo
{
	bool dead;
	bool dead_animation_over;

	short explosion_x;
	short x;

	unsigned short timer;
	unsigned short y;

	 uniform_int_distribution<unsigned short> timer_distribution;



	Animation animation;
	Animation explosion;
public:
	Ufo( mt19937_64& i_random_engine);

	bool check_bullet_collision( mt19937_64& i_random_engine, const sf::IntRect& i_bullet_hitbox, unsigned short& i_score);


	void draw(sf::RenderWindow& i_window);
	void reset(bool i_dead,  mt19937_64& i_random_engine);
	void update( mt19937_64& i_random_engine);

	sf::IntRect get_hitbox() const;
};
