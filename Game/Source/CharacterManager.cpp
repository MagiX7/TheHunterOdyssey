#include "App.h"

#include "CharacterManager.h"
#include "SceneGameplay.h"
#include "Player.h"

#include "GuiButton.h"
#include "Textures.h"

CharacterManager::CharacterManager(SceneGameplay* s, PlayerType type, Font* font) : scene(s)
{
	guiTex = app->tex->Load("Assets/Textures/UI/gui_character_swap_menu.png");

	btnHunter = new GuiButton(1, { 505, 180, 270, 64 }, "Hunter", this, font);
	btnHunter->texture = guiTex;
	btnHunter->section = { 0,656,270,64 };
	btnHunter->alineation = 1;

	btnWizard = new GuiButton(2, { 505, 265, 270, 64 }, "Wizard", this, font);
	btnWizard->texture = guiTex;
	btnWizard->section = { 0,656,270,64 };
	btnWizard->alineation = 1;

	btnThief = new GuiButton(3, { 505, 350, 270, 64 }, "Thief", this, font);
	btnThief->texture = guiTex;
	btnThief->section = { 0,656,270,64 };
	btnThief->alineation = 1;

	btnWarrior = new GuiButton(4, { 505, 435, 270, 64 }, "Warrior", this, font);
	btnWarrior->texture = guiTex;
	btnWarrior->section = { 0,656,270,64 };
	btnWarrior->alineation = 1;

	btnExit = new GuiButton(5, { 505, 517, 270, 64 }, "Return", this, font);
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
}

CharacterManager::~CharacterManager()
{
}

bool CharacterManager::Load(Font* font)
{
	//buttons.push_back(btnHunter);
	buttons.push_back(btnWizard);
	buttons.push_back(btnThief);
	buttons.push_back(btnWarrior);
	buttons.push_back(btnExit);

	//currentButton = (*buttons.begin().next());
	currentButton = nullptr;
	lastButton = nullptr;

	return true;
}

bool CharacterManager::Update(float dt)
{
	UpdatingButtons(app->input);

	int id = -1;
	if (lastUserInput == 0 && currentButton != nullptr)
	{
		id = currentButton->id;
	}

	btnHunter->Update(app->input, dt, id);
	btnWizard->Update(app->input, dt, id);
	btnThief->Update(app->input, dt, id);
	btnWarrior->Update(app->input, dt, id);
	btnExit->Update(app->input, dt, id);

	return true;
}

void CharacterManager::Draw(Font* font, bool showColliders)
{
	SDL_Rect section = { 0,0,430,650 };
	app->render->DrawTexture(guiTex, 425, 48, &section, false);

	btnHunter->Draw(app->render, showColliders, 36, { 0,0,0,255 });
	btnWizard->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
	btnThief->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
	btnWarrior->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
	btnExit->Draw(app->render, showColliders, 36, { 0, 0, 0, 255 });
}

bool CharacterManager::UnLoad()
{
	app->tex->UnLoad(guiTex);

	RELEASE(btnHunter);
	RELEASE(btnWizard);
	RELEASE(btnThief);
	RELEASE(btnWarrior);
	RELEASE(btnExit);

	buttons.clear();

	return true;
}

bool CharacterManager::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) // Hunter
		{
			buttons.clear();
			buttons.push_back(btnWizard);
			buttons.push_back(btnThief);
			buttons.push_back(btnWarrior);
			buttons.push_back(btnExit);

			scene->CharacterSwap(PlayerType::HUNTER);
			ChangeButtonState(PlayerType::HUNTER);
		}
		else if (control->id == 2) // Wizard
		{
			buttons.clear();
			buttons.push_back(btnHunter);
			buttons.push_back(btnThief);
			buttons.push_back(btnWarrior);
			buttons.push_back(btnExit);
			currentButton = (*buttons.begin());

			scene->CharacterSwap(PlayerType::WIZARD);
			ChangeButtonState(PlayerType::WIZARD);
		}
		else if (control->id == 3) // Thief
		{
			buttons.clear();
			buttons.push_back(btnHunter);
			buttons.push_back(btnWizard);
			buttons.push_back(btnWarrior);
			buttons.push_back(btnExit);
			currentButton = (*buttons.begin());

			scene->CharacterSwap(PlayerType::THIEF);
			ChangeButtonState(PlayerType::THIEF);
		}
		else if (control->id == 4) // Warrior
		{
			buttons.clear();
			buttons.push_back(btnHunter);
			buttons.push_back(btnWizard);
			buttons.push_back(btnThief);
			buttons.push_back(btnExit);
			currentButton = (*buttons.begin());

			scene->CharacterSwap(PlayerType::WARRIOR);
			ChangeButtonState(PlayerType::WARRIOR);
		}
		else if (control->id == 5) // Exit
		{
			buttons.clear();
			buttons.push_back(btnHunter);
			buttons.push_back(btnWizard);
			buttons.push_back(btnThief);
			buttons.push_back(btnWarrior);
			buttons.push_back(btnExit);
			currentButton = (*buttons.begin());
			
			scene->ChangeState(GameplayMenuState::NONE);
			currentButton = (*buttons.begin());
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
				if ((*it)->id == currentButton->id + 1 || (*it)->id == currentButton->id + 2)
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
			eastl::list<GuiButton*>::iterator it = buttons.end().prev();
			for (int i = buttons.size() - 1; i >= 0; --i, --it)
			{
				if ((*it)->id == currentButton->id - 1 || (*it)->id == currentButton->id - 2)
				{
					currentButton = (*it);
					break;
				}
			}
		}
	}
}