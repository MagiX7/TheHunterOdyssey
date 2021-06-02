#pragma once

#include "Item.h"

class OmniPotion : public Item
{
public:
	OmniPotion(iPoint pos, SDL_Texture* atlas, eastl::string map = "");
	virtual ~OmniPotion();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

	bool SaveState(pugi::xml_node& n) override;

public:

};