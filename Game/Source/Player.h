#pragma once

#include "Entity.h"

enum class PlayerType
{
	NONE = 0,
	HUNTER,
	WIZARD
};

class Player : public Entity
{
public:
	Player(PlayerType pType);

	virtual ~Player();

	bool Load() override;
	
	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	void HandleInput(float dt);

private:

	PlayerType type;
};