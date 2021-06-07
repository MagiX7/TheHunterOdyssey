#pragma once

#include "Menu.h"
#include "GuiControl.h"

class QuestManager;
class Easing;

class QuestMenu : public Menu
{
public:
	QuestMenu(SceneGameplay* s, QuestManager* qManager, Font* font);

	virtual ~QuestMenu();

	bool Load();

	bool Update(float dt) override;

	void Draw(Font* font, bool showColliders) override;

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control);

	void UpdatingButtons(Input* input);

private:
	SceneGameplay* scene;
	QuestManager* questManager;

	float pos;
	Easing* easing;
	Easing* easing2;
	GuiButton* btnBack;
	SDL_Texture* guiTex;
};