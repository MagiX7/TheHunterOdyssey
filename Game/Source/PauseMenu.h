#pragma once

#include "Menu.h"
#include "Font.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

enum class PauseState
{
	DEFAULT = 0,
	OPTIONS,
	SAVE
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
	GuiButton* btnExit;
};