#include "App.h"
#include "Textures.h"

#include "CharacterManager.h"
#include "SceneGameplay.h"
#include "Player.h"

#include "Easings.h"

#include "GuiButton.h"

CharacterManager::CharacterManager(SceneGameplay* s, PlayerType type, Font* font) : scene(s)
{
	guiTex = app->tex->Load("Textures/UI/gui_character_swap_menu.png");

	btnHunter = new GuiButton(1, { 505, 0, 270, 64 }, "Hunter", this, font);
	btnHunter->texture = guiTex;
	btnHunter->section = { 0,656,270,64 };
	btnHunter->alineation = 1;

	btnWizard = new GuiButton(2, { 505, 0, 270, 64 }, "Wizard", this, font);
	btnWizard->texture = guiTex;
	btnWizard->section = { 0,656,270,64 };
	btnWizard->alineation = 1;

	btnThief = new GuiButton(3, { 505, 0, 270, 64 }, "Thief", this, font);
	btnThief->texture = guiTex;
	btnThief->section = { 0,656,270,64 };
	btnThief->alineation = 1;

	btnWarrior = new GuiButton(4, { 505, 0, 270, 64 }, "Warrior", this, font);
	btnWarrior->texture = guiTex;
	btnWarrior->section = { 0,656,270,64 };
	btnWarrior->alineation = 1;

	btnExit = new GuiButton(5, { 505, 0, 270, 64 }, "Return", this, font);
	btnExit->texture = guiTex;
	btnExit->section = { 0,656,270,64 };
	btnExit->alineation = 1;

	switch (type)
	{
	case PlayerType::HUNTER:
		btnHunter->state = GuiControlState::DISABLED;
		break;
	case PlayerType::WIZARD:
		btnWizard->state = GuiControlState::DISABLED;
		break;
	case PlayerType::THIEF:
		btnThief->state = GuiControlState::DISABLED;
		break;
	case PlayerType::WARRIOR:
		btnWarrior->state = GuiControlState::DISABLED;
		break;
	}

	easing = new Easing(true, 0, 0, 180, 80);
	easing2 = new Easing(false, 0, 180, -680, 90);
	counter = 0;
}

CharacterManager::~CharacterManager()
{
}

bool CharacterManager::Load(Font* font)
{
	//buttons.push_back(btnHunter);
	controls.push_back(btnWizard);
	controls.push_back(btnThief);
	controls.push_back(btnWarrior);
	controls.push_back(btnExit);

	//currentButton = (*buttons.begin().next());
	currentControl = (*controls.begin());
	lastControl = nullptr;

	return true;
}

bool CharacterManager::Update(float dt)
{
	UpdatingButtons(app->input);

	int id = -1;
	if (lastUserInput == 0 && currentControl != nullptr) id = currentControl->id;

	if (easing->easingsActivated)
	{
		btnHunter->bounds.y = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		btnWizard->bounds.y = easing->backEaseOut(easing->currentIteration, easing->initialPos, 265, easing->totalIterations);
		btnThief->bounds.y = easing->backEaseOut(easing->currentIteration, easing->initialPos, 350, easing->totalIterations);
		btnWarrior->bounds.y = easing->backEaseOut(easing->currentIteration, easing->initialPos, 435, easing->totalIterations);
		btnExit->bounds.y = easing->backEaseOut(easing->currentIteration, easing->initialPos, 517, easing->totalIterations);

		if (easing->currentIteration < easing->totalIterations)
			easing->currentIteration++;
		else
		{
			easing->currentIteration = 0;
			easing->easingsActivated = false;
		}
	}

	if (easing2->easingsActivated)
	{
		btnHunter->bounds.y = easing2->backEaseIn(easing2->currentIteration, easing2->initialPos, easing2->deltaPos, easing2->totalIterations);
		btnWizard->bounds.y = easing2->backEaseIn(easing2->currentIteration, 265, -765, easing2->totalIterations);
		btnThief->bounds.y = easing2->backEaseIn(easing2->currentIteration, 350, -850, easing2->totalIterations);
		btnWarrior->bounds.y = easing2->backEaseIn(easing2->currentIteration, 435, -935, easing2->totalIterations);
		btnExit->bounds.y = easing2->backEaseIn(easing2->currentIteration, 517, -1017, easing2->totalIterations);

		if (easing2->currentIteration < easing2->totalIterations)
			easing2->currentIteration++;
		else
		{
			easing2->currentIteration = 0;
			easing2->easingsActivated = false;
		}
	}

	btnHunter->Update(app->input, dt, id);
	btnWizard->Update(app->input, dt, id);
	btnThief->Update(app->input, dt, id);
	btnWarrior->Update(app->input, dt, id);
	btnExit->Update(app->input, dt, id);

	if (counter > 100 && app->frameCount - counter >= 120)
	{
		controls.clear();
		controls.push_back(btnHunter);
		controls.push_back(btnWizard);
		controls.push_back(btnThief);
		controls.push_back(btnWarrior);
		controls.push_back(btnExit);
		currentControl = (*controls.begin());

		btnHunter->bounds.y = 0;
		btnThief->bounds.y = 0;
		btnWarrior->bounds.y = 0;
		btnWizard->bounds.y = 0;
		btnExit->bounds.y = 0;
		easing->easingsActivated = true;
		counter = 0;

		scene->ChangeState(GameplayMenuState::NONE);
		currentControl = (*controls.begin());
	}

	return true;
}

