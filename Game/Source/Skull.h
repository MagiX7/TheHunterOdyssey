#pragma once

#include "Enemy.h"

class Skull : public Enemy
{
public:
	Skull(iPoint pos);
	virtual ~Skull();

	bool Load() override;

	bool Update(float dt) override;

	bool CheckCollisions() override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool LoadState(pugi::xml_node& node) override;

	bool SaveState(pugi::xml_node& node) override;

private:

};