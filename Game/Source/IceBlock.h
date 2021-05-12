#pragma once
#include "Entity.h"

#define FIND_RADIOUS 10

class IceBlock : public Entity
{
public:
	IceBlock(EntityType type, iPoint position);
	virtual ~IceBlock();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders) override;
	bool UnLoad() override;
	bool SaveState(pugi::xml_node& node);

	bool CheckCollision(Player* player) override;

	void SetInactive() override;
	void SetActive() override;
	inline EntityState GetState() override { return state; }

	EntityState GetNewState();

	void OnCollision();

public:
	EntityState state;
	Animation idleAnim;

	SDL_Rect lastPosition;
};