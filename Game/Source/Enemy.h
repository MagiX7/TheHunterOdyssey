#pragma once

#include "Entity.h"

class Player;

class Enemy : public Entity
{
public:
	Enemy(EntityType type) : Entity(type){}

	virtual ~Enemy(){}

	virtual void GetDamage(int dmg) {}
	
	virtual void Attack(Player* player) {}

	int GetHealth() const { return health; }

protected:
	int health;
	int mana;
	int damage;
	int defense;
	float speed;
	
};