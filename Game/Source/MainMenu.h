#pragma once

#include "Menu.h"
#include "Font.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

class SceneTitle;
class Easing;

enum class MenuState
{
	NONE = 0,
	NORMAL,
	OPTIONS,
	CREDITS,
	EXIT
};

class GuiButton;
class Easing;

class MainMenu : public Menu
{
public:
	MainMenu(SceneTitle* s);

	virtual ~MainMenu();

	bool Load(Font* font) override;

	bool Update(float dt) override;

	void Draw(Font* font, bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

	void UpdatingButtons(Input* input) override;

private:
	MenuState state;
	SceneTitle* scene;

	SDL_Texture* guiTex;
	SDL_Texture* bg;
	SDL_Texture* creditsTexture;
	
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
	GuiButton* btnCreditsRoles;
	GuiCheckBox* checkFullscreen;
	GuiCheckBox* checkVSync;
	GuiSlider* slideMusicVolume;
	GuiSlider* slideFXVolume;

	//Easings
	Easing* easingUp;
	Easing* easingDown;

	Easing* easing;
	Easing* easing2;
	Easing* easing3;

	float titlePosition;

	bool playMusicOptions;
};