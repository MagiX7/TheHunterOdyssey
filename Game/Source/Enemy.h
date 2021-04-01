#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(EntityType type) : Entity(type){}

	virtual ~Enemy(){}

private:
	float health;
	float mana;
	
};