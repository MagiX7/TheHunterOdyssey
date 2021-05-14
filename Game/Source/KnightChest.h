#pragma once

#include "Armor.h"

class KnightChest : public Armor
{
public:
	KnightChest(SDL_Rect b, iPoint(pos), SDL_Texture* atlas) : Armor(pos, atlas)
	{
		bounds = b;
		armorType = ArmorType::HELMET;
	}
	virtual ~KnightChest() {}

	bool Load() override;
	void Draw(bool showColliders) override;

public:
	int armorPoints;
	int durability;
};