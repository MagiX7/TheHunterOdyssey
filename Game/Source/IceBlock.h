#pragma once
#include "Entity.h"

#define FIND_RADIOUS 0

class IceBlock : public Entity
{
public:
	IceBlock(EntityType type, iPoint position, Player* player);
	virtual ~IceBlock();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders) override;
	bool UnLoad() override;
	bool SaveState(pugi::xml_node& node);

	bool CheckCollision(Player* player) override;

	void SetInactive() override;
	void SetActive() override;
	inline void SetState(EntityState myState) { state = myState; }
	void SetBounds(SDL_Rect myBounds) override;
	inline EntityState GetState() override { return state; }

	EntityState GetNewState();

	void OnCollision();

public:
	EntityState state;
	EntityState tmpState;
	Animation idleAnim;
	Player* currPlayer;
	bool isMoving;
	bool pushed;
	SDL_Rect lastPosition;
};