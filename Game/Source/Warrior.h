#pragma once

#include "Player.h"

class Warrior : public Player
{
public:
	Warrior(iPoint position, pugi::xml_node anim, ParticlesManager* particles);
	virtual ~Warrior();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool SaveState(pugi::xml_node& node) override;

	void Attack(Enemy* enemy) override;

	void GetDamage(int dmg) override;

	void GetHealed(int heal) override;

	void GetMana(int amount) override;

	void Ability(Enemy* enemy, int currentAbility) override;

	void UseObject(Player* player, int currentObject) override;

	inline void SetDefend(bool option) override { isDefending = option; }

	void SetIdleRoaming()
	{
		currentAnim = &idleDown;
	}

protected:
	void HandleInput(float dt) override;
	void Travel(iPoint destination, float dt) override;

	int footStepFx;
	int attackFx;
	int dieFx;
	int hurtFx;

	bool isDead;
private:
};