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
	OBJECT_SELECT,
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

	bool HandleObjects(Input* input, int currentObject);

	bool HandleDefense(Input* input);

	void EraseEnemy();

	void EnemyTurn();

private:
	BattleState type;

	Enemy* currEnemy;
	Player* currPlayer;

	Player* tempPlayer;

	// Menu buttons
	GuiButton* btnAttack;
	GuiButton* btnAbility;
	GuiButton* btnDefense;
	GuiButton* btnObject;

	// Ability buttons
	GuiButton* btnAbilitySlot1;
	GuiButton* btnAbilitySlot2;
	GuiButton* btnAbilitySlot3;
	GuiButton* btnAbilitySlot4;

	// Object buttons
	GuiButton* btnObjectSlot1;
	GuiButton* btnObjectSlot2;
	GuiButton* btnObjectSlot3;
	GuiButton* btnObjectSlot4;

	SceneBattle* sceneBattle;

	SDL_Texture* guiTex;

	bool enemyKilled;
};