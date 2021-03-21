#pragma once

class GuiControl;

class Menu
{
public:
	Menu() {}

	virtual ~Menu() {}

	virtual bool Load() { return true; }

	virtual bool Update(float dt) { return true; }

	virtual void Draw(bool showColliders) {}

	virtual bool UnLoad() { return true; }

	virtual bool OnGuiMouseClickEvent(GuiControl* control) { return true; }
};