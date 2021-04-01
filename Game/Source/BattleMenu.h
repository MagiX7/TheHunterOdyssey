#pragma once

#include "Font.h"
#include "Menu.h"
#include "GuiButton.h"

enum class BattleState
{
	NONE = 0,
	DEFAULT,
	ATTACK,
	ABILITY,
	DEFENSE,
	OBJECT
};

class Player;
class Enemy;

class BattleMenu : public Menu
{
public:
	BattleMenu(eastl::list<Player*> list, eastl::list<Enemy*> enList);
	virtual ~BattleMenu();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:
	void DrawStats();
	void HandleInput(Input* input);

private:
	BattleState type;
	eastl::list<Player*> playerList;
	eastl::list<Enemy*> enemyList;

	Enemy* currEnemy;
	Player* currPlayer;

	GuiButton* btnAttack;
	GuiButton* btnAbility;
	GuiButton* btnDefense;
	GuiButton* btnObject;

	Font* font;
};