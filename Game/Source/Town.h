#pragma once

#include "Npc.h"

class Town : public Npc
{
public:
	Town(iPoint position, pugi::xml_node anim, int id);
	virtual ~Town();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool SaveState(pugi::xml_node& node) override;

};