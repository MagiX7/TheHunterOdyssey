#include "App.h"
#include "Render.h"
#include "Window.h"
#include "MainMenu.h"

#include "SceneTitle.h"
#include "Textures.h"
#include "Audio.h"
#include "Easings.h"

MainMenu::MainMenu(SceneTitle* s)
{
	state = MenuState::NONE;
	scene = s;

	//Easings Initialized
	easingUp = new Easing();
	easingDown = new Easing();

	easingUp->currentIteration = 0;
	easingUp->totalIterations = 60;
	easingUp->initialPos = 588;
	easingUp->deltaPos = -20;
	easingUp->easingsActivated = true;

	easingDown->currentIteration = 0;
	easingDown->totalIterations = 60;
	easingDown->initialPos = 568;
	easingDown->deltaPos = 20;
	easingDown->easingsActivated = false;

	titlePosition = 13;

	playMusicOptions = true;
}

MainMenu::~MainMenu()
{
}

bool MainMenu::Load(Font* font)
{
	guiTex = app->tex->Load("Textures/UI/gui_main_menu.png");
	bg = app->tex->Load("Textures/UI/main_menu_textures.png");
	creditsTexture = app->tex->Load("Textures/Scenes/credits.png");

	// Main menu =========================
	btnNewGame = new GuiButton(1, { 510, 325, 260, 59 }, "New Game", this, font);
	btnNewGame->section = { 0,0,260,59 };
	btnNewGame->texture = guiTex;
	btnNewGame->alineation = 1;

	btnContinue = new GuiButton(2, { 510, 393, 260, 59 }, "Continue", this, font);
	btnContinue->section = { 0,0,260,59 };
	btnContinue->texture = guiTex;
	btnContinue->alineation = 1;

	btnOptions = new GuiButton(3, { 510, 461, 260, 59 }, "Options", this, font);
	btnOptions->section = { 0,0,260,59 };
	btnOptions->texture = guiTex;
	btnOptions->alineation = 1;

	btnCredits = new GuiButton(4, { 510, 529, 260, 59 }, "Credits", this, font);
	btnCredits->section = { 0,0,260,59 };
	btnCredits->texture = guiTex;
	btnCredits->alineation = 1;

	btnExit = new GuiButton(5, { 510, 597, 260, 59 }, "Exit", this, font);
	btnExit->section = { 0,0,260,59 };
	btnExit->texture = guiTex;
	btnExit->alineation = 1;
	// Main menu =========================

	// Options menu =========================
	slideMusicVolume = new GuiSlider(6, { 103,162,339,46 }, "Music Volume", this, 0, 128, app->audio->GetMusicVolume());
	slideMusicVolume->section = { 223,138,339,46 };
	slideMusicVolume->texture = guiTex;

	slideFXVolume = new GuiSlider(7, { 103,258,339,46 }, "FX Volume", this, 0, 128, app->audio->GetFxVolume());
	slideFXVolume->section = { 223,138,339,46 };
	slideFXVolume->texture = guiTex;

	checkFullscreen = new GuiCheckBox(8, { 94,318,185,32 }, "Fullscreen", this, app->win->fullscreenWindow);
	checkFullscreen->section = { 528,5,32,32 };
	checkFullscreen->texture = guiTex;

	btnOptionsBack = new GuiButton(9, { 970, 615, 216, 51 }, "Back", this, font);
	btnOptionsBack->section = { 0,199,216,51 };
	btnOptionsBack->texture = guiTex;
	btnOptionsBack->alineation = 1;
	// Options menu =========================

	// Credits ==============================
	btnCreditsBack = new GuiButton(10, { 970, 615, 216, 51 }, "Back", this, font);
	btnCreditsBack->section = { 0,199,216,51 };
	btnCreditsBack->texture = guiTex;
	btnCreditsBack->alineation = 1;

	btnCreditsRoles = new GuiButton(11, { 0,0,0,0 }, "", this, font);
	// Credits ==============================

	// Exit =================================
	btnExitYes = new GuiButton(12, { 533, 312, 216, 51 }, "Yes", this, font);
	btnExitYes->section = { 0,135,216,51 };
	btnExitYes->texture = guiTex;
	btnExitYes->alineation = 1;

	btnExitNo = new GuiButton(13, { 533, 384, 216, 51 }, "No", this, font);
	btnExitNo->section = { 0,135,216,51 };
	btnExitNo->texture = guiTex;
	btnExitNo->alineation = 1;
	// Exit =================================

	controls.push_back(btnNewGame);
	controls.push_back(btnContinue);
	controls.push_back(btnOptions);
	controls.push_back(btnCredits);
	controls.push_back(btnExit);
	currentControl = nullptr;
	lastControl = nullptr;


	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

	if (result == NULL) btnContinue->state = GuiControlState::DISABLED;

	checkVSync = new GuiCheckBox(20, { 94,378,120,32 }, "VSync", this, true);
	checkVSync->section = { 528,5,32,32 };
	checkVSync->texture = guiTex;
	checkVSync->state = GuiControlState::DISABLED;

	lastUserInput = -1;

	return true;
}

