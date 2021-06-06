#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"

#include "PauseMenu.h"
#include "SceneGameplay.h"

#include "Easings.h"

PauseMenu::PauseMenu(SceneGameplay* s) : scene(s)
{
	playMusicOptions = true;

	easing = new Easing(true, 0, 1280, -775, 90);
	easing2 = new Easing(false, 0, 505, 392, 60);
	easing3 = new Easing(false, 0, 505, 1280, 90);
	counter = 0;
}

PauseMenu::~PauseMenu()
{
}

bool PauseMenu::Load(Font* font)
{
	guiTex = app->tex->Load("Textures/UI/gui_pause_menu.png");

	savedTextFont = new Font(app, "Font/font3.xml", app->tex);

	// Main menu ========================
	btnResume = new GuiButton(1, { 1280, 223, 270, 64 }, "Resume Game", this, font);
	btnResume->section = { 430,0,270,64 };
	btnResume->texture = guiTex;
	btnResume->alineation = 1;

	btnSave = new GuiButton(2, { 1280, 308, 130, 64 }, "Save", this, font);
	btnSave->section = { 430,245,130,64 };
	btnSave->texture = guiTex;
	btnSave->alineation = 1;

	btnLoad = new GuiButton(3, { 1280 + 140, 308, 130, 64 }, "Load", this, font);
	btnLoad->section = { 430,245,130,64 };
	btnLoad->texture = guiTex;
	btnLoad->alineation = 1;

	btnOptions = new GuiButton(4, { 1280, 393, 270, 64 }, "Options", this, font);
	btnOptions->section = { 430,0,270,64 };
	btnOptions->texture = guiTex;
	btnOptions->alineation = 1;

	btnReturnTitle = new GuiButton(5, { 1280, 478, 270, 64 }, "Return to Title", this, font);
	btnReturnTitle->section = { 430,0,270,64 };
	btnReturnTitle->texture = guiTex;
	btnReturnTitle->alineation = 1;

	btnExit = new GuiButton(6, { 1280, 563, 270, 64 }, "Exit Game", this, font);
	btnExit->section = { 430,0,270,64 };
	btnExit->texture = guiTex;
	btnExit->alineation = 1;
	// Main menu ========================

	// Options menu ========================
	slideMusicVolume = new GuiSlider(7, { 505,198,339,46 }, "Music volume", this, 0, 128, app->audio->GetMusicVolume());
	slideMusicVolume->section = { 440,83,339,46 };
	slideMusicVolume->texture = guiTex;

	slideFXVolume = new GuiSlider(8, { 505,313,339,46 }, "FX volume", this, 0, 128, app->audio->GetFxVolume());
	slideFXVolume->section = { 440,83,339,46 };
	slideFXVolume->texture = guiTex;

	checkFullscreen = new GuiCheckBox(9, { 543,385,250,35 }, "Fullscreen", this, app->win->fullscreenWindow);
	checkFullscreen->section = { 440,192,32,32 };
	checkFullscreen->texture = guiTex;

	btnOptionsBack = new GuiButton(10, { 538, 561, 270, 64 }, "Back", this, font);
	btnOptionsBack->section = { 430,0,270,64 };
	btnOptionsBack->texture = guiTex;
	btnOptionsBack->alineation = 1;
	// Options menu ========================

	// Return to title menu ========================
	btnReturnTitleYes = new GuiButton(11, { 505, 256, 270, 64 }, "Yes", this, font);
	btnReturnTitleYes->section = { 430,0,270,64 };
	btnReturnTitleYes->texture = guiTex;
	btnReturnTitleYes->alineation = 1;

	btnReturnTitleNo = new GuiButton(12, { 505, 341, 270, 64 }, "No", this, font);
	btnReturnTitleNo->section = { 430,0,270,64 };
	btnReturnTitleNo->texture = guiTex;
	btnReturnTitleNo->alineation = 1;
	// Return to title menu ========================

	// Exit menu ========================
	btnExitYes = new GuiButton(13, { 505, 256, 270, 64 }, "Yes", this, font);
	btnExitYes->section = { 430,0,270,64 };
	btnExitYes->texture = guiTex;
	btnExitYes->alineation = 1;

	btnExitNo = new GuiButton(14, { 505, 341, 270, 64 }, "No", this, font);
	btnExitNo->section = { 430,0,270,64 };
	btnExitNo->texture = guiTex;
	btnExitNo->alineation = 1;
	// Exit menu ========================


	controls.push_back(btnResume);
	controls.push_back(btnSave);
	controls.push_back(btnLoad);
	controls.push_back(btnOptions);
	controls.push_back(btnReturnTitle);
	controls.push_back(btnExit);
	//currentButton = (*buttons.begin());
	currentControl = nullptr;
	lastControl = nullptr;

	checkVSync = new GuiCheckBox(15, { 578,460,170,35 }, "VSync", this, true);
	checkVSync->section = { 440,192,32,32 };
	checkVSync->texture = guiTex;
	checkVSync->state = GuiControlState::DISABLED;

	state = PauseState::DEFAULT;

	savedGame = false;
	savedTextAlpha = 1.0f;
	toStartTimer = 1.0f;

	return true;
}

