#pragma once

#include "Npc.h"

class SDL_Texture;

class Ray : public Npc
{
public:
	Ray(iPoint position, pugi::xml_node anim, int id);
	virtual ~Ray();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;



	bool SaveState(pugi::xml_node& node) override;


private:
	SDL_Texture* texture;
};
