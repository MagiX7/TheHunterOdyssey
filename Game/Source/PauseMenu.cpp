#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"

#include "PauseMenu.h"
#include "SceneGameplay.h"

PauseMenu::PauseMenu(SceneGameplay* s) : scene(s)
{
	playMusicOptions = true;
}

PauseMenu::~PauseMenu()
{
}

bool PauseMenu::Load(Font* font)
{
	guiTex = app->tex->Load("Assets/Textures/UI/gui_pause_menu.png");

	btnResume = new GuiButton(1, { 505, 223, 270, 64 }, "Resume Game", this, font);
	btnResume->section = { 430,0,270,64 };
	btnResume->texture = guiTex;
	btnResume->alineation = 1;

	btnLoadSave = new GuiButton(2, { 505, 308, 270, 64 }, "Load/Save", this, font);
	btnLoadSave->section = { 430,0,270,64 };
	btnLoadSave->texture = guiTex;
	btnLoadSave->alineation = 1;

	btnOptions = new GuiButton(3, { 505, 393, 270, 64 }, "Options", this, font);
	btnOptions->section = { 430,0,270,64 };
	btnOptions->texture = guiTex;
	btnOptions->alineation = 1;

	btnReturnTitle = new GuiButton(4, { 505, 478, 270, 64 }, "Return to Title", this, font);
	btnReturnTitle->section = { 430,0,270,64 };
	btnReturnTitle->texture = guiTex;
	btnReturnTitle->alineation = 1;

	btnExit = new GuiButton(5, { 505, 563, 270, 64 }, "Exit Game", this, font);
	btnExit->section = { 430,0,270,64 };
	btnExit->texture = guiTex;
	btnExit->alineation = 1;

	btnSave = new GuiButton(6, { 505, 256, 270, 64 }, "Save", this, font);
	btnSave->section = { 430,0,270,64 };
	btnSave->texture = guiTex;
	btnSave->alineation = 1;

	btnLoad = new GuiButton(7, { 505, 341, 270, 64 }, "Load", this, font);
	btnLoad->section = { 430,0,270,64 };
	btnLoad->texture = guiTex;
	btnLoad->alineation = 1;

	btnBack = new GuiButton(8, { 505, 528, 270, 64 }, "Back", this, font);
	btnBack->section = { 430,0,270,64 };
	btnBack->texture = guiTex;
	btnBack->alineation = 1;

	btnOptionsBack = new GuiButton(9, { 930, 561, 270, 64 }, "Back", this, font);
	btnOptionsBack->section = { 430,0,270,64 };
	btnOptionsBack->texture = guiTex;
	btnOptionsBack->alineation = 1;

	slideMusicVolume = new GuiSlider(10, { 897,198,339,46 }, "Music volume", this, 0, 128, app->audio->GetMusicVolume());
	slideMusicVolume->section = { 440,83,339,46 };
	slideMusicVolume->texture = guiTex;

	slideFXVolume = new GuiSlider(11, { 897,313,339,46 }, "FX volume", this, 0, 128, app->audio->GetFxVolume());
	slideFXVolume->section = { 440,83,339,46 };
	slideFXVolume->texture = guiTex;

	checkFullscreen = new GuiCheckBox(12, { 935,385,250,35 }, "Fullscreen", this, app->win->fullscreenWindow);
	checkFullscreen->section = { 440,192,32,32 };
	checkFullscreen->texture = guiTex;

	checkVSync = new GuiCheckBox(13, { 977,460,170,35 }, "VSync", this, false);
	checkVSync->section = { 440,192,32,32 };
	checkVSync->texture = guiTex;

	btnExitYes = new GuiButton(14, { 505, 256, 270, 64 }, "Yes", this, font);
	btnExitYes->section = { 430,0,270,64 };
	btnExitYes->texture = guiTex;
	btnExitYes->alineation = 1;

	btnExitNo = new GuiButton(15, { 505, 341, 270, 64 }, "No", this, font);
	btnExitNo->section = { 430,0,270,64 };
	btnExitNo->texture = guiTex;
	btnExitNo->alineation = 1;

	btnReturnTitleYes = new GuiButton(16, { 505, 256, 270, 64 }, "Yes", this, font);
	btnReturnTitleYes->section = { 430,0,270,64 };
	btnReturnTitleYes->texture = guiTex;
	btnReturnTitleYes->alineation = 1;

	btnReturnTitleNo = new GuiButton(17, { 505, 341, 270, 64 }, "No", this, font);
	btnReturnTitleNo->section = { 430,0,270,64 };
	btnReturnTitleNo->texture = guiTex;
	btnReturnTitleNo->alineation = 1;

	buttons.push_back(btnResume);
	buttons.push_back(btnLoadSave);
	buttons.push_back(btnOptions);
	buttons.push_back(btnReturnTitle);
	buttons.push_back(btnExit);
	//currentButton = (*buttons.begin());
	currentButton = nullptr;
	lastButton = nullptr;

	state = PauseState::DEFAULT;

	return true;
}