bool PauseMenu::Update(float dt)
{
	bool ret = true;

	UpdatingButtons(app->input);

	int id = -1;
	if (lastUserInput == 0 && currentControl != nullptr) id = currentControl->id;

	if (easing->easingsActivated)
	{
		btnExit->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		btnResume->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		btnSave->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		btnLoad->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos + 140, easing->deltaPos, easing->totalIterations);
		btnReturnTitle->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		btnOptions->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);

		if (easing->currentIteration < easing->totalIterations)
		{
			easing->currentIteration++;
		}
		else
		{
			easing->currentIteration = 0;
			easing->easingsActivated = false;
		}
	}


	if (easing3->easingsActivated)
	{
		btnExit->bounds.x = easing3->backEaseIn(easing3->currentIteration, easing3->initialPos, easing3->deltaPos, easing3->totalIterations);
		btnResume->bounds.x = easing3->backEaseIn(easing3->currentIteration, easing3->initialPos, easing3->deltaPos, easing3->totalIterations);
		btnSave->bounds.x = easing3->backEaseIn(easing3->currentIteration, easing3->initialPos, easing3->deltaPos, easing3->totalIterations);
		btnLoad->bounds.x = easing3->backEaseIn(easing3->currentIteration, easing3->initialPos + 140, easing3->deltaPos, easing3->totalIterations);
		btnReturnTitle->bounds.x = easing3->backEaseIn(easing3->currentIteration, easing3->initialPos, easing3->deltaPos, easing3->totalIterations);
		btnOptions->bounds.x = easing3->backEaseIn(easing3->currentIteration, easing3->initialPos, easing3->deltaPos, easing3->totalIterations);

		if (easing3->currentIteration < easing3->totalIterations)
		{
			easing3->currentIteration++;
		}
		else
		{
			easing3->currentIteration = 0;
			easing3->easingsActivated = false;

			controls.clear();
			controls.push_back(btnResume);
			controls.push_back(btnSave);
			controls.push_back(btnLoad);
			controls.push_back(btnOptions);
			controls.push_back(btnReturnTitle);
			controls.push_back(btnExit);
			currentControl = nullptr;
			lastControl = nullptr;

			easing->easingsActivated = true;
			btnExit->bounds.x = 1280;
			btnLoad->bounds.x = 1280 + 140;
			btnOptions->bounds.x = 1280;
			btnResume->bounds.x = 1280;
			btnReturnTitle->bounds.x = 1280;
			btnSave->bounds.x = 1280;

			state = PauseState::DEFAULT;
			scene->ChangeState(GameplayMenuState::NONE);
		}
	}

	if (easing2->easingsActivated)
	{
		slideFXVolume->bounds.x = easing2->backEaseOut(easing2->currentIteration, easing2->initialPos, easing2->deltaPos, easing2->totalIterations);
		slideMusicVolume->bounds.x = easing2->backEaseOut(easing2->currentIteration, easing2->initialPos, easing2->deltaPos, easing2->totalIterations);
		checkFullscreen->bounds.x = easing2->backEaseOut(easing2->currentIteration, easing2->initialPos + 38, easing2->deltaPos, easing2->totalIterations);
		btnOptionsBack->bounds.x = easing2->backEaseOut(easing2->currentIteration, easing2->initialPos + 33, easing2->deltaPos, easing2->totalIterations);
		checkVSync->bounds.x = easing2->backEaseOut(easing2->currentIteration, easing2->initialPos + 73, easing2->deltaPos, easing2->totalIterations);

		if (easing2->currentIteration < easing2->totalIterations)
		{
			easing2->currentIteration++;
		}
		else
		{
			easing2->currentIteration = 0;
			easing2->easingsActivated = false;
		}
	}

	switch (state)
	{
	case PauseState::DEFAULT:

		if (playMusicOptions == false)
		{
			app->audio->PlayMusic("Audio/Music/village_theme_1.ogg");
			playMusicOptions = true;
		}

		btnResume->Update(app->input, dt, id);
		btnSave->Update(app->input, dt, id);
		btnLoad->Update(app->input, dt, id);
		btnOptions->Update(app->input, dt, id);
		btnReturnTitle->Update(app->input, dt, id);
		btnExit->Update(app->input, dt, id);

		break;
	case PauseState::OPTIONS:

		if (playMusicOptions == true)
		{
			app->audio->PlayMusic("Audio/Music/options_theme.ogg");
			playMusicOptions = false;
		}

		slideMusicVolume->Update(app->input, dt, id);
		slideFXVolume->Update(app->input, dt, id);
		checkFullscreen->Update(app->input, dt, id);
		//checkVSync->Update(app->input, dt, id);
		btnOptionsBack->Update(app->input, dt, id);

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

	if (savedGame)
	{
		toStartTimer -= dt;
		if (toStartTimer < -0.01)
		{
			savedTextAlpha -= dt;
			if (savedTextAlpha < -0.01)
			{
				toStartTimer = 1.0f;
				savedTextAlpha = 1.0f;
				savedGame = false;
			}
		}
	}

	LOG("TO START  %f", toStartTimer);
	LOG("ALPHA  %f", savedTextAlpha);

	return ret;
}

