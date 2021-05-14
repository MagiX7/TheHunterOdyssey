#pragma once

#include "Item.h"

class OrbFragment : public Item
{
public:
	OrbFragment(iPoint pos, SDL_Texture* atlas);
	virtual ~OrbFragment();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;
};