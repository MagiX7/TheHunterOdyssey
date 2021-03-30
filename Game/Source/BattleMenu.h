#pragma once

#include "Font.h"
#include "Menu.h"
#include "GuiButton.h"

class Player;

class BattleMenu : public Menu
{
public:
	BattleMenu(eastl::list<Player*> list);
	virtual ~BattleMenu();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:
	void DrawStats();

private:

	eastl::list<Player*> playerList;

	GuiButton* btnAttack;
	GuiButton* btnAbility;
	GuiButton* btnDefense;
	GuiButton* btnObject;

	Font* font;
};