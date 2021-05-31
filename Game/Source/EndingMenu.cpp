#include "App.h"
#include "Input.h"
#include "Textures.h"

#include "EndingMenu.h"

EndingMenu::EndingMenu(Font* font, Scene* scen)
{
	// Buttons
	btnNewGame = new GuiButton(1, { 936, 463, 260, 59 }, "New Game", this, font);
	btnMainMenu = new GuiButton(2, { 936, 550, 260, 59 }, "Main Menu", this, font);

	scene = scen;

	buttons.push_back(btnNewGame);
	buttons.push_back(btnMainMenu);
	currentControl = btnNewGame;

	bg = nullptr;
}

EndingMenu::~EndingMenu()
{
}

bool EndingMenu::Load(Font* font)
{
	bg = app->tex->Load("Textures/UI/ending_menu.png");
	btnNewGame->texture = bg;
	btnNewGame->section = {0, 279, 260, 59};
	btnNewGame->alineation = 1;

	btnMainMenu->texture = bg;
	btnMainMenu->section = { 0, 279, 260, 59 };
	btnMainMenu->alineation = 1;

	return true;
}

bool EndingMenu::Update(float dt)
{
	UpdatingButtons(app->input);

	int id = -1;
	if (lastUserInput == 0 && currentControl != nullptr)
	{
		id = currentControl->id;
	}

	btnNewGame->Update(app->input, dt, id);
	btnMainMenu->Update(app->input, dt, id);

	return true;
}

void EndingMenu::Draw(Font* font, bool showColliders)
{
	SDL_Rect rect = { 0, 0, 372, 279 };
	app->render->DrawTexture(bg, 880, 400, &rect);
	btnNewGame->Draw(app->render, showColliders, 36);
	btnMainMenu->Draw(app->render, showColliders, 36);
}

bool EndingMenu::UnLoad()
{
	RELEASE(btnNewGame);
	RELEASE(btnMainMenu);

	buttons.clear();

	return true;
}

bool EndingMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) // New Game
		{
			scene->TransitionToScene(SceneType::GAMEPLAY, TransitionType::ALTERNATING_BARS);
		}
		else if (control->id == 2) // Return to Main Menu
		{
			scene->TransitionToScene(SceneType::TITLE, TransitionType::HALF_WIDHT_RECTANGLES);
		}

		break;
	}

	return true;
}

void EndingMenu::UpdatingButtons(Input* input)
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
			currentControl = (*buttons.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
		{
			eastl::list<GuiButton*>::iterator it = buttons.begin();
			for (int i = 0; i < buttons.size(); ++i, ++it)
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
			currentControl = (*buttons.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
		{
			eastl::list<GuiButton*>::iterator it = buttons.begin();
			for (int i = 0; i < buttons.size(); ++i, ++it)
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
