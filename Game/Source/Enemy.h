#pragma once

#include "Entity.h"

enum class EnemyState
{
	NONE = 0,
	ROAMING,
	NORMAL,
	ATTACKING,
	ATTACK_FINISHED
};

class Player;

class Enemy : public Entity
{
public:
	Enemy(EntityType type) : Entity(type){}

	virtual ~Enemy(){}

	virtual void GetDamage(int dmg) {}
	
	virtual void Attack(Player* player) {}

	virtual void Travel(iPoint destination, float dt) {}

	virtual inline bool AnimationFinished()
	{
		return this->deathAnim.HasFinished();
	}

	virtual bool SaveState(pugi::xml_node& node)
	{
		pugi::xml_node auxiliar1 = node.append_child("bounds");
		auxiliar1.append_attribute("X").set_value(bounds.x);
		auxiliar1.append_attribute("Y").set_value(bounds.y);
		auxiliar1.append_attribute("W").set_value(bounds.w);
		auxiliar1.append_attribute("H").set_value(bounds.h);

		return true;
	}

	// Setters
	inline void SetCurrentState(EnemyState state) { currentState = state; }
	
	void SetPos(iPoint pos) 
	{ 
		bounds.x = pos.x; 
		bounds.y = pos.y;
		battlePos = pos;
	}

	// Getters
	inline EnemyState GetCurrentState() const { return currentState; }
	
	inline int GetHealth() const { return health; }

protected:
	// Basic stats for enemies
	int health;
	int mana;
	int damage;
	int defense;
	float speed;

	// Starting position when battling
	iPoint battlePos;

	// currentState works for the different states of the enemy
	EnemyState currentState;

	// Enemy target to attack
	Player* target;

	Animation idleAnim;
	Animation deathAnim;
	Animation hitAnim;
	Animation attackAnim;
};