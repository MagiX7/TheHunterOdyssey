#pragma once

#include "Scene.h"

class GuiManager;
class Font;

class SceneMenu : public Scene
{
public:
	SceneMenu();

	virtual ~SceneMenu() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	SDL_Texture* bg;

	GuiManager* guiManager;
};