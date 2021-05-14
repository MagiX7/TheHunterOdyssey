#pragma once

#include "Point.h"

#include "Entity.h"

class Statue : public Entity
{
public:
	Statue(EntityType type, iPoint position, Player* player, int id);
	virtual ~Statue();

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

	SDL_Rect lastPosition;
};