bool PauseMenu::Update(float dt)
{
	bool ret = true;

	UpdatingButtons(app->input);

	int id = -1;
	if (lastUserInput == 0 && currentButton != nullptr)
	{
		id = currentButton->id;
	}

	switch (state)
	{
	case PauseState::DEFAULT:

		if (playMusicOptions == false)
		{
			app->audio->PlayMusic("Assets/Audio/Music/village_theme_1.ogg");
			playMusicOptions = true;
		}

		btnResume->Update(app->input, dt, id);
		btnLoadSave->Update(app->input, dt, id);
		btnOptions->Update(app->input, dt, id);
		btnReturnTitle->Update(app->input, dt, id);
		btnExit->Update(app->input, dt, id);

		break;
	case PauseState::OPTIONS:

		if (playMusicOptions == true)
		{
			app->audio->PlayMusic("Assets/Audio/Music/options_theme.ogg");
			playMusicOptions = false;
		}

		btnOptionsBack->Update(app->input, dt, id);

		slideMusicVolume->Update(app->input, dt);
		slideFXVolume->Update(app->input, dt);
		checkFullscreen->Update(app->input, dt);
		checkVSync->Update(app->input, dt);

		break;
	case PauseState::SAVE:
		if (scene->isTown == false)
		{
			btnSave->state = GuiControlState::DISABLED;
			btnLoad->state = GuiControlState::DISABLED;
		}
		else
		{
			btnSave->state = GuiControlState::FOCUSED;
			btnLoad->state = GuiControlState::FOCUSED;
		}
		btnSave->Update(app->input, dt, id);
		btnLoad->Update(app->input, dt, id);
		btnBack->Update(app->input, dt, id);

		break;
	case PauseState::RETURN_TITLE:
		btnReturnTitleYes->Update(app->input, dt, id);
		btnReturnTitleNo->Update(app->input, dt, id);

		break;
	case PauseState::EXIT:
		ret = btnExitYes->Update(app->input, dt, id);
		btnExitNo->Update(app->input, dt, id);

		break;
	}

	return ret;
}

