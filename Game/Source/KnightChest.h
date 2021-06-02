#pragma once

#include "Armor.h"

class KnightChest : public Armor
{
public:
	KnightChest(SDL_Rect b, iPoint(pos), SDL_Texture* atlas, eastl::string map = "") : Armor(pos, atlas)
	{
		bounds = b;
		armorType = ArmorType::CHEST;
		mapName = map;
		itemType = ItemType::NONE;
	}
	virtual ~KnightChest() {}

	bool Load() override;
	void Draw(bool showColliders) override;

	bool SaveState(pugi::xml_node& node) override;

public:
	int armorPoints;
	int durability;
};