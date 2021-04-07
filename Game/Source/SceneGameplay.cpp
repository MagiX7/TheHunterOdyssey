#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "EntityManager.h"
#include "Player.h"
#include "Wizard.h"
#include "Warrior.h"
#include "Thief.h"
#include "Hunter.h"
#include "SceneGameplay.h"
#include "SceneBattle.h"
#include "CharacterManager.h"
#include "Npc.h"
#include "Map.h"
#include "DialogueManager.h"
#include "PauseMenu.h"
#include "Audio.h"

#include "Log.h"

#define NPC_RADIUS 10

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

	/*player1 = new Hunter();
	player2 = new Wizard();
	player3 = new Thief();
	player4 = new Warrior();*/

	Player* player = nullptr;
	player = (Player*)entityManager->CreateEntity(EntityType::HUNTER, position);
	currentPlayer = player;
	playerList.push_back(player);

	position = { 18,90 };
	player = (Player*)entityManager->CreateEntity(EntityType::WIZARD, position);
	playerList.push_back(player);

	position = { 12,45 };
	player = (Player*)entityManager->CreateEntity(EntityType::THIEF, position);
	playerList.push_back(player);

	position = { 13,56 };
	player = (Player*)entityManager->CreateEntity(EntityType::WARRIOR, position);
	playerList.push_back(player);

	Npc* generalNpc = nullptr;
	position = { 500,500 };
	generalNpc=(Npc*)entityManager->CreateEntity(EntityType::TABERN, position);

	position = { 200,200 };
	generalNpc = (Npc*)entityManager->CreateEntity(EntityType::TOWN, position);

	position = { 400,400 };
	generalNpc = (Npc*)entityManager->CreateEntity(EntityType::NPC_WIZARD, position);

	//Create Enemies

	Enemy* skull = nullptr;
	position = { 300,100 };
	skull = (Enemy*)entityManager->CreateEntity(EntityType::SKULL, position);

	Enemy* golem = nullptr;
	position = { 300,200 };
	golem = (Enemy*)entityManager->CreateEntity(EntityType::GOLEM, position);

	Enemy* bat = nullptr;
	position = { 300,300 };
	bat = (Enemy*)entityManager->CreateEntity(EntityType::BAT, position);

	/*player = new Player(PlayerType::HUNTER);
	npc = new Npc(EntityType::NPC);*/

	pause = new PauseMenu(this);

	showColliders = false;

	bg = nullptr;
}

Player* SceneGameplay::getCurrentPlayer()
{
	return currentPlayer;
}

bool SceneGameplay::Load()
{
	LOG("Loading Scene Gameplay");
	bool ret = true;

	font = new Font("Assets/Font/font3.xml", app->tex);

	entityManager->Load();

	// Startup
	menuState = GameplayMenuState::NONE;
	gameState = GameplayState::NONE;

	// Instantiate character swap manager
	charManager = new CharacterManager(currentPlayer, this);
	charManager->Load(font);

	// Start music
	app->audio->PlayMusic("Assets/Audio/Music/village_theme_1.ogg", 0);

	map = new Map();
	map->Load("town_map.tmx", app->tex);

	dialogueManager = new DialogueManager();
	dialogueManager->Start();

	pause->Load(font);

	return ret;
}

bool SceneGameplay::Update(float dt)
{
	bool ret = true;

	switch (gameState)
	{
	case GameplayState::NONE:
		gameState = GameplayState::ROAMING;
		break;
	case GameplayState::ROAMING:
		map->Update(dt);
		HandleInput(dt);
		
		if (dialogueManager->isDialogueActive == false)
		{
			currentPlayer->Update(dt);
			/*npc->Update(dt);*/
			entityManager->Update(dt);
			CheckDialogue();
		}
		else
		{
			dialogueManager->Update(dt);
		}

		break;
	case GameplayState::BATTLE:
		if (sceneBattle->Update(dt) == false)
		{
			sceneBattle->UnLoad();
			RELEASE(sceneBattle);
			gameState = GameplayState::ROAMING;
		}
		break;
	}

	switch (menuState)
	{
	case GameplayMenuState::CHARACTER_SWAP:
		charManager->Update(dt);
		break;
	case GameplayMenuState::PAUSE:
		ret = pause->Update(dt);
		break;
	}

	return ret;
}

void SceneGameplay::Draw()
{
	switch (gameState)
	{
	case GameplayState::ROAMING:
		map->Draw(showColliders);
		entityManager->Draw(showColliders);
		currentPlayer->Draw(true);
		if (dialogueManager->isDialogueActive)
			dialogueManager->Draw();
		if (menuState == GameplayMenuState::CHARACTER_SWAP) charManager->Draw(font, showColliders);
		if (menuState == GameplayMenuState::PAUSE) pause->Draw(font, showColliders);
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
		eastl::list<Player*>::iterator it = playerList.begin();
		for (; it != playerList.end(); ++it)
		{
			if ((*it)->playerType == player)
			{
				currentPlayer = (*it);
				break;
			}
		}
	}

	currentPlayer->bounds = tmpBounds;
}

bool SceneGameplay::CheckDialogue()
{
	bool ret = false;
	eastl::list<Npc*>::iterator it = entityManager->npcs.begin();
	while (it != entityManager->npcs.end())
	{
		ret = (*it)->CheckCollision(currentPlayer);
		if (ret) break;

		++it;
	}

	if (ret)
	{
		dialogueManager->current = dialogueManager->LoadDialogue((*it)->dialogeId);
		dialogueManager->isDialogueActive = true;
		dialogueManager->printText = true;
		(*it)->drawPtext = false;
		(*it)->talkStart = true;
	}
	return ret;
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

void SceneGameplay::HandleInput(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		// Instantiate and load scene battle
		sceneBattle = new SceneBattle(playerList, 3, this);
		sceneBattle->Load();

		gameState = GameplayState::BATTLE;
	}

	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) menuState = GameplayMenuState::CHARACTER_SWAP;
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		menuState = GameplayMenuState::PAUSE;
}