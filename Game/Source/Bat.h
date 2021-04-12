#pragma once

#include "Enemy.h"

class Font;

class Bat : public Enemy
{
public:
	Bat(iPoint pos);
	virtual ~Bat();

	bool Load() override;

	bool Update(float dt) override;

	bool CheckCollisions() override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool LoadState(pugi::xml_node& node) override;

	bool SaveState(pugi::xml_node& node) override;

	void GetDamage(int dmg) override;

	void Attack(Player* player) override;

	void Travel(iPoint destination, float dt) override;

private:
	Font* font;

	bool attack;
};