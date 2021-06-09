#pragma once

#include "Entity.h"
enum class EnemyType
{
	BAT = 0,
	SKULL,
	GOLEM
};

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
	Enemy(EntityType type);

	virtual ~Enemy();

	virtual void GetDamage(int dmg);
	
	virtual void Attack(Player* player);

	virtual void Travel(iPoint destination, float dt);
	

	virtual inline bool AnimationFinished();

	virtual bool SaveState(pugi::xml_node& node);

	// Setters
	 void SetCurrentState(EnemyState state);
	
	void SetPos(iPoint pos);
	void startPaarticles(iPoint pos, int particle);
	// Getters
	EnemyState GetCurrentState() const;
	
	int GetHealth() const;

	EnemyType GetEnemyType() const;

	int GetMaxHealthPoints() const;


public:
	eastl::string mapName;
	
protected:
	// Basic stats for enemies
	int health;
	int maxHealth;
	int mana;
	int damage;
	int defense;
	float speed;
	
	EnemyType eType;
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