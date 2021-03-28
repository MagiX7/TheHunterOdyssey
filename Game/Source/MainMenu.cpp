#include "App.h"
#include "MainMenu.h"

#include "SceneTitle.h"
#include "GuiButton.h"

MainMenu::MainMenu(SceneTitle* s)
{
	state = MenuState::NONE;
	scene = s;
}

MainMenu::~MainMenu()
{
}

bool MainMenu::Load()
{
	btnNewGame = new GuiButton(1, { 466, 156, 360, 65 }, "Play", this);
	btnContinue = new GuiButton(2, { 466, 242, 360, 65 }, "Continue", this);
	btnOptions = new GuiButton(3, { 466, 328, 360, 65 }, "Options", this);
	btnCredits = new GuiButton(4, { 466, 413, 360, 65 }, "Credits", this);
	btnExit = new GuiButton(5, { 466, 504, 360, 65 }, "Exit", this);
	btnExitYes = new GuiButton(6, { 383, 332, 216, 51 }, "Yes", this);
	btnExitNo = new GuiButton(7, { 679, 332, 216, 51 }, "No", this);
	btnOptionsBack = new GuiButton(8, { 1048, 653, 216, 51 }, "Back", this);
	btnCreditsBack = new GuiButton(9, { 1048, 653, 216, 51 }, "Back", this);

	return true;
}

bool MainMenu::Update(float dt)
{
	switch (state)
	{
	case MenuState::NONE:
	{
		state = MenuState::NORMAL;
	}
	break;
	case MenuState::NORMAL:
	{
		btnNewGame->Update(app->input, dt);
		btnContinue->Update(app->input, dt);
		btnOptions->Update(app->input, dt);
		btnCredits->Update(app->input, dt);
		btnExit->Update(app->input, dt);
	}
	break;
	case MenuState::OPTIONS:
	{
		btnOptionsBack->Update(app->input, dt);
	}
	break;
	case MenuState::CREDITS:
	{
		btnCreditsBack->Update(app->input, dt);
	}
	break;
	case MenuState::EXIT:
	{
		btnExitYes->Update(app->input, dt);
		btnExitNo->Update(app->input, dt);
	}
	break;
	case MenuState::GLOBAL_EXIT:
	{
		return false;
	}
	break;
	}

	return true;
}

void MainMenu::Draw(bool showColliders)
{
	switch (state)
	{
	case MenuState::NORMAL:
	{
		btnNewGame->Draw(app->render, showColliders);
		btnContinue->Draw(app->render, showColliders);
		btnOptions->Draw(app->render, showColliders);
		btnCredits->Draw(app->render, showColliders);
		btnExit->Draw(app->render, showColliders);
	}
	break;
	case MenuState::OPTIONS:
	{
		btnOptionsBack->Draw(app->render, showColliders);
	}
	break;
	case MenuState::CREDITS:
	{
		btnCreditsBack->Draw(app->render, showColliders);
	}
	break;
	case MenuState::EXIT:
	{
		btnExitYes->Draw(app->render, showColliders);
		btnExitNo->Draw(app->render, showColliders);
	}
	break;
	}
}

bool MainMenu::UnLoad()
{
	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) scene->TransitionToScene(SceneType::GAMEPLAY); // New Game
		else if (control->id == 2) scene->TransitionToScene(SceneType::GAMEPLAY); // Continue
		else if (control->id == 3) state = MenuState::OPTIONS; // Options
		else if (control->id == 4) state = MenuState::CREDITS; // Credits
		else if (control->id == 5) state = MenuState::EXIT; // Exit button pressed
		else if (control->id == 6) state = MenuState::GLOBAL_EXIT; // Yes exit
		else if (control->id == 7) state = MenuState::NORMAL; // Go back to the menu
		else if (control->id == 8) state = MenuState::NORMAL; // Go back to the menu
		else if (control->id == 9) state = MenuState::NORMAL; // Go back to the menu
	}

	return false;
}