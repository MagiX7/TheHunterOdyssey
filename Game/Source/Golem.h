#pragma once

#include "Enemy.h"

class Font;


class Golem : public Enemy
{
public:
	Golem(iPoint pos);
	virtual ~Golem();

	bool Load() override;

	bool Update(float dt) override;

	bool CheckCollisions() override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool LoadState(pugi::xml_node& node) override;

	bool SaveState(pugi::xml_node& node) override;

private:
	Font* font;

};