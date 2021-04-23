#pragma once

#include "Entity.h"
#include "Point.h"
#include "Log.h"
#include"ParticlesManager.h"
#include "EASTL/string.h"

#define SPEED_X 200.0f
#define SPEED_Y 200.0f

class SDL_Texture;
class Enemy;

enum class PlayerType
{
	NONE = 0,
	HUNTER,
	WIZARD,
	THIEF, 
	WARRIOR
};

enum class PlayerStance
{
	NONE = 0,
	ROAMING,
	BATTLE,
	ATTACKING,
	ATTACK_FINISHED,
	ABILITY,
	ABILITY_FINISHED
};

class Player : public Entity
{
public:
	Player(PlayerType t, EntityType type, iPoint position);

	virtual ~Player();

	virtual bool Load() override;
	
	virtual bool Update(float dt) override;

	virtual void Draw(bool showColliders) override;

	virtual bool SaveState(pugi::xml_node& node);

	virtual void Attack(Enemy* enemy);

	virtual void Ability(Enemy* enemy, int currentAbility);
	
	virtual void UseObject(Player* player, int currentObject);
	
	virtual void GetDamage(int dmg);

	virtual void GetHealed(int heal);

	virtual void GetMana(int amount);

	virtual void SetDefend(bool option);

	virtual void SetIdleBattle() { currentAnim = &idleBattle; }

	virtual void SetIdleRoaming() {}

	// Getters
	inline int GetHealthPoints() const { return healthPoints; }
	inline int GetMaxHealthPoints() const { return maxHealthPoints; }
	inline int GetManaPoints() const { return manaPoints; }
	inline int GetMaxManaPoints() const { return maxManaPoints; }
	inline int GetAbilitySelected() const { return currentAbilitySelected; }
	inline int GetObjectSelected() const { return currentObjectSelected; }
	inline bool GetDefend() const { return isDefending; }
	inline eastl::string GetName() const { return name; }

	// Setters
	inline void SetAbilitySelected(int num) { currentAbilitySelected = num; }
	inline void SetObjectSelected(int num) { currentObjectSelected = num; }

protected:
	virtual void HandleInput(float dt);
	virtual void Travel(iPoint destination, float dt) {}

public:
	PlayerType playerType;
	PlayerStance stance;
	iPoint battlePos;
	Generator* generator;
protected:
	SDL_Texture* battlerTexture;

	eastl::string name;

	// Enemy target
	Enemy* target;

	bool isDefending;
	bool attack;

	int healthPoints;
	int maxHealthPoints;
	int manaPoints;
	int maxManaPoints;
	int meleeDamage;
	int magicDamage;
	int defense;
	int currentAbilitySelected;
	int currentObjectSelected;

	

	//Animations
	Animation idleDown;
	Animation idleLeft;
	Animation idleRight;
	Animation idleUp;
	Animation walkDown;
	Animation walkLeft;
	Animation walkRight;
	Animation walkUp;

	Animation idleBattle;
	Animation death;
	Animation attackAnim;
	Animation damageTaken;
};