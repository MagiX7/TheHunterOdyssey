#pragma once

#include "Item.h"

class FairyWing : public Item
{
public:
	FairyWing();
	virtual ~FairyWing();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders, SDL_Rect bounds);
	bool UnLoad() override;

public:

};