void PauseMenu::Draw(Font* font, bool showColliders)
{
	SDL_Rect section;

	switch (state)
	{
	case PauseState::DEFAULT:
		// Black rectangle for the background
		app->render->DrawRectangle({ 0 - app->render->camera.x, 0 - app->render->camera.y, 1280, 720 }, 0, 0, 0, 120);

		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, -app->render->camera.x + 425, -app->render->camera.y + 48, &section);

		btnResume->Draw(app->render, showColliders, 36, {0, 0, 0, 255});
		btnLoadSave->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnOptions->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnReturnTitle->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnExit->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });

		app->render->DrawText(font, "PAUSE", 552, 121, 72, 5, { 255,255,255,255 });

		break;
	case PauseState::OPTIONS:
		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, -app->render->camera.x + 425, -app->render->camera.y + 48, &section);

		btnResume->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnLoadSave->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnOptions->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnReturnTitle->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnExit->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });

		app->render->DrawText(font, "PAUSE", 552, 121, 72, 5, { 255,255,255,255 });

		// Black rectangle for the background
		app->render->DrawRectangle({ 0 - app->render->camera.x, 0 - app->render->camera.y, 1280, 720 }, 0, 0, 0, 120);

		app->render->DrawTexture(guiTex, -app->render->camera.x + 850, -app->render->camera.y + 48, &section);

		btnOptionsBack->Draw(app->render, showColliders, 36, { 0,0,0,225 });
		slideMusicVolume->Draw(app->render, showColliders);
		slideFXVolume->Draw(app->render, showColliders);
		checkFullscreen->Draw(app->render, showColliders);
		checkVSync->Draw(app->render, showColliders);

		app->render->DrawText(font, "Music volume", 954, 150, 36, 5, { 255,255,255,255 });
		app->render->DrawText(font, "FX volume", 982, 264, 36, 5, { 255,255,255,255 });
		app->render->DrawText(font, "Fullscreen", 944, 386, 36, 5, { 255,255,255,255 });
		app->render->DrawText(font, "VSync", 985, 459, 36, 5, { 255,255,255,255 });
		break;
	case PauseState::SAVE:
		// Black rectangle for the background
		app->render->DrawRectangle({ 0 - app->render->camera.x, 0 - app->render->camera.y, 1280, 720 }, 0, 0, 0, 120);

		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, -app->render->camera.x + 425, -app->render->camera.y + 48, &section);

		btnSave->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnLoad->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnBack->Draw(app->render, showColliders, 36, {0, 0, 0, 255});

		app->render->DrawText(font, "PAUSE", 552, 121, 72, 5, { 255,255,255,255 });

		break;
	case PauseState::RETURN_TITLE:
		// Black rectangle for the background
		app->render->DrawRectangle({ 0 - app->render->camera.x, 0 - app->render->camera.y, 1280, 720 }, 0, 0, 0, 120);

		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, -app->render->camera.x + 425, -app->render->camera.y + 48, &section);

		btnReturnTitleYes->Draw(app->render, showColliders, 36, { 0,0,0,225 });
		btnReturnTitleNo->Draw(app->render, showColliders, 36, { 0,0,0,225 });

		app->render->DrawText(font, "Are you sure?", 507, 147, 40, 5, { 255,255,255,255 });
		break;
	case PauseState::EXIT:
		// Black rectangle for the background
		app->render->DrawRectangle({ 0 - app->render->camera.x, 0 - app->render->camera.y, 1280, 720 }, 0, 0, 0, 120);

		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, -app->render->camera.x + 425, -app->render->camera.y + 48, &section);
		
		btnExitYes->Draw(app->render, showColliders, 36, { 0,0,0,225 });
		btnExitNo->Draw(app->render, showColliders, 36, { 0,0,0,225 });

		app->render->DrawText(font, "Are you sure?", 507, 147, 40, 5, { 255,255,255,255 });
		break;
	}
}

bool PauseMenu::UnLoad()
{
	app->tex->UnLoad(guiTex);

	RELEASE(btnResume);
	RELEASE(btnLoadSave);
	RELEASE(btnSave);
	RELEASE(btnLoad);
	RELEASE(btnBack);
	RELEASE(btnOptions);
	RELEASE(btnReturnTitle);
	RELEASE(btnExit);
	RELEASE(btnOptionsBack);

	RELEASE(btnExitYes);
	RELEASE(btnExitNo);

	RELEASE(btnReturnTitleYes);
	RELEASE(btnReturnTitleNo);
	
	RELEASE(slideMusicVolume);
	RELEASE(slideFXVolume);

	RELEASE(checkFullscreen);
	RELEASE(checkVSync);

	buttons.clear();

	return true;
}

