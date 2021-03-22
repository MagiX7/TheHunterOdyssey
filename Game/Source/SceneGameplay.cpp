#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"
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

	entityManager = new EntityManager();

	iPoint position = { 0,0 };

	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, position);
	entityManager->CreateEntity(EntityType::NPC, position);

	/*player = new Player(PlayerType::HUNTER);
	npc = new Npc(EntityType::NPC);*/

	showColliders = false;

	bg = nullptr;
}

bool SceneGameplay::Load()
{
	LOG("Loading Scene Gameplay");
	bool ret = true;

	entityManager->Load();

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

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}

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
		/*player->Update(dt);
		npc->Update(dt);*/
		entityManager->Update(dt);
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
		/*player->Draw(showColliders);
		npc->Draw(showColliders);*/
		entityManager->Draw(showColliders);
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

	/*RELEASE(player);
	RELEASE(npc);*/
	entityManager->UnLoad();
	return ret;
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{
	pugi::xml_node toLoadEntites = load.child("entities");
	
	entityManager->LoadState(&toLoadEntites);
	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{
	pugi::xml_node toSaveEntites = save.append_child("entities");

	entityManager->SaveState(&toSaveEntites);
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