void CharacterManager::Draw(Font* font, bool showColliders)
{
	// Black rectangle for the background
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 150, true, false);

	SDL_Rect section = { 0,0,430,650 };
	app->render->DrawTexture(guiTex, 425, btnHunter->bounds.y - 150, &section, false);

	btnHunter->Draw(app->render, showColliders, 36, { 0,0,0,255 });
	btnWizard->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
	btnThief->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
	btnWarrior->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
	btnExit->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
}

bool CharacterManager::UnLoad()
{
	app->tex->UnLoad(guiTex);

	RELEASE(easing);
	RELEASE(easing2);

	RELEASE(btnHunter);
	RELEASE(btnWizard);
	RELEASE(btnThief);
	RELEASE(btnWarrior);
	RELEASE(btnExit);

	controls.clear();

	return true;
}

bool CharacterManager::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) // Hunter
		{
			controls.clear();
			controls.push_back(btnWizard);
			controls.push_back(btnThief);
			controls.push_back(btnWarrior);
			controls.push_back(btnExit);

			scene->CharacterSwap(PlayerType::HUNTER);
			ChangeButtonState(PlayerType::HUNTER);
		}
		else if (control->id == 2) // Wizard
		{
			controls.clear();
			controls.push_back(btnHunter);
			controls.push_back(btnThief);
			controls.push_back(btnWarrior);
			controls.push_back(btnExit);
			currentControl = (*controls.begin());

			scene->CharacterSwap(PlayerType::WIZARD);
			ChangeButtonState(PlayerType::WIZARD);
		}
		else if (control->id == 3) // Thief
		{
			controls.clear();
			controls.push_back(btnHunter);
			controls.push_back(btnWizard);
			controls.push_back(btnWarrior);
			controls.push_back(btnExit);
			currentControl = (*controls.begin());

			scene->CharacterSwap(PlayerType::THIEF);
			ChangeButtonState(PlayerType::THIEF);
		}
		else if (control->id == 4) // Warrior
		{
			controls.clear();
			controls.push_back(btnHunter);
			controls.push_back(btnWizard);
			controls.push_back(btnThief);
			controls.push_back(btnExit);
			currentControl = (*controls.begin());

			scene->CharacterSwap(PlayerType::WARRIOR);
			ChangeButtonState(PlayerType::WARRIOR);
		}
		else if (control->id == 5) // Exit
		{
			counter = app->frameCount;
			easing2->easingsActivated = true;
		}

	}

	return true;
}

void CharacterManager::ChangeButtonState(PlayerType type)
{
	switch (type)
	{
	case PlayerType::HUNTER:
		btnHunter->state = GuiControlState::DISABLED;
		btnWizard->state = GuiControlState::NORMAL;
		btnThief->state = GuiControlState::NORMAL;
		btnWarrior->state = GuiControlState::NORMAL;
		break;
	case PlayerType::WIZARD:
		btnHunter->state = GuiControlState::NORMAL;
		btnWizard->state = GuiControlState::DISABLED;
		btnThief->state = GuiControlState::NORMAL;
		btnWarrior->state = GuiControlState::NORMAL;
		break;
	case PlayerType::THIEF:
		btnHunter->state = GuiControlState::NORMAL;
		btnWizard->state = GuiControlState::NORMAL;
		btnThief->state = GuiControlState::DISABLED;
		btnWarrior->state = GuiControlState::NORMAL;
		break;
	case PlayerType::WARRIOR:
		btnHunter->state = GuiControlState::NORMAL;
		btnWizard->state = GuiControlState::NORMAL;
		btnThief->state = GuiControlState::NORMAL;
		btnWarrior->state = GuiControlState::DISABLED;
		break;
	}
}

void CharacterManager::UpdatingButtons(Input* input)
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
				if ((*it)->id == currentControl->id + 1 || (*it)->id == currentControl->id + 2)
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
			eastl::list<GuiControl*>::iterator it = controls.end().prev();
			for (int i = controls.size() - 1; i >= 0; --i, --it)
			{
				if ((*it)->id == currentControl->id - 1 || (*it)->id == currentControl->id - 2)
				{
					currentControl = (*it);
					break;
				}
			}
		}
	}
}