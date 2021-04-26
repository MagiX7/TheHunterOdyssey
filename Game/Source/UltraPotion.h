#pragma once

#include "Item.h"

class UltraPotion : public Item
{
public:
	UltraPotion(iPoint pos);
	virtual ~UltraPotion();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

public:

};
