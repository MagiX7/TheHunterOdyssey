#pragma once

#include "Menu.h"
#include "GuiControl.h"

enum class QuestState
{
	ACTIVE = 0,
	FINISHED
};

class QuestManager;

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

private:
	SceneGameplay* scene;
	QuestManager* questManager;

	GuiButton* btnActive;
	GuiButton* btnFinished;
	GuiButton* btnBack;

	QuestState state;
};