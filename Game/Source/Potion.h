#pragma once

#include "Item.h"

class Potion : public Item
{
public:
	Potion();
	virtual ~Potion();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders, SDL_Rect bounds);
	bool UnLoad() override;

public:

};