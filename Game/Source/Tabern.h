#pragma once
#include "Npc.h"

class SDL_Texture;

class Tabern : public Npc
{
public:
	Tabern(iPoint position);
	virtual ~Tabern();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;



	bool SaveState(pugi::xml_node& node) override;


private:
	SDL_Texture* texture;
};
