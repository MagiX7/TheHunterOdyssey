#pragma once

#include "Item.h"

class Potion : public Item
{
public:
	Potion(iPoint pos);
	virtual ~Potion();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

	void UseItem(Player* player) override;

public:

	int healAmount;

};