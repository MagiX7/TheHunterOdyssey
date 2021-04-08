#pragma once

#include "Player.h"

class GuiButton;

class Wizard : public Player
{
public:
	Wizard(iPoint position);
	virtual ~Wizard();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	void HandleInput(float dt) override;

	bool SaveState(pugi::xml_node& node) override;

	void Attack(Enemy* enemy) override;

	void GetDamage(int dmg) override;
	
	void Ability(Enemy* enemy, int currentAbility) override;

	void UseObject(Player* player, int currentObject) override;

};