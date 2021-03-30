#pragma once

#include "Player.h"

class SDL_Texture;

class Hunter : public Player
{
public:
	Hunter(iPoint position);
	virtual ~Hunter();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	void HandleInput(float dt) override;

	bool SaveState(pugi::xml_node& node) override;


private:
	SDL_Texture* texture;
};