void PauseMenu::Draw(Font* font, bool showColliders)
{
	SDL_Rect section;

	switch (state)
	{
	case PauseState::DEFAULT:
		// Black rectangle for the background
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120, true, false);

		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, (btnExit->bounds.x - 80), 48, &section, false);

		btnResume->Draw(app->render, showColliders, 36, {0, 0, 0, 255});
		btnSave->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnLoad->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnOptions->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnReturnTitle->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnExit->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });

		app->render->DrawText(font, "PAUSE", (btnExit->bounds.x + 45), 121, 72, 5, { 255,255,255,255 });

		break;
	case PauseState::OPTIONS:
		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, 425, 48, &section, false);

		btnResume->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnSave->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnLoad->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnOptions->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnReturnTitle->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
		btnExit->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });

		app->render->DrawText(font, "PAUSE", 552, 121, 72, 5, { 255,255,255,255 });

		// Black rectangle for the background
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120, true, false);

		app->render->DrawTexture(guiTex, slideFXVolume->bounds.x - 45, 48, &section, false);

		btnOptionsBack->Draw(app->render, showColliders, 36, { 0,0,0,225 });
		slideMusicVolume->Draw(app->render, showColliders);
		slideFXVolume->Draw(app->render, showColliders);
		checkFullscreen->Draw(app->render, showColliders);
		checkVSync->Draw(app->render, showColliders);

		app->render->DrawText(font, "Music volume", slideMusicVolume->bounds.x + 65, 150, 36, 5, { 255,255,255,255 });
		app->render->DrawText(font, "FX volume", slideFXVolume->bounds.x + 65, 264, 36, 5, { 255,255,255,255 });
		app->render->DrawText(font, "Fullscreen", checkFullscreen->bounds.x, 386, 36, 5, { 255,255,255,255 });
		app->render->DrawText(font, "VSync", checkVSync->bounds.x, 459, 36, 5, { 255,255,255,255 });

		break;
	case PauseState::RETURN_TITLE:
		// Black rectangle for the background
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120, true, false);

		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, 425, 48, &section, false);

		btnReturnTitleYes->Draw(app->render, showColliders, 36, { 0,0,0,225 });
		btnReturnTitleNo->Draw(app->render, showColliders, 36, { 0,0,0,225 });

		app->render->DrawText(font, "Are you sure?", 507, 147, 40, 5, { 255,255,255,255 });
		break;
	case PauseState::EXIT:
		// Black rectangle for the background
		app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120, true, false);

		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, 425, 48, &section, false);
		
		btnExitYes->Draw(app->render, showColliders, 36, { 0,0,0,225 });
		btnExitNo->Draw(app->render, showColliders, 36, { 0,0,0,225 });

		app->render->DrawText(font, "Are you sure?", 507, 147, 40, 5, { 255,255,255,255 });
		break;
	}

	if (savedGame == true)
	{
		app->render->DrawText(savedTextFont, "The game has been saved", { 10,676,1000,500 }, 36, 5, { 255,255,255,(unsigned char)(savedTextAlpha * 255) });
	}
}

