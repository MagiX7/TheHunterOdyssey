#include "App.h"
#include "Render.h"
#include "SceneGameplay.h"
#include "QuestManager.h"
#include "QuestMenu.h"

#include "GuiButton.h"

QuestMenu::QuestMenu(SceneGameplay* s, QuestManager* qManager, Font* font)
{
	questManager = qManager;
	scene = s;

	btnActive = new GuiButton(1, { 140, 60, 500, 50 }, "Active Quests", this, font);
	btnActive->alineation = 1;
	btnFinished = new GuiButton(2, { 640, 60, 500, 50 }, "Finished Quests", this, font);
	btnFinished->alineation = 1;
	btnBack = new GuiButton(3, { 1040, 610, 100, 50 }, "Back", this, font);
	btnBack->alineation = 1;

	state = QuestState::ACTIVE;
}

QuestMenu::~QuestMenu()
{
}

bool QuestMenu::Load()
{
	return true;
}

bool QuestMenu::Update(float dt)
{
	btnActive->Update(app->input, dt, -1);
	btnFinished->Update(app->input, dt, -1);
	btnBack->Update(app->input, dt, -1);

	return true;
}

void QuestMenu::Draw(Font* font, bool showColliders)
{
	// Black rectangle for the background
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120, true, false);
	app->render->DrawRectangle({ 140, 60, 1000, 600 }, 0, 0, 0, 200, true, false);

	btnActive->Draw(app->render, showColliders, 34, { 255, 255, 255, 255 });
	btnFinished->Draw(app->render, showColliders, 34, { 255, 255, 255, 255 });
	btnBack->Draw(app->render, showColliders, 34, { 255, 255, 255, 255 });
	
	switch (state)
	{
	case QuestState::ACTIVE:
	{
		eastl::list<Quest*>::iterator it = questManager->GetActiveList().begin();
		eastl::list<Quest*>::iterator itEnd = questManager->GetActiveList().end();
		for (int i = 0; it != itEnd; ++it, i += 50)
		{
			app->render->DrawText(font, (*it)->name.c_str(), 200, 150, 40, 3, { 255, 255, 255, 255 });
		}
	}
	break;
	case QuestState::FINISHED:
	{
		eastl::list<Quest*>::iterator it = questManager->GetFinishedList().begin();
		eastl::list<Quest*>::iterator itEnd = questManager->GetFinishedList().end();
		for (int i = 0; it != itEnd; ++it, i += 50)
		{
			app->render->DrawText(font, (*it)->name.c_str(), 200, 150 + i, 40, 3, { 255, 255, 255, 255 });
		}
	}
	break;
	}
}

bool QuestMenu::UnLoad()
{
	RELEASE(btnActive);
	RELEASE(btnFinished);
	RELEASE(btnBack);

	return true;
}

bool QuestMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) state = QuestState::ACTIVE;
		else if (control->id == 2) state = QuestState::FINISHED;
		else if (control->id == 3) scene->ChangeState(GameplayMenuState::NONE);
		break;
	}
	return false;
}