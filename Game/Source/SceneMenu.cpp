#include "App.h"
#include "Textures.h"

#include "SceneMenu.h"
#include "GuiManager.h"
#include "Font.h"

#include "GuiButton.h"

SceneMenu::SceneMenu()
{
	state = MenuState::NORMAL;
	showColliders = true;
}

bool SceneMenu::Load()
{
	isExitPressed = false;

	btnNewGame = new GuiButton(1, { 466, 156, 360, 65 }, "Play", this);
	btnContinue = new GuiButton(2, { 466, 242, 360, 65 }, "Continue", this);
	btnOptions = new GuiButton(3, { 466, 328, 360, 65 }, "Options", this);
	btnCredits = new GuiButton(4, { 466, 413, 360, 65 }, "Credits", this);
	btnExit = new GuiButton(5, { 466, 504, 360, 65 }, "Exit", this);
	btnExitYes = new GuiButton(6, { 383, 332, 216, 51 }, "Yes", this);
	btnExitNo = new GuiButton(7, { 679, 332, 216, 51 }, "No", this);
		

	//guiManager = new GuiManager();
	//guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, { 466, 156, 360, 65 }, "Play", this);
	//guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, { 466, 242, 360, 65 }, "Continue", this);
	//guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, { 466, 328, 360, 65 }, "Options", this);
	//guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, { 466, 413, 360, 65 }, "Credits", this);
	//guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, { 466, 504, 360, 65 }, "Exit", this);
	//guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, { 383, 332, 216, 51 }, "Yes", this);
	//guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, { 679, 332, 216, 51 }, "No", this);

	return true;
}

bool SceneMenu::Update(float dt)
{
	switch (state)
	{
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

	}
	break;
	case MenuState::CREDITS:
	{

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

void SceneMenu::Draw()
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

	}
	break;
	case MenuState::CREDITS:
	{

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

bool SceneMenu::UnLoad()
{
	//guiManager->CleanUp();
	//RELEASE(guiManager);

	RELEASE(btnNewGame);
	RELEASE(btnContinue);
	RELEASE(btnOptions);
	RELEASE(btnCredits);
	RELEASE(btnExit);
	RELEASE(btnExitYes);
	RELEASE(btnExitNo);

	return true;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) TransitionToScene(SceneType::GAMEPLAY); // New Game
		if (control->id == 2) TransitionToScene(SceneType::GAMEPLAY); // Continue
		if (control->id == 3) state = MenuState::OPTIONS; // Options
		if (control->id == 4) state = MenuState::CREDITS; // Credits
		if (control->id == 5) state = MenuState::EXIT;
		if (control->id == 6) state = MenuState::GLOBAL_EXIT;
		if (control->id == 7) state = MenuState::NORMAL;
	}

	return true;
}