bool PauseMenu::UnLoad()
{
	app->tex->UnLoad(guiTex);

	RELEASE(btnResume);
	RELEASE(btnSave);
	RELEASE(btnLoad);
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

	RELEASE(easing);
	RELEASE(easing2);
	RELEASE(easing3);

	controls.clear();

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
			easing3->easingsActivated = true;
		}
		else if (control->id == 2) // Save
		{
			if (savedGame == false)
			{
				savedGame = true;
				app->SaveGameRequest();
			}
		}
		else if (control->id == 3) // Load
		{
			app->LoadGameRequest();
		}
		else if (control->id == 4) // Options
		{
			state = PauseState::OPTIONS;

			controls.clear();
			controls.push_back(btnOptionsBack);
			controls.push_back(slideMusicVolume);
			controls.push_back(slideFXVolume);
			controls.push_back(checkFullscreen);

			easing2->easingsActivated = true;

			lastControl = currentControl;
			currentControl = (*controls.begin());
		}
		else if (control->id == 5) // Return to title
		{
			state = PauseState::RETURN_TITLE;

			controls.clear();
			controls.push_back(btnReturnTitleYes);
			controls.push_back(btnReturnTitleNo);
			lastControl = currentControl;
			currentControl = (*controls.end().prev());
		}
		else if (control->id == 6) // Exit
		{
			state = PauseState::EXIT;

			controls.clear();
			controls.push_back(btnExitYes);
			controls.push_back(btnExitNo);
			lastControl = currentControl;
			currentControl = (*controls.end().prev());
		}
		else if (control->id == 10 || control->id == 12 || control->id == 14) // Back from options
		{
			state = PauseState::DEFAULT;

			controls.clear();
			controls.push_back(btnResume);
			controls.push_back(btnSave);
			controls.push_back(btnLoad);
			controls.push_back(btnOptions);
			controls.push_back(btnReturnTitle);
			controls.push_back(btnExit);

			btnOptionsBack->bounds.x = 538;
			checkFullscreen->bounds.x = 543;
			checkVSync->bounds.x = 578;
			slideFXVolume->bounds.x = 505;
			slideMusicVolume->bounds.x = 505;

			currentControl = lastControl;
		}
		else if (control->id == 11) // Return to title yes
			scene->TransitionToScene(SceneType::TITLE, TransitionType::WIPE);

		else if (control->id == 13) // Exit yes
			return false;
		
		//else if (control->id == 15) // Return to title no
		//{
		//	state = PauseState::DEFAULT;

		//	controls.clear();
		//	controls.push_back(btnResume);
		//	controls.push_back(btnSave);
		//	controls.push_back(btnLoad);
		//	controls.push_back(btnOptions);
		//	controls.push_back(btnReturnTitle);
		//	controls.push_back(btnExit);

		//	currentControl = lastControl;
		//}
		//else if (control->id == 16) scene->TransitionToScene(SceneType::TITLE, TransitionType::FADE_TO_BLACK);
		//else if (control->id == 17)
		//{
		//	state = PauseState::DEFAULT;

		//	controls.clear();
		//	controls.push_back(btnResume);
		//	controls.push_back(btnSave);
		//	controls.push_back(btnLoad);
		//	controls.push_back(btnOptions);
		//	controls.push_back(btnReturnTitle);
		//	controls.push_back(btnExit);

		//	currentControl = lastControl;
		//}
	}
	break;
	case GuiControlType::SLIDER:
	{
		if (control->id == 7) app->audio->SetMusicVolume(slideMusicVolume->GetValue()); // Music Volume
		else if (control->id == 8) app->audio->SetFxVolume(slideFXVolume->GetValue()); // FX Volume
	}
	break;
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 9) // Fullscreen
			app->win->SetFullscreen();

		else if (control->id == 15); // VSync
	}
	break;
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