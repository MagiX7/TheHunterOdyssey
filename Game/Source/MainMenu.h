#pragma once

#include "Menu.h"

class SceneTitle;

enum class MenuState
{
	NONE = 0,
	NORMAL,
	OPTIONS,
	CREDITS,
	EXIT,
	GLOBAL_EXIT,
};

class GuiButton;

class MainMenu : public Menu
{
public:
	MainMenu(SceneTitle* s);

	virtual ~MainMenu();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:
	MenuState state;
	SceneTitle* scene;
	
	// Buttons declaration
	GuiButton* btnNewGame;
	GuiButton* btnContinue;
	GuiButton* btnOptions;
	GuiButton* btnCredits;
	GuiButton* btnExit;
	GuiButton* btnExitYes;
	GuiButton* btnExitNo;
	GuiButton* btnOptionsBack;
	GuiButton* btnCreditsBack;
};