bool MainMenu::Update(float dt)
{
	bool ret = true;

	UpdatingButtons(app->input);

	int id = -1;
	if (lastUserInput == 0 && currentControl != nullptr)
	{
		id = currentControl->id;
	}

	switch (state)
	{
	case MenuState::NONE:
	{
		state = MenuState::NORMAL;
	}
	break;
	
	case MenuState::NORMAL:
	{
		if (playMusicOptions == false)
		{
			app->audio->PlayMusic("Audio/Music/intro_theme.ogg");
			playMusicOptions = true;
		}

		btnNewGame->Update(app->input, dt, id);
		btnContinue->Update(app->input, dt, id);
		btnOptions->Update(app->input, dt, id);
		btnCredits->Update(app->input, dt, id);
		btnExit->Update(app->input, dt, id);
	}
	break;

	case MenuState::OPTIONS:
	{
		if (playMusicOptions == true)
		{
			app->audio->PlayMusic("Audio/Music/options_theme.ogg");
			playMusicOptions = false;
		}

		btnOptionsBack->Update(app->input, dt, id);
		checkFullscreen->Update(app->input, dt, id);
		checkVSync->Update(app->input, dt, id);
		slideMusicVolume->Update(app->input, dt, id);
		slideFXVolume->Update(app->input, dt, id);
	}
	break;

	case MenuState::CREDITS:
	{
		btnCreditsBack->Update(app->input, dt, id);
	}
	break;

	case MenuState::EXIT:
	{
		ret = btnExitYes->Update(app->input, dt, id);
		btnExitNo->Update(app->input, dt, id);
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
		SDL_Rect auxSection = { 734,720,372,454 };

		app->render->DrawTexture(bg, 454, 266, &auxSection);

		section = { 1106,720,633,248 };

		btnNewGame->Draw(app->render, showColliders, 36, { 255,255,255,255 });
		btnContinue->Draw(app->render, showColliders, 36, { 255,255,255,255 });
		btnOptions->Draw(app->render, showColliders, 36, { 255,255,255,255 });
		btnCredits->Draw(app->render, showColliders, 36, { 255,255,255,255 });
		btnExit->Draw(app->render, showColliders, 36, { 255,255,255,255 });

		app->render->DrawTexture(bg, 323, titlePosition, &section);
	}
	break;

	case MenuState::OPTIONS:
	{
		section = { 0,0,1280,720 };
		app->render->DrawTexture(bg, 0, 0, &section);
		btnOptionsBack->Draw(app->render, showColliders, 36, {0, 0, 0, 255});
		checkFullscreen->Draw(app->render, showColliders);
		checkVSync->Draw(app->render, showColliders);
		slideMusicVolume->Draw(app->render, showColliders);
		slideFXVolume->Draw(app->render, showColliders);
		//app->render->DrawText(font, btnOptionsBack->text.GetString(), 1045, 625, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnOptions->text.GetString(), 535, 55, 64, 5, { 0,0,0,255 });
		app->render->DrawText(font, "Music volume", 103, 124, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, "FX volume", 103, 224, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, checkFullscreen->text.GetString(), 103, 324, 24, 5, { 0,0,0,255 });
		app->render->DrawText(font, checkVSync->text.GetString(), 103, 384, 24, 5, { 0,0,0,255 });
	}
	break;

	case MenuState::CREDITS:
	{
		section = { 0,0,1280,720 };
		app->render->DrawTexture(bg, 0, 0, &section);

		app->render->DrawTexture(creditsTexture, 0, 0, NULL);

		btnCreditsBack->Draw(app->render, showColliders, 36, { 0,0,0,225 });
	}
	break;

	case MenuState::EXIT:
	{
		section = { 0,720,734,488 };
		app->render->DrawTexture(bg, 272, 79, &section);
		btnExitYes->Draw(app->render, showColliders, 36, { 255,255,255,255 });
		btnExitNo->Draw(app->render, showColliders, 36, { 255,255,255,255 });
		app->render->DrawText(font, "Are you sure?", 501, 217, 48, 5, { 255,255,255,255 });
	}
	break;

	}
}

