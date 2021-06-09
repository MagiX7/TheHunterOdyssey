#pragma once

#include "Entity.h"
#include "Armor.h"

#include "Point.h"
#include "Log.h"
#include "ParticlesManager.h"
#include "EASTL/string.h"

#define SPEED_X 200.0f
#define SPEED_Y 200.0f

struct SDL_Texture;
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
	void startPaarticles(iPoint pos,int particle);
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

	virtual bool CanUseAbility(int abilityNum);

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

	const char* GetAbilityName(int abilityNum) const { return abilityName[abilityNum]; }
	int GetAbilityManaCost(int abilityNum) { return abilityCost[abilityNum]; }

	Armor* GetHelmet() const 
	{
		if (helmet) return helmet;

		return nullptr;
	}
	Armor* GetChest() const 
	{
		if (chest) return chest; 

		return nullptr;
	}
	Armor* GetBoots() const
	{
		if (boots) return boots; 

		return nullptr;
	}
	
	// Setters
	inline void SetAbilitySelected(int num) { currentAbilitySelected = num; }
	inline void SetObjectSelected(int num) { currentObjectSelected = num; }

	Armor* SetEquipment(Armor* armor)
	{
		Armor* ret = nullptr;

		if (armor->armorType == ArmorType::HELMET)
		{
			if (helmet == nullptr) helmet = armor;
			else
			{
				ret = helmet;
				helmet = armor;
			}
		}
		else if (armor->armorType == ArmorType::CHEST)
		{
			if (chest == nullptr) chest = armor;
			else
			{
				ret = chest;
				chest = armor;
			}
		}
		else if(armor->armorType == ArmorType::BOOTS)
		{
			if (boots == nullptr) boots = armor;
			else
			{
				ret = boots;
				boots = armor;
			}
		}
		return ret;
	}

	void UnequipArmor(Armor* armor)
	{
		if (armor->armorType == ArmorType::HELMET) helmet = nullptr;
		else if (armor->armorType == ArmorType::CHEST) chest = nullptr;
		else if (armor->armorType == ArmorType::BOOTS) boots = nullptr;
	}


protected:
	virtual void HandleInput(float dt);
	virtual void Travel(iPoint destination, float dt) {}

public:
	//int particleNum;
	PlayerType playerType;
	PlayerStance stance;
	iPoint battlePos;
	Generator* generator;
	static int gold;
	bool canMove;

protected:
	SDL_Texture* battlerTexture;
	ParticlesManager* particles;
	eastl::string name;

	Armor* helmet;
	Armor* boots;
	Armor* chest;

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

	//Abilities data
	const char* abilityName[4];
	int abilityCost[4];
};