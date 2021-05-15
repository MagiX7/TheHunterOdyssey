#pragma once

#include "Item.h"

class FairyWing : public Item
{
public:
	FairyWing(iPoint pos, SDL_Texture* atlas, eastl::string map);
	virtual ~FairyWing();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

public:
};
