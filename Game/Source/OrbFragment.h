#pragma once

#include "Item.h"

class OrbFragment : public Item
{
public:
	OrbFragment(iPoint pos, SDL_Texture* atlas, eastl::string map);
	virtual ~OrbFragment();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

	bool SaveState(pugi::xml_node& n) override;
};