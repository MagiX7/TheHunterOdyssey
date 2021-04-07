#pragma once

#include "Player.h"

class SDL_Texture;

class Thief : public Player
{
public:
	Thief(iPoint position);
	virtual ~Thief();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	void HandleInput(float dt) override;

	bool SaveState(pugi::xml_node& node) override;

	void Attack(Enemy* enemy) override;

	void GetDamage(int dmg) override;

	void Ability(Enemy* enemy, int currentAbility) override;

	void UseObject(int currentObject) override;


private:
	SDL_Texture* texture;
};