#pragma once

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy() : Entity(EntityType::GOLEM){}
	virtual ~Enemy(){}

private:
	float health;
	float mana;
	
};