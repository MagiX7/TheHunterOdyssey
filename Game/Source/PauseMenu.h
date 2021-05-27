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
	RETURN_TITLE,
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

	void UpdatingButtons(Input* input) override;

private:
	PauseState state;
	SceneGameplay* scene;

	SDL_Texture* guiTex;

	GuiButton* btnResume;
	GuiButton* btnSave;
	GuiButton* btnLoad;
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

	bool playMusicOptions;
};