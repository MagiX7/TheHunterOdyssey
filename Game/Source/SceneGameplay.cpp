#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Player.h"
#include "SceneGameplay.h"
#include "CharacterManager.h"
#include "GuiButton.h"
#include "Npc.h"

#include "Audio.h"

#include "Log.h"

SceneGameplay::SceneGameplay()
{
	name.Create("scenegameplay");

	player = new Player(PlayerType::HUNTER);
	npc = new Npc(EntityType::NPC);

	showColliders = false;

	bg = nullptr;
}

bool SceneGameplay::Load()
{
	LOG("Loading Scene Gameplay");
	bool ret = true;

	player->Load();
	npc->Load();

	// Startup
	state = SwapCharState::NONE;

	// Instantiate character swap manager
	charManager = new CharacterManager(player, this);
	charManager->Load();

	return ret;
}

bool SceneGameplay::Update(float dt)
{
	bool ret = true;

	player->Update(dt);
	npc->Update(dt);

	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		state = SwapCharState::NORMAL;
		
	}

	switch (state)
	{
	case SwapCharState::NORMAL:
		charManager->Update(dt);
		break;
	}

	return ret;
}

void SceneGameplay::Draw()
{
	player->Draw(showColliders);
	npc->Draw(showColliders);

	if (state == SwapCharState::NORMAL)
	{
		charManager->Draw(showColliders);
	}
}

bool SceneGameplay::UnLoad()
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	RELEASE(player);
	RELEASE(npc);

	return ret;
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{

	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{

	return true;
}

bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1);
		if (control->id == 2);
	}

	return true;
}

void SceneGameplay::ChangeState(SwapCharState type)
{
	state = type;
}

void SceneGameplay::SetPlayer(Player* pl)
{
	player = pl;
}