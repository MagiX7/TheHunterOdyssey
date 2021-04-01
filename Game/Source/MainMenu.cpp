#include "App.h"
#include "MainMenu.h"

#include "SceneTitle.h"
#include "Textures.h"

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
	guiTex = app->tex->Load("Assets/Textures/UI/gui_textures.png");
	bg = app->tex->Load("Assets/Textures/UI/menus_textures.png");

	btnNewGame = new GuiButton(1, { 380, 36, 520, 117 }, "New Game", this);
	btnNewGame->section = { 0,0,520,117 };
	btnNewGame->texture = guiTex;
	btnContinue = new GuiButton(2, { 380, 174, 520, 117 }, "Continue", this);
	btnContinue->section = { 0,0,520,117 };
	btnContinue->texture = guiTex;
	btnOptions = new GuiButton(3, { 380, 305, 520, 117 }, "Options", this);
	btnOptions->section = { 0,0,520,117 };
	btnOptions->texture = guiTex;
	btnCredits = new GuiButton(4, { 380, 440, 520, 117 }, "Credits", this);
	btnCredits->section = { 0,0,520,117 };
	btnCredits->texture = guiTex;
	btnExit = new GuiButton(5, { 380, 574, 520, 117 }, "Exit", this);
	btnExit->section = { 0,0,520,117 };
	btnExit->texture = guiTex;
	btnExitYes = new GuiButton(6, { 383, 332, 216, 51 }, "Yes", this);
	btnExitYes->section = { 0,135,216,51 };
	btnExitYes->texture = guiTex;
	btnExitNo = new GuiButton(7, { 679, 332, 216, 51 }, "No", this);
	btnExitNo->section = { 0,135,216,51 };
	btnExitNo->texture = guiTex;
	btnOptionsBack = new GuiButton(8, { 970, 615, 216, 51 }, "Back", this);
	btnOptionsBack->section = { 0,199,216,51 };
	btnOptionsBack->texture = guiTex;
	btnCreditsBack = new GuiButton(9, { 970, 615, 216, 51 }, "Back", this);
	btnCreditsBack->section = { 0,199,216,51 };
	btnCreditsBack->texture = guiTex;
	checkfullscreen = new GuiCheckBox(10, { 103,366,32,32 }, "Fullscreen", this);
	checkfullscreen->section = { 528,5,32,32 };
	checkfullscreen->texture = guiTex;
	checkVSync = new GuiCheckBox(10, { 103,462,32,32 }, "VSync", this);
	checkVSync->section = { 528,5,32,32 };
	checkVSync->texture = guiTex;

	font = new Font("Assets/Font/font3.xml", app->tex);

	return true;
}

bool MainMenu::Update(float dt)
{
	bool ret = true;

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
		checkfullscreen->Update(app->input, dt);
		checkVSync->Update(app->input, dt);
	}
	break;
	case MenuState::CREDITS:
	{
		btnCreditsBack->Update(app->input, dt);
	}
	break;
	case MenuState::EXIT:
	{
		ret = btnExitYes->Update(app->input, dt);
		btnExitNo->Update(app->input, dt);
	}
	break;
	}

	return ret;
}

void MainMenu::Draw(bool showColliders)
{
	SDL_Rect section;
	switch (state)
	{
	case MenuState::NORMAL:
	{
		section = { 729,0,271,106 };
		btnNewGame->Draw(app->render, showColliders);
		btnContinue->Draw(app->render, showColliders);
		btnOptions->Draw(app->render, showColliders);
		btnCredits->Draw(app->render, showColliders);
		btnExit->Draw(app->render, showColliders);
		app->render->DrawTexture(bg, 734, 720, &section);
		app->render->DrawText(font, btnNewGame->text.GetString(), 521, 65, 64, 5, { 255,255,255,255 });
		app->render->DrawText(font, btnContinue->text.GetString(), 530, 202, 64, 5, { 255,255,255,255 });
		app->render->DrawText(font, btnOptions->text.GetString(), 548, 332, 64, 5, { 255,255,255,255 });
		app->render->DrawText(font, btnCredits->text.GetString(), 550, 467, 64, 5, { 255,255,255,255 });
		app->render->DrawText(font, btnExit->text.GetString(), 596, 603, 64, 5, { 255,255,255,255 });
	}
	break;
	case MenuState::OPTIONS:
	{
		section = { 0,0,1280,720 };
		app->render->DrawTexture(bg, 0, 0, &section);
		btnOptionsBack->Draw(app->render, showColliders);
		checkfullscreen->Draw(app->render, showColliders);
		checkVSync->Draw(app->render, showColliders);
		app->render->DrawText(font, btnOptionsBack->text.GetString(), 1045, 625, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnOptions->text.GetString(), 535, 55, 64, 5, { 0,0,0,255 });
		app->render->DrawText(font, "Music volume", 103, 124, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, "FX volume", 103, 224, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, checkfullscreen->text.GetString(), 103, 324, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, checkVSync->text.GetString(), 103, 424, 24, 5, { 0,0,0,255 });
	}
	break;
	case MenuState::CREDITS:
	{
		btnCreditsBack->Draw(app->render, showColliders);
		app->render->DrawText(font, btnCreditsBack->text.GetString(), 1045, 625, 36, 2, { 0,0,0,255 });
	}
	break;
	case MenuState::EXIT:
	{
		section = { 0,720,734,488 };
		app->render->DrawTexture(bg, 272, 79, &section);
		btnExitYes->Draw(app->render, showColliders);
		btnExitNo->Draw(app->render, showColliders);
		app->render->DrawText(font, "Are you sure?", 501, 217, 48, 5, { 255,255,255,255 });
		app->render->DrawText(font, btnExitYes->text.GetString(), 465, 345, 36, 5, { 255,255,255,255 });
		app->render->DrawText(font, btnExitNo->text.GetString(), 767, 345, 36, 5, { 255,255,255,255 });
	}
	break;
	}
}

bool MainMenu::UnLoad()
{
	RELEASE(btnNewGame);
	RELEASE(btnContinue);
	RELEASE(btnOptions);
	RELEASE(btnCredits);
	RELEASE(btnExit);
	RELEASE(btnOptionsBack);
	RELEASE(btnCreditsBack);
	RELEASE(btnExitYes);
	RELEASE(btnExitNo);
	RELEASE(checkfullscreen);
	RELEASE(checkVSync);

	RELEASE(font);

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
		else if (control->id == 6) return false; // Yes exit
		else if (control->id == 7) state = MenuState::NORMAL; // Go back to the menu
		else if (control->id == 8) state = MenuState::NORMAL; // Go back to the menu
		else if (control->id == 9) state = MenuState::NORMAL; // Go back to the menu
		else if (control->id == 10); // Fullscreen
		else if (control->id == 11); // VSync
	}

	return true;
}