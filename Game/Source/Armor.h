#pragma once

#include "Entity.h"

class Armor : public Entity
{
public:
	Armor(SDL_Rect rectBounds, SDL_Texture* atlas)
	{
		bounds = rectBounds;
		texture = atlas;
	}
	virtual ~Armor(){}

	virtual bool Load() { return true; }

protected:
	int armorPoints;
	int durability;
	SDL_Texture* texture;
	SDL_Rect atlasSection;
};