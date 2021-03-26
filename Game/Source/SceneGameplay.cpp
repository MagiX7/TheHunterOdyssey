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
#include "Map.h"

#include "Audio.h"

#include "Log.h"

SceneGameplay::SceneGameplay()
{
	name.Create("scenegameplay");

	entityManager = new EntityManager();

	iPoint position = { 0,0 };

	player = (Player*)entityManager->CreateEntity(EntityType::PLAYER, position);
	player->setPlayerType(PlayerType::HUNTER);

	Npc* generalNpc = nullptr;
	generalNpc=(Npc*)entityManager->CreateEntity(EntityType::NPC, position);
	generalNpc->setNpcType(NpcType::TABERN);
	position = { 12,45 };
	generalNpc = (Npc*)entityManager->CreateEntity(EntityType::NPC, position);
	generalNpc->setNpcType(NpcType::TOWN);

	position = { 24,56 };
	generalNpc = (Npc*)entityManager->CreateEntity(EntityType::NPC, position);
	generalNpc->setNpcType(NpcType::WIZARD);
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


	// Start music
	app->audio->PlayMusic("Assets/Audio/Music/village_theme_1.ogg");

	textures = new Textures();

	textures->Load("Assets/Textures/Maps/map_tileset.png");
	textures->Load("Assets/Textures/Maps/hitboxes.png");

	map = new Map();

	map->Load("town_map.tmx", textures);

	return ret;
}

bool SceneGameplay::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}

	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		app->LoadGameRequest();
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

	map->Update(dt);

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

	map->Draw();

}

bool SceneGameplay::UnLoad()
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	/*RELEASE(player);
	RELEASE(npc);*/
	entityManager->UnLoad();
	entityManager->DeleteAllEntities();
	
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