#pragma once

#include "Player.h"

class Thief : public Player
{
public:
	Thief(iPoint position, pugi::xml_node anim);
	virtual ~Thief();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool SaveState(pugi::xml_node& node) override;

	void Attack(Enemy* enemy) override;

	void GetDamage(int dmg) override;

	void Ability(Enemy* enemy, int currentAbility) override;

	void UseObject(Player* player, int currentObject) override;

	void SetDefend(bool option) override;

	void SetIdleRoaming() { currentAnim = &idleDown; }

protected:
	void HandleInput(float dt) override;
	void Travel(iPoint destination, float dt) override;

private:

};