#include "App.h"
#include "SwapPlayerScene.h"
#include "Player.h"

#include "GuiButton.h"

CharacterManager::CharacterManager(Player *pl): player(pl)
{
	swap = false;
	exit = false;
}

CharacterManager::~CharacterManager()
{
}

bool CharacterManager::Load()
{
	hunterButton = new GuiButton(1, { 466, 156, 360, 64 }, "SwapHunter", this);
	wizardButton = new GuiButton(2, { 466, 250, 360, 64 }, "SwapWizard", this);
	exitButton = new GuiButton(3, { 600, 350, 64, 64 }, "Exit", this);

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

Player* CharacterManager::CharacterSwap(PlayerType pType)
{
	swap = true;

	SDL_Rect tempBounds = player->bounds;
	player->UnLoad();
	RELEASE(player);

	switch (pType)
	{
	case PlayerType::HUNTER:
		player = new Player(PlayerType::HUNTER);
		break;
	case PlayerType::WIZARD:
		player = new Player(PlayerType::WIZARD);
		break;
	}

	if (player != nullptr)
	{
		player->Load();
		player->bounds = tempBounds;
	}

	return player;
}

bool CharacterManager::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) CharacterSwap(PlayerType::WIZARD);
		if (control->id == 2) CharacterSwap(PlayerType::HUNTER);
		if (control->id == 3) exit = true;
	}

	return true;
}

bool CharacterManager::GetSwap() const
{
	return swap;
}

bool CharacterManager::GetExit()
{
	return exit;
}

void CharacterManager::SetExit(bool setExit)
{
	exit = setExit;
}

Player* CharacterManager::GetPlayer()
{
	swap = false;

	return player;
}