#include "App.h"
#include "Render.h"
#include "Window.h"
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

bool MainMenu::Load(Font* font)
{
	guiTex = app->tex->Load("Assets/Textures/UI/gui_main_menu.png");
	bg = app->tex->Load("Assets/Textures/UI/main_menu_textures.png");

	btnNewGame = new GuiButton(1, { 380, 36, 520, 117 }, "New Game", this, font);
	btnNewGame->section = { 0,0,520,117 };
	btnNewGame->texture = guiTex;
	btnNewGame->alineation = 1;

	btnContinue = new GuiButton(2, { 380, 174, 520, 117 }, "Continue", this, font);
	btnContinue->section = { 0,0,520,117 };
	btnContinue->texture = guiTex;
	btnContinue->alineation = 1;

	btnOptions = new GuiButton(3, { 380, 305, 520, 117 }, "Options", this, font);
	btnOptions->section = { 0,0,520,117 };
	btnOptions->texture = guiTex;
	btnOptions->alineation = 1;

	btnCredits = new GuiButton(4, { 380, 440, 520, 117 }, "Credits", this, font);
	btnCredits->section = { 0,0,520,117 };
	btnCredits->texture = guiTex;
	btnCredits->alineation = 1;

	btnExit = new GuiButton(5, { 380, 574, 520, 117 }, "Exit", this, font);
	btnExit->section = { 0,0,520,117 };
	btnExit->texture = guiTex;
	btnExit->alineation = 1;

	btnExitYes = new GuiButton(6, { 383, 332, 216, 51 }, "Yes", this, font);
	btnExitYes->section = { 0,135,216,51 };
	btnExitYes->texture = guiTex;
	btnExitYes->alineation = 1;

	btnExitNo = new GuiButton(7, { 679, 332, 216, 51 }, "No", this, font);
	btnExitNo->section = { 0,135,216,51 };
	btnExitNo->texture = guiTex;
	btnExitNo->alineation = 1;

	btnOptionsBack = new GuiButton(8, { 970, 615, 216, 51 }, "Back", this, font);
	btnOptionsBack->section = { 0,199,216,51 };
	btnOptionsBack->texture = guiTex;
	btnOptionsBack->alineation = 1;

	btnCreditsBack = new GuiButton(9, { 970, 615, 216, 51 }, "Back", this, font);
	btnCreditsBack->section = { 0,199,216,51 };
	btnCreditsBack->texture = guiTex;
	btnCreditsBack->alineation = 1;

	buttons.push_back(btnNewGame);
	buttons.push_back(btnContinue);
	buttons.push_back(btnOptions);
	buttons.push_back(btnCredits);
	buttons.push_back(btnExit);
	currentButton = (*buttons.begin());
	lastButton = nullptr;


	// Check-boxes
	checkFullscreen = new GuiCheckBox(10, { 94,318,185,32 }, "Fullscreen", this);
	checkFullscreen->section = { 528,5,32,32 };
	checkFullscreen->texture = guiTex;

	checkVSync = new GuiCheckBox(11, { 94,419,120,32 }, "VSync", this);
	checkVSync->section = { 528,5,32,32 };
	checkVSync->texture = guiTex;

	return true;
}

bool MainMenu::Update(float dt)
{
	bool ret = true;

	HandleInput();

	switch (state)
	{
	case MenuState::NONE:
	{
		state = MenuState::NORMAL;
	}
	break;
	case MenuState::NORMAL:
	{
		btnNewGame->Update(app->input, dt, currentButton->id);
		btnContinue->Update(app->input, dt, currentButton->id);
		btnOptions->Update(app->input, dt, currentButton->id);
		btnCredits->Update(app->input, dt, currentButton->id);
		btnExit->Update(app->input, dt, currentButton->id);
	}
	break;
	case MenuState::OPTIONS:
	{
		btnOptionsBack->Update(app->input, dt, currentButton->id);
		checkFullscreen->Update(app->input, dt);
		checkVSync->Update(app->input, dt);
	}
	break;
	case MenuState::CREDITS:
	{
		btnCreditsBack->Update(app->input, dt, currentButton->id);
	}
	break;
	case MenuState::EXIT:
	{
		ret = btnExitYes->Update(app->input, dt, currentButton->id);
		btnExitNo->Update(app->input, dt, currentButton->id);
	}
	break;
	}

	return ret;
}

