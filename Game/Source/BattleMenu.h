#pragma once

#include "Font.h"
#include "Menu.h"
#include "GuiButton.h"

class SceneBattle;

enum class BattleState
{
	NONE = 0,
	DEFAULT,
	ATTACK,
	ABILITY,
	DEFENSE,
	OBJECT,
	ENEMY_TURN
};

class Player;
class Enemy;

class BattleMenu : public Menu
{
public:
	BattleMenu(SceneBattle* s);
	virtual ~BattleMenu();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:
	void DrawStats();

	bool HandleInput(Input* input);

	void EraseEnemy();

	void EnemyTurn();

private:
	BattleState type;

	Enemy* currEnemy;
	Player* currPlayer;

	GuiButton* btnAttack;
	GuiButton* btnAbility;
	GuiButton* btnDefense;
	GuiButton* btnObject;

	Font* font;

	SceneBattle* sceneBattle;
};