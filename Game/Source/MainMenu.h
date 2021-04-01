#pragma once

#include "Menu.h"
#include "Font.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
class SceneTitle;

enum class MenuState
{
	NONE = 0,
	NORMAL,
	OPTIONS,
	CREDITS,
	EXIT
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

	SDL_Texture* guiTex;
	SDL_Texture* bg;
	
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
	GuiCheckBox* checkfullscreen;
	GuiCheckBox* checkVSync;

	Font* font;
};