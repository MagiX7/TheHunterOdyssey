#pragma once

#include "EASTL/list.h"

class GuiControl;
class GuiButton;
class Font;

class Menu
{
public:
	Menu() {}

	virtual ~Menu() {}

	virtual bool Load(Font* font) { return true; }

	virtual bool Update(float dt) { return true; }

	virtual void Draw(Font* font, bool showColliders) {}

	virtual bool UnLoad() { return true; }

	virtual bool OnGuiMouseClickEvent(GuiControl* control) { return true; }

public:
	// For input handling
	eastl::list<GuiButton*> buttons;
	GuiButton* currentButton;
	GuiButton* lastButton;

};