#pragma once

#include "Npc.h"

class SDL_Texture;

class NpcWizard : public Npc
{
public:
	NpcWizard(iPoint position);
	virtual ~NpcWizard();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;



	bool SaveState(pugi::xml_node& node) override;


private:
	SDL_Texture* texture;
};
