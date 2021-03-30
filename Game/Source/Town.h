#pragma once

#include "Npc.h"

class SDL_Texture;

class Town : public Npc
{
public:
	Town(iPoint position);
	virtual ~Town();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	

	bool SaveState(pugi::xml_node& node) override;


private:
	SDL_Texture* texture;
};
