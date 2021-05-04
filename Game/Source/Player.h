#pragma once

#include "Entity.h"
#include "Point.h"
#include "Log.h"

#include "EASTL/string.h"

#define SPEED_X 200.0f
#define SPEED_Y 200.0f

class SDL_Texture;
class Enemy;
class Item;

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
	inline int GetArmorPoints() const { return armorPoints; }
	inline int GetMaxArmorPoints() const { return maxArmorPoints; }
	inline int GetAbilitySelected() const { return currentAbilitySelected; }
	inline int GetObjectSelected() const { return currentObjectSelected; }
	inline bool GetDefend() const { return isDefending; }
	inline eastl::string GetName() const { return name; }
	inline Item* GetHelmet() const {if (helmet) return helmet; }
	inline Item* GetChest() const { if (chest) return chest; }
	inline Item* GetBoots() const { if (boots) return boots; }
	inline Item* GetWeapon() const { if (weapon) return weapon; }
	
	// Setters
	inline void SetAbilitySelected(int num) { currentAbilitySelected = num; }
	inline void SetObjectSelected(int num) { currentObjectSelected = num; }
	Item* SetHelmet(Item* helm) 
	{
		Item* ret = nullptr;

		if (helmet == nullptr)
		{
			helmet = helm;
		}
		else
		{
			ret = helmet;
			helmet = helm;
		}
		return ret;
	}
	Item* SetChest(Item* che) 
	{ 
		Item* ret = nullptr;

		if (chest == nullptr)
		{
			chest = che;
		}
		else
		{
			ret = chest;
			chest = che;
		}
		return ret;
	}
	Item* SetBoots(Item* boot)
	{ 
		Item* ret = nullptr;

		if (boots == nullptr)
		{
			boots = boot;
		}
		else
		{
			ret = boots;
			boots = boot;
		}
		return ret;
	}
	Item* SetWeapon(Item* weap) 
	{
		Item* ret = nullptr;

		if (weapon == nullptr)
		{
			weapon = weap;
		}
		else
		{
			ret = weapon;
			weapon = weap;
		}
		return ret;
	}

protected:
	virtual void HandleInput(float dt);
	virtual void Travel(iPoint destination, float dt) {}

public:
	PlayerType playerType;
	PlayerStance stance;
	iPoint battlePos;

protected:
	SDL_Texture* battlerTexture;

	eastl::string name;

	Item* helmet;
	Item* boots;
	Item* chest;
	Item* weapon;

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
	int armorPoints;
	int maxArmorPoints;
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