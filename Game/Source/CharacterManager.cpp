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

bool CharacterManager::Load()
{
	hunterButton = new GuiButton(1, { 466, 156, 360, 64 }, "SwapHunter", this);
	wizardButton = new GuiButton(2, { 466, 250, 360, 64 }, "SwapWizard", this);
	exitButton = new GuiButton(3, { 600, 350, 64, 64 }, "Exit", this);

	switch (player->playerType)
	{
	case PlayerType::HUNTER:
		hunterButton->state = GuiControlState::DISABLED;
		break;
	case PlayerType::WIZARD:
		wizardButton->state = GuiControlState::DISABLED;
		break;
	}

	return true;
}

bool CharacterManager::Update(float dt)
{
	hunterButton->Update(app->input, dt);
	wizardButton->Update(app->input, dt);
	exitButton->Update(app->input, dt);

	return true;
}

void CharacterManager::Draw(bool showColliders)
{
	hunterButton->Draw(app->render, showColliders);
	wizardButton->Draw(app->render, showColliders);
	exitButton->Draw(app->render, showColliders);
}

bool CharacterManager::UnLoad()
{
	RELEASE(hunterButton);
	RELEASE(wizardButton);
	RELEASE(exitButton);

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
		else if (control->id == 3) scene->ChangeState(GameplayMenuState::NONE);
	}

	return true;
}

void CharacterManager::ChangeButtonState(PlayerType type)
{
	switch (type)
	{
	case PlayerType::HUNTER:
		hunterButton->state = GuiControlState::DISABLED;
		wizardButton->state = GuiControlState::NORMAL;
		break;
	case PlayerType::WIZARD:
		hunterButton->state = GuiControlState::NORMAL;
		wizardButton->state = GuiControlState::DISABLED;
		break;
	}
}