#pragma once

#include "Npc.h"

class Merchant : public Npc
{
public:
	Merchant(iPoint position, pugi::xml_node anim, int id);

	virtual ~Merchant();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;
};