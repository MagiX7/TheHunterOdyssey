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

	btnBack = new GuiButton(1, { 1040, 610, 100, 50 }, "Back", this, font);
	btnBack->alineation = 1;

	controls.push_back(btnBack);
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
	UpdatingButtons(app->input);

	int id = -1;
	if (lastUserInput == 0 && currentControl != nullptr)
	{
		id = currentControl->id;
	}

	btnBack->Update(app->input, dt, id);

	return true;
}

void QuestMenu::Draw(Font* font, bool showColliders)
{
	// Black rectangle for the background
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120, true, false);
	app->render->DrawRectangle({ 140, 60, 1000, 600 }, 0, 0, 0, 200, true, false);

	app->render->DrawCenterText(font, "Active Quests", { 140, 60, 500, 50 }, 34, 5, { 255, 255, 255, 255 });
	app->render->DrawCenterText(font, "Finished Quests", { 640, 60, 500, 50 }, 34, 5, { 255, 255, 255, 255 });
	btnBack->Draw(app->render, showColliders, 34, { 255, 255, 255, 255 });
	
	eastl::list<Quest*>::iterator it = questManager->GetActiveList().begin();
	eastl::list<Quest*>::iterator itEnd = questManager->GetActiveList().end();
	for (int i = 0; it != itEnd; ++it, i += 50)
	{
		app->render->DrawText(font, (*it)->name.c_str(), 200, 150 + i, 40, 3, { 255, 255, 255, 255 });
	}

	it = questManager->GetFinishedList().begin();
	itEnd = questManager->GetFinishedList().end();
	for (int i = 0; it != itEnd; ++it, i += 50)
	{
		app->render->DrawText(font, (*it)->name.c_str(), 700, 150 + i, 40, 3, { 255, 255, 255, 255 });
	}
}

bool QuestMenu::UnLoad()
{
	RELEASE(btnBack);

	return true;
}

bool QuestMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) scene->ChangeState(GameplayMenuState::NONE);
		break;
	}
	return false;
}

void QuestMenu::UpdatingButtons(Input* input)
{
	int prevX = xMouse;
	int prevY = yMouse;
	input->GetMousePosition(xMouse, yMouse);
	if (prevX != xMouse || prevY != yMouse)
	{
		lastUserInput = 1;
		SDL_ShowCursor(SDL_ENABLE);
	}
	else
	{
		lastUserInput = 0;
	}

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
	{
		if (currentControl == nullptr)
		{
			currentControl = (*controls.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
		{
			eastl::list<GuiControl*>::iterator it = controls.begin();
			for (int i = 0; i < controls.size(); ++i, ++it)
			{
				if ((*it)->id == currentControl->id + 1)
				{
					currentControl = (*it);
					break;
				}
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
	{
		if (currentControl == nullptr)
		{
			currentControl = (*controls.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
		{
			eastl::list<GuiControl*>::iterator it = controls.begin();
			for (int i = 0; i < controls.size(); ++i, ++it)
			{
				if ((*it)->id == currentControl->id - 1)
				{
					currentControl = (*it);
					break;
				}
			}
		}
	}
}