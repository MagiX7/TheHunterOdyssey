#pragma once

#include "Item.h"

class InterruptorBlock : public Item
{
public:
	InterruptorBlock(iPoint pos, SDL_Texture* atlas);
	virtual ~InterruptorBlock();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders);
	bool UnLoad() override;
};