#pragma once

#include "Font.h"
#include "Menu.h"
#include "GuiButton.h"

enum class BattleState
{
	NONE = 0,
	DEFAULT,
	ATTACK,
	ABILITY_SELECT,
	ABILITY,
	DEFENSE,
	OBJECT,
	ENEMY_TURN
};

class SceneBattle;
class Player;
class Enemy;

class BattleMenu : public Menu
{
public:
	BattleMenu(SceneBattle* s);
	virtual ~BattleMenu();

	bool Load(Font* font) override;

	bool Update(float dt) override;

	void Draw(Font* font, bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:
	void DrawStats(Font* font);

	bool HandleInput(Input* input);

	bool HandleAbilities(Input* input, int currentAbility);

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

	GuiButton* btnAbilitySlot1;
	GuiButton* btnAbilitySlot2;
	GuiButton* btnAbilitySlot3;
	GuiButton* btnAbilitySlot4;

	SceneBattle* sceneBattle;
};