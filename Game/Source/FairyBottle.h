#pragma once

#include "Item.h"

class FairyBottle : public Item
{
public:
	FairyBottle();
	virtual ~FairyBottle();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders, SDL_Rect bounds);
	bool UnLoad() override;

public:

};