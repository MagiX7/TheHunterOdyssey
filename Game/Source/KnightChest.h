#pragma once

#include "Armor.h"

class KnightChest : public Armor
{
public:
	KnightChest(SDL_Rect b, iPoint(pos), SDL_Texture* atlas, eastl::string map) : Armor(pos, atlas)
	{
		bounds = b;
		armorType = ArmorType::HELMET;
		mapName = map;
	}
	virtual ~KnightChest() {}

	bool Load() override;
	void Draw(bool showColliders) override;

public:
	int armorPoints;
	int durability;
};