bool MainMenu::UnLoad()
{
	app->tex->UnLoad(guiTex);
	app->tex->UnLoad(bg);
	app->tex->UnLoad(creditsTexture);

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
	RELEASE(slideMusicVolume);
	RELEASE(slideFXVolume);
	RELEASE(easingDown);
	RELEASE(easingUp);
	RELEASE(btnCreditsRoles);

	controls.clear();

	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) // New Game
			scene->TransitionToScene(SceneType::GAMEPLAY, TransitionType::ALTERNATING_BARS);// New Game
		else if (control->id == 2) // Continue
			scene->TransitionToScene(SceneType::GAMEPLAY, TransitionType::WIPE, true);
		else if (control->id == 3) // Options
		{
			state = MenuState::OPTIONS;
			controls.clear();
			controls.push_back(slideMusicVolume);
			controls.push_back(slideFXVolume);
			controls.push_back(checkFullscreen);
			controls.push_back(btnOptionsBack);
			
			lastControl = currentControl;
			currentControl = (*controls.begin());
		}
		else if (control->id == 4) // Credits
		{
			state = MenuState::CREDITS;
			controls.clear();
			controls.push_back(btnCreditsBack);

			lastControl = currentControl;
			currentControl = (*controls.begin());
		}
		else if (control->id == 5) // Exit button pressed
		{
			state = MenuState::EXIT;
			controls.clear();
			controls.push_back(btnExitYes);
			controls.push_back(btnExitNo);

			lastControl = currentControl;
			currentControl = (*controls.end().prev());
		}
		else if (control->id == 9 || control->id == 10 || control->id == 13) // Go back to the menu
		{
			state = MenuState::NORMAL;
			controls.clear();
			controls.push_back(btnNewGame);
			controls.push_back(btnContinue);
			controls.push_back(btnOptions);
			controls.push_back(btnCredits);
			controls.push_back(btnExit);

			currentControl = lastControl;
		}
		else if (control->id == 12) return false; // Yes exit
		break;
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 8) // Fullscreen
		{
			app->win->SetFullscreen();
		}
		else if (control->id == 11); // Vsync
		break;
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 6) // Music
			app->audio->SetMusicVolume(slideMusicVolume->GetValue());
		else if (control->id == 7) // Fx
			app->audio->SetFxVolume(slideFXVolume->GetValue());
		break;
	}
	default: break;
	}
	
	return true;
}

void MainMenu::UpdatingButtons(Input* input)
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