#pragma once

#include "Item.h"

class OmniPotion : public Item
{
public:
	OmniPotion();
	virtual ~OmniPotion();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders, SDL_Rect bounds);
	bool UnLoad() override;

public:

};