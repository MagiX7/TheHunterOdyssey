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

	virtual void UpdatingButtons(Input* input) {}

public:
	// For input handling
	eastl::list<GuiControl*> controls;
	GuiControl* currentControl;
	GuiControl* lastControl;

	// Stuff for keyobard and mouse handling at the same time
	int lastUserInput; // 1 == keyboard; 0 == mouse
	int xMouse;
	int yMouse;
};