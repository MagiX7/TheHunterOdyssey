#pragma once

#include "Item.h"

class UltraPotion : public Item
{
public:
	UltraPotion(iPoint pos, SDL_Texture* atlas, eastl::string map);
	virtual ~UltraPotion();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

	void UseItem(Player* player) override;

public:

	int healAmount;
};
