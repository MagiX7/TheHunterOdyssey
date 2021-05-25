#pragma once

#include "Menu.h"
#include "Scene.h"

#include "GuiButton.h"

#include "EASTL/list.h"

class Input;

class EndingMenu : public Menu
{
public:
	EndingMenu(Font* font, Scene* scen);
	virtual ~EndingMenu();

	bool Load(Font* font) override;
	bool Update(float dt) override;
	void Draw(Font* font, bool showColliders) override;
	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;
	void UpdatingButtons(Input* input) override;
	
private:
	// Buttons
	GuiButton* btnMainMenu;
	GuiButton* btnNewGame;

	eastl::list<GuiButton*> buttons;

	Scene* scene;
};