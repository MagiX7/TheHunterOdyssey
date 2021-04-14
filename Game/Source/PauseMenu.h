#pragma once

#include "Menu.h"
#include "Font.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

class SceneGameplay;

enum class PauseState
{
	DEFAULT = 0,
	OPTIONS,
	SAVE,
	RETURNTITLE,
	EXIT
};

class PauseMenu : public Menu
{
public:
	PauseMenu(SceneGameplay* s);

	virtual ~PauseMenu();

	bool Load(Font* font) override;

	bool Update(float dt) override;

	void Draw(Font* font, bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

	void HandleInput();

private:
	PauseState state;
	SceneGameplay* scene;

	SDL_Texture* guiTex;

	GuiButton* btnResume;
	GuiButton* btnLoadSave;
	GuiButton* btnSave;
	GuiButton* btnLoad;
	GuiButton* btnBack;
	GuiButton* btnOptions;
	GuiButton* btnReturnTitle;
	GuiButton* btnReturnTitleYes;
	GuiButton* btnReturnTitleNo;
	GuiButton* btnExit;
	GuiButton* btnExitYes;
	GuiButton* btnExitNo;
	GuiSlider* slideMusicVolume;
	GuiSlider* slideFXVolume;
	GuiCheckBox* checkFullscreen;
	GuiCheckBox* checkVSync;
	GuiButton* btnOptionsBack;
};