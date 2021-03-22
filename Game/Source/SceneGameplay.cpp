#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Player.h"
#include "SceneGameplay.h"
#include "SceneBattle.h"
#include "CharacterManager.h"
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
	menuState = GameplayMenuState::NONE;
	gameState = GameplayState::NONE;


	// Instantiate character swap manager
	charManager = new CharacterManager(player, this);
	charManager->Load();

	// Instantiate and load scene battle
	sceneBattle = new SceneBattle(player, player);
	sceneBattle->Load();


	return ret;
}

bool SceneGameplay::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		menuState = GameplayMenuState::CHARACTER_SWAP;
	}

	switch (gameState)
	{
	case GameplayState::NONE:
		gameState = GameplayState::ROAMING;
		break;
	case GameplayState::ROAMING:
		player->Update(dt);
		npc->Update(dt);
		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) gameState = GameplayState::BATTLE;
		break;
	case GameplayState::BATTLE:
		sceneBattle->Update(dt);
		break;
	}

	switch (menuState)
	{
	case GameplayMenuState::CHARACTER_SWAP:
		charManager->Update(dt);
		break;
	}

	return ret;
}

void SceneGameplay::Draw()
{
	if (menuState == GameplayMenuState::CHARACTER_SWAP)
	{
		charManager->Draw(showColliders);
	}

	switch (gameState)
	{
	case GameplayState::ROAMING:
		player->Draw(showColliders);
		npc->Draw(showColliders);
		break;
	case GameplayState::BATTLE:
		sceneBattle->Draw(showColliders);
		break;
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

void SceneGameplay::ChangeState(GameplayMenuState type)
{
	menuState = type;
}

void SceneGameplay::SetPlayer(Player* pl)
{
	player = pl;
}