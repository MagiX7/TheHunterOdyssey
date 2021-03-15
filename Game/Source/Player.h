#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();

	virtual ~Player();

	bool Load() override;
	
	bool Update(float dt) override;

	void Draw(bool showColliders) override;

private:

};