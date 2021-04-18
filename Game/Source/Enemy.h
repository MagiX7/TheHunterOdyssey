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

	// Setters
	void SetCurrentState(EnemyState state) { currentState = state; }

	// Getters
	EnemyState GetCurrentState() const { return currentState; }
	
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