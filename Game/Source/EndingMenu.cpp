#include "App.h"
#include "Input.h"

#include "EndingMenu.h"

EndingMenu::EndingMenu(Font* font, Scene* scen)
{
	// Buttons
	btnNewGame = new GuiButton(1, { 1000, 516, 250, 50 }, "New Game", this, font);
	btnMainMenu = new GuiButton(2, { 1000, 576, 250, 50 }, "Main Menu", this, font);

	scene = scen;

	buttons.push_back(btnNewGame);
	buttons.push_back(btnMainMenu);
}

EndingMenu::~EndingMenu()
{
}

bool EndingMenu::Load(Font* font)
{
	return true;
}

bool EndingMenu::Update(float dt)
{
	btnNewGame->Update(app->input, dt, -1);
	btnMainMenu->Update(app->input, dt, -1);

	return true;
}

void EndingMenu::Draw(Font* font, bool showColliders)
{
	btnNewGame->Draw(app->render, showColliders, 40);
	btnMainMenu->Draw(app->render, showColliders, 40);
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
}
