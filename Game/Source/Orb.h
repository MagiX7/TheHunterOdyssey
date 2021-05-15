#pragma once

#include "Item.h"

class Orb : public Item
{
public:
	Orb(iPoint pos, SDL_Texture* atlas, eastl::string map);
	Orb(SDL_Texture* atlas);
	virtual ~Orb();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

	bool SaveState(pugi::xml_node& n) override;
};