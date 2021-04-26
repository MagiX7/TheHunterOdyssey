#pragma once

#include "Item.h"

class FairyTear : public Item
{
public:
	FairyTear();
	virtual ~FairyTear();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders, SDL_Rect bounds);
	bool UnLoad() override;

public:

};
