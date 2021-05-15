#pragma once

#include "Item.h"

class FairyTear : public Item
{
public:
	FairyTear(iPoint pos, SDL_Texture* atlas, eastl::string map);
	virtual ~FairyTear();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;

public:
};
