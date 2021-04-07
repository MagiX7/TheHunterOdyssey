#pragma once

class GuiControl;
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
};