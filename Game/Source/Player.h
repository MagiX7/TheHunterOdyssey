#pragma once

#include "Entity.h"
#include"Point.h"
#define SPEED_X 200.0f
#define SPEED_Y 200.0f

class SDL_Texture;

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

	virtual void HandleInput(float dt);

	virtual bool SaveState(pugi::xml_node& node);


	int GetHealthPoints() const { return healthPoints; }
	int GetManaPoints() const { return manaPoints; }
	std::string GetName() const { return name; }

public:
	PlayerType playerType;
	PlayerStance stance;

protected:
	SDL_Texture* texture;

	int healthPoints;
	int manaPoints;

	std::string name;
};