bool PauseMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) // Resume
		{
			// Reset all the buttons like if it was the first time user presses pause menu button
			buttons.clear();
			buttons.push_back(btnResume);
			buttons.push_back(btnLoadSave);
			buttons.push_back(btnOptions);
			buttons.push_back(btnReturnTitle);
			buttons.push_back(btnExit);
			currentButton = nullptr;
			lastButton = nullptr;

			state = PauseState::DEFAULT;
			scene->ChangeState(GameplayMenuState::NONE);
		}
		else if (control->id == 2) // Load / Save
		{
			state = PauseState::SAVE;

			pugi::xml_document saveFile;
			pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

			if (result == NULL) btnLoad->state = GuiControlState::DISABLED;
			else btnLoad->state = GuiControlState::NORMAL;

			buttons.clear();
			if (scene->isTown)
			{
				buttons.push_back(btnSave);
				buttons.push_back(btnLoad);
			}
			buttons.push_back(btnBack);
			currentButton = nullptr;
			lastButton = nullptr;

		}
		else if (control->id == 3) // Options
		{
			state = PauseState::OPTIONS;

			buttons.clear();
			buttons.push_back(btnOptionsBack);
			lastButton = currentButton;
			currentButton = (*buttons.begin());
		}
		else if (control->id == 4) // Return to title
		{
			state = PauseState::RETURN_TITLE;

			buttons.clear();
			buttons.push_back(btnReturnTitleYes);
			buttons.push_back(btnReturnTitleNo);
			lastButton = currentButton;
			currentButton = (*buttons.end().prev());
		}
		else if (control->id == 5) // Exit
		{
			state = PauseState::EXIT;

			buttons.clear();
			buttons.push_back(btnExitYes);
			buttons.push_back(btnExitNo);
			lastButton = currentButton;
			currentButton = (*buttons.end().prev());
		}
		else if (control->id == 6) app->SaveGameRequest();// Save Game
		else if (control->id == 7) app->LoadGameRequest(); // Load Game
		else if (control->id == 8) // Back
		{
			state = PauseState::DEFAULT;

			buttons.clear();
			buttons.push_back(btnResume);
			buttons.push_back(btnLoadSave);
			buttons.push_back(btnOptions);
			buttons.push_back(btnReturnTitle);
			buttons.push_back(btnExit);

			currentButton = lastButton;
		}
		else if (control->id == 9) // Back from options
		{
			state = PauseState::DEFAULT;

			buttons.clear();
			buttons.push_back(btnResume);
			buttons.push_back(btnLoadSave);
			buttons.push_back(btnOptions);
			buttons.push_back(btnReturnTitle);
			buttons.push_back(btnExit);

			currentButton = lastButton;
		}
		else if (control->id == 14)	return false;
		else if (control->id == 15)
		{
			state = PauseState::DEFAULT;

			buttons.clear();
			buttons.push_back(btnResume);
			buttons.push_back(btnLoadSave);
			buttons.push_back(btnOptions);
			buttons.push_back(btnReturnTitle);
			buttons.push_back(btnExit);

			currentButton = lastButton;
		}
		else if (control->id == 16)	scene->TransitionToScene(SceneType::TITLE);
		else if (control->id == 17)
		{
			state = PauseState::DEFAULT;

			buttons.clear();
			buttons.push_back(btnResume);
			buttons.push_back(btnLoadSave);
			buttons.push_back(btnOptions);
			buttons.push_back(btnReturnTitle);
			buttons.push_back(btnExit);

			currentButton = lastButton;
		}
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 10) app->audio->SetMusicVolume(slideMusicVolume->GetValue()); // Music Volume
		else if (control->id == 11) app->audio->SetFxVolume(slideFXVolume->GetValue()); // FX Volume
	}
	case GuiControlType::CHECKBOX:
	{
	if (control->id == 12) // Fullscreen
	{
		app->win->SetFullscreen();
	}
	else if (control->id == 13); // VSync
	}
	}
	
	return true;
}

void PauseMenu::UpdatingButtons(Input* input)
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
		if (currentButton == nullptr)
		{
			currentButton = (*buttons.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
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
	}
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
	{
		if (currentButton == nullptr)
		{
			currentButton = (*buttons.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
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
}