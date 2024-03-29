#pragma once

#include "Npc.h"

class NpcWizard : public Npc
{
public:
	NpcWizard(iPoint position, pugi::xml_node anim, int id);
	virtual ~NpcWizard();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool SaveState(pugi::xml_node& node) override;

private:

};