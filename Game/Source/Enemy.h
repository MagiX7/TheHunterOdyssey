#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(EntityType type) : Entity(type){}

	virtual ~Enemy(){}

protected:
	float health;
	float mana;
	float damage;
	float defense;
	float speed;
	
};