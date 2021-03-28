#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Player.h"
#include "Wizard.h"
#include "Hunter.h"
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

	/*player1 = (Player*)entityManager->CreateEntity(EntityType::PLAYER, position);
	player1->SetPlayerType(PlayerType::HUNTER);
	player1->Load();

	player2 = (Player*)entityManager->CreateEntity(EntityType::PLAYER, position);
	player2->SetPlayerType(PlayerType::WIZARD);
	player2->Load();*/

	player1 = new Hunter();
	player2 = new Wizard();

	currentPlayer = player1;

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
	charManager = new CharacterManager(player1, this);
	charManager->Load();

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
		currentPlayer->Update(dt);
		/*npc->Update(dt);*/
		//entityManager->Update(dt);
		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			// Instantiate and load scene battle
			sceneBattle = new SceneBattle(player1, player2);
			sceneBattle->Load();

			gameState = GameplayState::BATTLE;
		}
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
	map->Draw();

	if (menuState == GameplayMenuState::CHARACTER_SWAP)
	{
		charManager->Draw(showColliders);
	}

	switch (gameState)
	{
	case GameplayState::ROAMING:
		/*player->Draw(showColliders);
		npc->Draw(showColliders);*/
		//entityManager->Draw(showColliders);
		currentPlayer->Draw(showColliders);
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
	entityManager->DeleteAllEntities();
	
	return ret;
}

void SceneGameplay::CharacterSwap(PlayerType player)
{
	SDL_Rect tmpBounds = currentPlayer->bounds;
	if (player != currentPlayer->playerType)
	{
		switch (player)
		{
		case PlayerType::HUNTER:
			currentPlayer = player1;
			break;
		case PlayerType::WIZARD:
			currentPlayer = player2;
			break;
		}
	}

	currentPlayer->bounds = tmpBounds;
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{
	pugi::xml_node toLoadEntities = load.child("entities");
	
	entityManager->LoadState(&toLoadEntities);
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
	player1 = pl;
}