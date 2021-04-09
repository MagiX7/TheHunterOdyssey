#pragma once

#include "Entity.h"
#include"Point.h"
#define SPEED_X 200.0f
#define SPEED_Y 200.0f

class SDL_Texture;
class Enemy;
class BattleMenu;
class GamePad;

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
	BATTLE
};

class Player : public Entity
{
public:
	Player(PlayerType t, EntityType type, iPoint position);

	virtual ~Player();

	virtual bool Load() override;
	
	virtual bool Update(float dt) override;

	virtual void Draw(bool showColliders) override;

	virtual void HandleInput(float dt, GamePad& pad);

	virtual bool SaveState(pugi::xml_node& node);

	virtual void Attack(Enemy* enemy);

	virtual void Ability(Enemy* enemy, int currentAbility);
	
	virtual void UseObject(Player* player, int currentObject);
	
	virtual void GetDamage(int dmg);

	virtual void GetHealed(int heal);

	virtual void SetDefend(bool option);

	// Getters
	int GetHealthPoints() const { return healthPoints; }
	int GetManaPoints() const { return manaPoints; }
	int GetAbilitySelected() const { return currentAbilitySelected; }
	int GetObjectSelected() const { return currentObjectSelected; }
	bool GetDefend() const { return isDefending; }
	std::string GetName() const { return name; }

	// Setters
	void SetAbilitySelected(int num) { currentAbilitySelected = num; }
	void SetObjectSelected(int num) { currentObjectSelected = num; }

public:
	PlayerType playerType;
	PlayerStance stance;

protected:
	SDL_Texture* texture;

	std::string name;

	bool isDefending;

	int healthPoints;
	int manaPoints;
	int meleeDamage;
	int magicDamage;
	int defense;
	int currentAbilitySelected;
	int currentObjectSelected;
};