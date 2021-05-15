#pragma once

#include "Item.h"

class FairyBottle : public Item
{
public:
	FairyBottle(iPoint pos, SDL_Texture* atlas, eastl::string map);
	virtual ~FairyBottle();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

	bool SaveState(pugi::xml_node& n) override;

public:

};