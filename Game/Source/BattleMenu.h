#pragma once

#include "Font.h"
#include "Menu.h"
#include "GuiButton.h"

enum class BattleState
{
	NONE = 0,
	DEFAULT,
	ATTACK,
	ATTACKING,
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
class Easing;

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

	void UpdatingButtons(Input* input) override;

private:
	void DrawStats(Font* font);

	bool HandleInput(Input* input);

	bool HandleAbilities(Input* input, int currentAbility);

	bool HandleObjects(Input* input, int currentObject);

	bool HandleDefense(Input* input);

	void EraseEnemy();

	void EnemyTurn();

	void DefaultStateButtons();

	void AbilityStateButtons();

	void ObjectStateButtons();
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

	// Easing
	Easing* easingArrow;
	Easing* easingArrowBack;
	float position_x;
	float position_x2;
	Uint8 alpha;

	Easing* playerEasing;
	Easing* enemyEasing;
	Easing* playerEasing2;
	Easing* enemyEasing2;
	float playerPos;
	float enemyPos;

	SceneBattle* sceneBattle;

	SDL_Texture* guiTex;

	bool enemyKilled;
};