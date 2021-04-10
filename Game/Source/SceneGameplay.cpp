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

	iPoint position = { 260,290 };

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
	pugi::xml_document animations;
	pugi::xml_node anims;
	pugi::xml_parse_result result = animations.load_file("animations.xml");

	if (result == NULL) 
		LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else 
		anims = animations.child("animations");

	Player* player = nullptr;
	player = new Hunter(position, anims);
	currentPlayer = player;
	playerList.push_back(player);

	position = { 18,90 };
	player = new Wizard(position, anims);
	playerList.push_back(player);

	position = { 12,45 };
	player = new Thief(position, anims);
	playerList.push_back(player);

	position = { 13,56 };
	player = new Warrior(position, anims);
	playerList.push_back(player);

	Npc* generalNpc = nullptr;
	/*position = { 500,500 };
	generalNpc=(Npc*)entityManager->CreateEntity(EntityType::TABERN, position,anims);*/

	position = { 200,250 };
	generalNpc = (Npc*)entityManager->CreateEntity(EntityType::TOWN, position, anims, 0);

	position = { 700,1060 };
	generalNpc = (Npc*)entityManager->CreateEntity(EntityType::RAY, position, anims, 3);
	//generalNpc->NpcMove(false);

	position = { 700,810 };
	generalNpc = (Npc*)entityManager->CreateEntity(EntityType::TABERN, position, anims, 2);

	position = { 500,350 };
	generalNpc = (Npc*)entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 1);

	//Create Enemies

	/*Enemy* skull = nullptr;
	position = { 300,100 };
	skull = (Enemy*)entityManager->CreateEntity(EntityType::SKULL, position);

	Enemy* golem = nullptr;
	position = { 300,200 };
	golem = (Enemy*)entityManager->CreateEntity(EntityType::GOLEM, position);

	Enemy* bat = nullptr;
	position = { 300,300 };
	bat = (Enemy*)entityManager->CreateEntity(EntityType::BAT, position);*/

	/*player = new Player(PlayerType::HUNTER);
	npc = new Npc(EntityType::NPC);*/

	pause = new PauseMenu(this);

	showColliders = false;
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
		switch (menuState)
		{
		case GameplayMenuState::NONE:
			map->Update(dt);
			if (dialogueManager->isDialogueActive == false)
			{
				HandleInput(dt);
				SDL_Rect tmpBounds = currentPlayer->bounds;
				currentPlayer->Update(dt);
				if (CollisionMapEntity(currentPlayer->bounds,currentPlayer->type) == true) currentPlayer->bounds = tmpBounds;
				CameraFollow(app->render);
				/*npc->Update(dt);*/
				entityManager->Update(dt);
				entityManager->CheckEntityColision(this);
				CheckDialogue();
			}
			else
			{
				dialogueManager->Update(dt);
			}
			break;
		case GameplayMenuState::CHARACTER_SWAP:
			charManager->Update(dt);
			break;
		case GameplayMenuState::PAUSE:
			ret = pause->Update(dt);
			break;
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

	return ret;
}

void SceneGameplay::Draw()
{
	switch (gameState)
	{
	case GameplayState::ROAMING:
		map->Draw(showColliders);
		entityManager->Draw(showColliders);
		currentPlayer->Draw(showColliders);
		if (dialogueManager->isDialogueActive)
		{
			app->render->DrawRectangle({ -(app->render->camera.x),-(app->render->camera.y),1280, 720 }, 0, 0, 0, 150);
			dialogueManager->Draw();
		}
		if (menuState == GameplayMenuState::CHARACTER_SWAP)
		{
			app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y, 1280, 720 }, 0, 0, 0, 150);
			charManager->Draw(font, showColliders);
		}
		if (menuState == GameplayMenuState::PAUSE)
		{
			app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y, 1280, 720 }, 0, 0, 0, 150);
			pause->Draw(font, showColliders);
		}
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
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->pads->start) menuState = GameplayMenuState::PAUSE;
}

bool SceneGameplay::CollisionMapEntity(SDL_Rect rect, EntityType type)
{
	iPoint pos = map->WorldToMap(rect.x, rect.y);
	if (rect.x > map->data.width* map->data.tileWidth)return true;
	else if (rect.x < 0)return true;
	else if (rect.y > map->data.tileWidth* map->data.height)return true;
	else if (rect.y < 0)return true;
	int x = pos.x + (rect.w / map->data.tileWidth);
	int y = pos.y + (rect.h / map->data.tileHeight);
	if (rect.w < map->data.tileWidth) x = pos.x + 1;
	if (rect.h < map->data.tileHeight) y = pos.y + 1;

	// Only check adyacent tiles
	for (int j = pos.y; j <= y; j++)
	{
		for (int i = pos.x; i <= x; i++)
		{
			if (i >= 0 && j >= 0)
			{
				if (((*map->data.layers.end().prev())->Get(i, j) == 769) && CheckCollision(map->GetTilemapRec(i, j), rect) ||
					((*map->data.layers.end().prev())->Get(i, j) == 1537) && CheckCollision(map->GetTilemapRec(i, j), rect) ||
					((*map->data.layers.end().prev())->Get(i, j) == 1) && CheckCollision(map->GetTilemapRec(i, j), rect))
				{
					return true;
				}
				if (type != EntityType::TOWN && type != EntityType::TABERN && type != EntityType::NPC_WIZARD) {
					if (((*map->data.layers.end().prev())->Get(i, j) == 771) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 625,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("house1.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 771) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 97,505 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 772) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 625,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("house2.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 772) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 70,765 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 773) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 625,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("house3.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 773) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 380,1120 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 774) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 625,430 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("cave_house.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 6) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 97,1100 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 775) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 655,430 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("pub.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 7) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 755,710 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 776) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 630,450 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("adventurer_house.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 1544) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 385,600 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 777) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 760,300 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("adventurer_house.tmx", app->tex);
					}
					if (((*map->data.layers.end().prev())->Get(i, j) == 1545) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						iPoint position = { 385,600 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
					}
				}
			}
		}
	}

	return false;
}

bool SceneGameplay::CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;

	else return false;
}

void SceneGameplay::CameraFollow(Render* render)
{
	int midPlayerPosX = (currentPlayer->bounds.w / 2) + currentPlayer->bounds.x;
	int midPlayerPosY = (currentPlayer->bounds.h / 2) + currentPlayer->bounds.y;
	if (midPlayerPosX > -render->camera.x + (render->camera.w / 2) || midPlayerPosX < -render->camera.x + (render->camera.w / 2))
	{
		render->camera.x = -(midPlayerPosX - (render->camera.w / 2));
	}

	if (midPlayerPosY > -render->camera.y + (render->camera.h / 2) || midPlayerPosY < -render->camera.y + (render->camera.h / 2))
	{
		render->camera.y = -(midPlayerPosY - (render->camera.h / 2));
	}

	int mapWidth = map->data.width * map->data.tileWidth;
	int mapHeight = map->data.height * map->data.tileHeight;
	if (-render->camera.x + render->camera.w >= mapWidth) render->camera.x = -(mapWidth - render->camera.w);
	if (-render->camera.x <= 0) render->camera.x = 0;

	if (-render->camera.y + render->camera.h >= mapHeight) render->camera.y = -(mapHeight - render->camera.h);
	if (-render->camera.y <= 0) render->camera.y = 0;
}