void MainMenu::Draw(Font* font, bool showColliders)
{
	SDL_Rect section;
	switch (state)
	{
	case MenuState::NORMAL:
	{
		section = { 729,0,271,106 };
		btnNewGame->Draw(app->render, showColliders, 64, { 255,255,255,255 });
		btnContinue->Draw(app->render, showColliders, 64, { 255,255,255,255 });
		btnOptions->Draw(app->render, showColliders, 64, { 255,255,255,255 });
		btnCredits->Draw(app->render, showColliders, 64, { 255,255,255,255 });
		btnExit->Draw(app->render, showColliders, 64, { 255,255,255,255 });
		app->render->DrawTexture(bg, 734, 720, &section);
		//app->render->DrawText(font, btnNewGame->text.GetString(), 521, 65, 64, 5, { 255,255,255,255 });
		//app->render->DrawText(font, btnContinue->text.GetString(), 530, 202, 64, 5, { 255,255,255,255 });
		//app->render->DrawText(font, btnOptions->text.GetString(), 548, 332, 64, 5, { 255,255,255,255 });
		//app->render->DrawText(font, btnCredits->text.GetString(), 550, 467, 64, 5, { 255,255,255,255 });
		//app->render->DrawText(font, btnExit->text.GetString(), 596, 603, 64, 5, { 255,255,255,255 });
	}
	break;
	case MenuState::OPTIONS:
	{
		section = { 0,0,1280,720 };
		app->render->DrawTexture(bg, 0, 0, &section);
		btnOptionsBack->Draw(app->render, showColliders, 36, {0, 0, 0, 255});
		checkFullscreen->Draw(app->render, showColliders);
		checkVSync->Draw(app->render, showColliders);
		//app->render->DrawText(font, btnOptionsBack->text.GetString(), 1045, 625, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnOptions->text.GetString(), 535, 55, 64, 5, { 0,0,0,255 });
		app->render->DrawText(font, "Music volume", 103, 124, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, "FX volume", 103, 224, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, checkFullscreen->text.GetString(), 103, 324, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, checkVSync->text.GetString(), 103, 424, 24, 5, { 0,0,0,255 });
	}
	break;
	case MenuState::CREDITS:
	{
		section = { 0,0,1280,720 };
		app->render->DrawTexture(bg, 0, 0, &section);
		btnCreditsBack->Draw(app->render, showColliders);
		//app->render->DrawText(font, btnCreditsBack->text.GetString(), 1045, 625, 36, 2, { 0,0,0,255 });
	}
	break;
	case MenuState::EXIT:
	{
		section = { 0,720,734,488 };
		app->render->DrawTexture(bg, 272, 79, &section);
		btnExitYes->Draw(app->render, showColliders, 36, { 255,255,255,255 });
		btnExitNo->Draw(app->render, showColliders, 36, { 255,255,255,255 });
		app->render->DrawText(font, "Are you sure?", 501, 217, 48, 5, { 255,255,255,255 });
		//app->render->DrawText(font, btnExitYes->text.GetString(), 465, 345, 36, 5, { 255,255,255,255 });
		//app->render->DrawText(font, btnExitNo->text.GetString(), 767, 345, 36, 5, { 255,255,255,255 });
	}
	break;
	}
}

bool MainMenu::UnLoad()
{
	app->tex->UnLoad(guiTex);
	app->tex->UnLoad(bg);
	font->UnLoad(app->tex);

	RELEASE(btnNewGame);
	RELEASE(btnContinue);
	RELEASE(btnOptions);
	RELEASE(btnCredits);
	RELEASE(btnExit);
	RELEASE(btnOptionsBack);
	RELEASE(btnCreditsBack);
	RELEASE(btnExitYes);
	RELEASE(btnExitNo);
	RELEASE(checkFullscreen);
	RELEASE(checkVSync);

	RELEASE(font);

	buttons.clear();

	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) scene->TransitionToScene(SceneType::GAMEPLAY); // New Game
		else if (control->id == 2) scene->TransitionToScene(SceneType::GAMEPLAY); // Continue
		else if (control->id == 3) // Options
		{
			state = MenuState::OPTIONS;
			buttons.clear();
			buttons.push_back(btnOptionsBack);
			
			lastButton = currentButton;
			currentButton = (*buttons.begin());
		}
		else if (control->id == 4) // Credits
		{
			state = MenuState::CREDITS;
			buttons.clear();
			buttons.push_back(btnCreditsBack);

			lastButton = currentButton;
			currentButton = (*buttons.begin());
		}
		else if (control->id == 5) // Exit button pressed
		{
			state = MenuState::EXIT;
			buttons.clear();
			buttons.push_back(btnExitYes);
			buttons.push_back(btnExitNo);

			lastButton = currentButton;
			currentButton = (*buttons.end().prev());
		}
		else if (control->id == 6) return false; // Yes exit
		else if (control->id == 7) // Go back to the menu
		{
			state = MenuState::NORMAL;
			buttons.clear();
			buttons.push_back(btnNewGame);
			buttons.push_back(btnContinue);
			buttons.push_back(btnOptions);
			buttons.push_back(btnCredits);
			buttons.push_back(btnExit);

			currentButton = lastButton;
		}
		else if (control->id == 8) // Go back to the menu
		{
			state = MenuState::NORMAL;
			buttons.clear();
			buttons.push_back(btnNewGame);
			buttons.push_back(btnContinue);
			buttons.push_back(btnOptions);
			buttons.push_back(btnCredits);
			buttons.push_back(btnExit);
			
			currentButton = lastButton;
		}
		else if (control->id == 9) // Go back to the menu
		{
			state = MenuState::NORMAL;
			buttons.clear();
			buttons.push_back(btnNewGame);
			buttons.push_back(btnContinue);
			buttons.push_back(btnOptions);
			buttons.push_back(btnCredits);
			buttons.push_back(btnExit);

			currentButton = lastButton;
		}
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 10) // Fullscreen
		{
			Window::Get()->fullscreenWindow = !Window::Get()->fullscreenWindow;
			Window::Get()->SetFullscreen();
		}
		else if (control->id == 11); // Vsync
	}
	default: break;
	}
	
	return true;
}

void MainMenu::HandleInput()
{
	int key1 = SDL_SCANCODE_DOWN;
	int key2 = SDL_SCANCODE_UP;
	
	if (state == MenuState::EXIT)
	{
		key1 = SDL_SCANCODE_RIGHT;
		key2 = SDL_SCANCODE_LEFT;
	}

	if (app->input->GetKey(key1) == KEY_DOWN)
	{
		eastl::list<GuiButton*>::iterator it = buttons.begin();
		for (int i = 0; i < buttons.size(); ++i, ++it)
		{
			if ((*it)->id == currentButton->id + 1)
			{
				currentButton = (*it);
				break;
			}
		}
	}
	else if (app->input->GetKey(key2) == KEY_DOWN)
	{
		eastl::list<GuiButton*>::iterator it = buttons.begin();
		for (int i = 0; i < buttons.size(); ++i, ++it)
		{
			if ((*it)->id == currentButton->id - 1)
			{
				currentButton = (*it);
				break;
			}
		}
	}
}