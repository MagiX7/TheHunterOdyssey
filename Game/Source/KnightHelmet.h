#pragma once

#include "Armor.h"

class KnightHelmet : public Armor
{
public:
	KnightHelmet(SDL_Rect b, iPoint(pos), SDL_Texture* atlas, eastl::string map = "") : Armor(pos, atlas)
	{
		bounds = b;
		armorType = ArmorType::HELMET;
		mapName = map;
		itemType = ItemType::NONE;
	}
	virtual ~KnightHelmet() {}
	
	bool Load() override;
	void Draw(bool showColliders) override;

	bool SaveState(pugi::xml_node& node) override;

public:
	int armorPoints;
	int durability;
};