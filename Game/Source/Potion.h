#pragma once

#include "Item.h"

class Potion : public Item
{
public:
	Potion(iPoint pos, SDL_Texture* atlas, eastl::string map);
	virtual ~Potion();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

	bool SaveState(pugi::xml_node& n) override;

	void UseItem(Player* player) override;

public:

	int healAmount;
};