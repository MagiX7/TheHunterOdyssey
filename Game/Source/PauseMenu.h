#pragma once

#include "Menu.h"
#include "GuiButton.h"

enum class PauseState
{
	DEFAULT = 0,
	OPTIONS
};

class PauseMenu : public Menu
{
public:
	PauseMenu(SceneGameplay* s);

	virtual ~PauseMenu();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:
	PauseState state;
	SceneGameplay* scene;

	GuiButton* btnResume;
	GuiButton* btnReturnTitle;
	GuiButton* btnOptions;
	GuiButton* btnExit;
};