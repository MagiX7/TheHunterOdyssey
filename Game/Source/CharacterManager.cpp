#include "App.h"
#include "CharacterManager.h"
#include "Player.h"

#include "SceneGameplay.h"

#include "GuiButton.h"

CharacterManager::CharacterManager(Player *pl, SceneGameplay* s) : player(pl), scene(s)
{
}

CharacterManager::~CharacterManager()
{
}

bool CharacterManager::Load(Font* font)
{
	btnHunter = new GuiButton(1, { 466, 150, 360, 64 }, "Hunter", this, font);
	btnHunter->alineation = 1;

	btnWizard = new GuiButton(2, { 466, 250, 360, 64 }, "Wizard", this, font);
	btnWizard->alineation = 1;

	btnThief = new GuiButton(3, { 466, 350, 360, 64 }, "Thief", this, font);
	btnThief->alineation = 1;

	btnWarrior = new GuiButton(4, { 466, 450, 360, 64 }, "Warrior", this, font);
	btnWarrior->alineation = 1;

	btnExit = new GuiButton(5, { 600, 550, 64, 64 }, "Exit", this, font);
	btnExit->alineation = 1;

	switch (player->playerType)
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

	return true;
}

bool CharacterManager::Update(float dt)
{
	/*btnHunter->Update(app->input, dt);
	btnWizard->Update(app->input, dt);
	btnThief->Update(app->input, dt);
	btnWarrior->Update(app->input, dt);
	btnExit->Update(app->input, dt);*/

	return true;
}

void CharacterManager::Draw(Font* font, bool showColliders)
{
	btnHunter->Draw(app->render, showColliders, 36);
	btnWizard->Draw(app->render, showColliders, 36);
	btnThief->Draw(app->render, showColliders, 36);
	btnWarrior->Draw(app->render, showColliders, 36);
	btnExit->Draw(app->render, showColliders, 36);
}

bool CharacterManager::UnLoad()
{
	RELEASE(btnHunter);
	RELEASE(btnWizard);
	RELEASE(btnThief);
	RELEASE(btnWarrior);
	RELEASE(btnExit);

	return true;
}

bool CharacterManager::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1)
		{
			scene->CharacterSwap(PlayerType::HUNTER);
			ChangeButtonState(PlayerType::HUNTER);
		}
		else if (control->id == 2)
		{
			scene->CharacterSwap(PlayerType::WIZARD);
			ChangeButtonState(PlayerType::WIZARD);
		}
		else if (control->id == 3)
		{
			scene->CharacterSwap(PlayerType::THIEF);
			ChangeButtonState(PlayerType::THIEF);
		}
		else if (control->id == 4)
		{
			scene->CharacterSwap(PlayerType::WARRIOR);
			ChangeButtonState(PlayerType::WARRIOR);
		}
		else if (control->id == 5) scene->ChangeState(GameplayMenuState::NONE);
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