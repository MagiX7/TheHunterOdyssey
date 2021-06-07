#include "App.h"
#include "Render.h"
#include "SceneGameplay.h"
#include "QuestManager.h"
#include "QuestMenu.h"

#include "Easings.h"

#include "GuiButton.h"

QuestMenu::QuestMenu(SceneGameplay* s, QuestManager* qManager, Font* font)
{
	questManager = qManager;
	scene = s;

	guiTex = app->tex->Load("Textures/UI/gui_quests_menu.png");

	btnBack = new GuiButton(1, { 944, 574, 130, 64 }, "Back", this, font);
	btnBack->alineation = 1;
	btnBack->texture = guiTex;
	btnBack->section = { 1000,0,130,64 };

	easing = new Easing(true, 0, 0, 570, 120);
	easing2 = new Easing(false, 0, 570, -1070, 120);
	pos = 0;

	currentControl = nullptr;
	lastControl = nullptr;

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

	if (easing->easingsActivated)
	{
		btnBack->bounds.y = easing->elasticEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		pos = easing->backEaseOut(easing->currentIteration, easing->initialPos, 140, easing->totalIterations);

		if (easing->currentIteration < easing->totalIterations)
		{
			easing->currentIteration++;
		}
		else
		{
			easing->easingsActivated = false;
			easing->currentIteration = 0;
		}
	}

	if (easing2->easingsActivated)
	{
		btnBack->bounds.y = easing2->backEaseIn(easing2->currentIteration, easing2->initialPos, easing2->deltaPos, easing2->totalIterations);
		pos = easing2->backEaseIn(easing2->currentIteration, 140, -1140, easing2->totalIterations);

		if (easing2->currentIteration < easing2->totalIterations)
		{
			easing2->currentIteration++;
		}
		else
		{
			easing2->easingsActivated = false;
			easing2->currentIteration = 0;
			easing->easingsActivated = true;
			scene->ChangeState(GameplayMenuState::NONE);
		}
	}

	btnBack->Update(app->input, dt, id);

	return true;
}

void QuestMenu::Draw(Font* font, bool showColliders)
{
	// Black rectangle for the background
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120, true, false);

	SDL_Rect section = { 0,0,1000,699 };
	app->render->DrawTexture(guiTex, pos, 10, &section, false);
	
	app->render->DrawText(font, "Active Quests", pos + 120, 90, 45, 5, { 0, 255, 255, 255 });
	app->render->DrawText(font, "Finished Quests", pos + 570, 90, 45, 5, { 0, 255, 255, 255 });
	btnBack->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
	
	eastl::list<Quest*>::iterator it = questManager->GetActiveList().begin();
	eastl::list<Quest*>::iterator itEnd = questManager->GetActiveList().end();
	for (int i = 0; it != itEnd; ++it, i += 50)
	{
		app->render->DrawText(font, (*it)->name.c_str(), pos + 80, 190 + i, 40, 3, { 255, 255, 255, 255 });
	}

	it = questManager->GetFinishedList().begin();
	itEnd = questManager->GetFinishedList().end();
	for (int i = 0; it != itEnd; ++it, i += 50)
	{
		app->render->DrawText(font, (*it)->name.c_str(), pos + 540, 190 + i, 40, 3, { 255, 255, 255, 255 });
	}
}

bool QuestMenu::UnLoad()
{
	RELEASE(btnBack);
	RELEASE(easing);
	RELEASE(easing2);

	app->tex->UnLoad(guiTex);

	return true;
}

bool QuestMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1)
		{
			easing2->easingsActivated = true;
		}
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