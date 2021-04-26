#pragma once

#include "Item.h"

class UltraPotion : public Item
{
public:
	UltraPotion();
	virtual ~UltraPotion();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders, SDL_Rect bounds);
	bool UnLoad() override;

public:

};
