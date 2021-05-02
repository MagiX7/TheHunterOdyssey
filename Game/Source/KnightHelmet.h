#pragma once

#include "Item.h"

class KnightHelmet : public Item
{
public:
	KnightHelmet(SDL_Rect b, iPoint(pos), SDL_Texture* atlas) : Item(ItemType::KNIGHT_HELMET, pos, atlas)
	{
		bounds = b;
	}
	virtual ~KnightHelmet() {}
	
	bool Load() override;
	void Draw(bool showColliders) override;

public:
	int armorPoints;
	int durability;
};