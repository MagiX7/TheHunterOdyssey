#pragma once

#include "Entity.h"

#define SPEED_X 200.0f
#define SPEED_Y 200.0f

class SDL_Texture;

enum class PlayerType
{
	NONE = 0,
	HUNTER,
	WIZARD
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
	Player(PlayerType pType);

	virtual ~Player();

	virtual bool Load() override;
	
	virtual bool Update(float dt) override;

	virtual void Draw(bool showColliders) override;

	virtual void HandleInput(float dt);

	virtual bool SaveState(pugi::xml_node& node);

	virtual void SetPlayerType(PlayerType Type);

public:
	SDL_Rect bounds;
	PlayerType playerType;
	PlayerStance stance;

protected:
	SDL_Texture* texture;
};