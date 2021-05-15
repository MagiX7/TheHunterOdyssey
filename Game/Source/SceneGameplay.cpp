#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Font.h"

#include "SceneGameplay.h"
#include "SceneBattle.h"

#include "EntityManager.h"
#include "Hunter.h"
#include "Wizard.h"
#include "Thief.h"
#include "Warrior.h"

#include "Golem.h"
#include "Bat.h"
#include "Skull.h"

#include "FairyTear.h"
#include "OmniPotion.h"
#include "UltraPotion.h"
#include "Potion.h"
#include "Orb.h"
#include "OrbFragment.h"
#include "InterruptorBlock.h"

#include "KnightHelmet.h"
#include "KnightChest.h"

#include "Npc.h"
#include "Map.h"
#include "DialogueManager.h"

#include "IceBlock.h"
#include "Door.h"
#include "Statue.h"
#include "QuestManager.h"

#include "CharacterManager.h"
#include "PauseMenu.h"
#include "Inventory.h"

#include "Log.h"

#include "Optick/include/optick.h"

#define NPC_RADIUS 10

SceneGameplay::SceneGameplay()
{
	name.Create("scenegameplay");

	entityManager = new EntityManager();
	particles = new ParticlesManager();

	iPoint position = { 260,290 };

	pugi::xml_document animations;
	pugi::xml_node anims;
	pugi::xml_parse_result result = animations.load_file("animations.xml");

	if (result == NULL) 
		LOG("Could not load xml file: %s. pugi error: %s", "animations.xml", result.description());
	else 
		anims = animations.child("animations");

	Player* player = nullptr;
	player = new Hunter(position, anims, particles);
	currentPlayer = player;
	playerList.push_back(player);
	position = { 18,90 };
	player = new Wizard(position, anims, particles);
	playerList.push_back(player);

	position = { 12,45 };
	player = new Thief(position, anims, particles);
	playerList.push_back(player);

	position = { 13,56 };
	player = new Warrior(position, anims, particles);
	playerList.push_back(player);

	tmpPosPlayer = { 0,0 };

	Enemy* en;
	en = new Golem({ 865, 365 }, anims);
	en->SetCurrentState(EnemyState::ROAMING);
	enemyList.push_back(en);

	en = new Skull({ 260, 868 }, anims);
	en->SetCurrentState(EnemyState::ROAMING);
	enemyList.push_back(en);

	en = new Bat({ 1087, 850 }, anims);
	en->SetCurrentState(EnemyState::ROAMING);
	enemyList.push_back(en);

	atlas = app->tex->Load("Textures/Items/items_atlas.png");
	inventory = new Inventory(playerList, atlas);

	Item *item = new UltraPotion(iPoint(128,1248), atlas);
	items.push_back(item);
	
	item = new OmniPotion(iPoint(768,1248), atlas);
	items.push_back(item);

	item = new Potion(iPoint(1280, 80), atlas);
	items.push_back(item);

	item = new FairyTear(iPoint(512, 800), atlas);
	items.push_back(item);

	item = new KnightHelmet({ 1376, 1056, 32, 32 }, iPoint(1376, 1056),atlas);
	items.push_back(item);

	item = new KnightChest({1408, 1056, 32, 32}, iPoint(1408, 1056), atlas);
	items.push_back(item);

	pause = new PauseMenu(this);

	font = new Font(app, "Font/font3.xml", app->tex);

	showColliders = false;
	transition = false;
	fadeOut = false;
	alpha = 0.0f;
	isDungeon = false;
	loadObjects = true;
	deleteDoor = true;
}

bool SceneGameplay::Load()
{
	LOG("Loading Scene Gameplay");
	bool ret = true;
	//particles->CleanUp();
	font = new Font(app, "Font/font3.xml", app->tex);

	goldTexture = app->tex->Load("Textures/UI/gold.png");

	entityManager->Load();

	eastl::list<Player*>::iterator it = playerList.begin();
	eastl::list<Player*>::iterator itEnd = playerList.end();
	for (; it != itEnd; ++it)
		(*it)->Load();

	// Startup
	menuState = GameplayMenuState::NONE;
	gameState = GameplayState::ROAMING;

	// Instantiate character swap manager
	charManager = new CharacterManager(this, currentPlayer->playerType, font);
	charManager->Load(font);

	// Start music
	channel = app->audio->SetChannel();

	app->audio->PlayMusic("Audio/Music/village_theme_1.ogg", 0);

	//Load Fx
	doorOpenedFx = app->audio->LoadFx("Audio/Fx/Gameplay/door_open.ogg");
	doorClosedFx = app->audio->LoadFx("Audio/Fx/Gameplay/door_close.ogg");
	puzzleCompletedFx = app->audio->LoadFx("Audio/Fx/Gameplay/puzzle_completed.ogg");

	map = new Map();
	isTown = true;
	map->Load("town_map.tmx", app->tex);
	
	LoadNpc(map->name);

	dialogueManager = new DialogueManager();
	dialogueManager->Start();

	pause->Load(font);

	inventory->Load(font);

	eastl::list<Item*>::iterator item = items.begin();
	for (; item != items.end(); ++item)
	{
		(*item)->Load();
	}

	QuestManager::GetInstance()->SetPlayer(currentPlayer);

	SDL_ShowCursor(SDL_ENABLE);
	//particles->StartSimulation(currentPlayer->generator);
	
	sceneBattle = nullptr;

	return ret;
}

bool SceneGameplay::Update(float dt)
{
	OPTICK_EVENT("Scene Gameplay Update");

	bool ret = true;
	switch (gameState)
	{
	case GameplayState::ROAMING:
		switch (menuState)
		{
		case GameplayMenuState::NONE:
			if(SDL_ShowCursor(-1) == SDL_DISABLE)
				SDL_ShowCursor(SDL_ENABLE);

			if (dialogueManager->isDialogueActive == false)
			{
				if (!transition) 
					QuestManager::GetInstance()->Update(app->input, dt);
				
				if (QuestManager::GetInstance()->QuestState() == false)
				{
					particles->PreUpdate();
					map->Update(dt);
					HandleInput(dt);
					SDL_Rect tmpBounds = currentPlayer->bounds;
					currentPlayer->Update(dt);

					particles->Update(dt);

					if (isTown)
					{
						eastl::list<Enemy*>::iterator enemies = enemyList.begin();
						eastl::list<Enemy*>::iterator enemyEnd = enemyList.end();
						for (; enemies != enemyEnd; ++enemies)
						{
							Enemy* enemy = (*enemies);
							enemy->Update(dt);
							if (!showColliders && CheckCollision(enemy->bounds, currentPlayer->bounds))
							{
								GenerateBattle();
								tmp = enemy;
								break;
							}
						}
					}
					SDL_Rect bounds = { currentPlayer->bounds.x,currentPlayer->bounds.y + currentPlayer->bounds.h - 7,currentPlayer->bounds.w,7 };
					if (showColliders == false && CollisionMapEntity(bounds, currentPlayer->type) == true)
						currentPlayer->bounds = tmpBounds;

					//PUZZLE
					if (isDungeon)
					{
						eastl::list<Entity*>::iterator entity = entityManager->entities.begin();
						eastl::list<Entity*>::iterator entityEnd = entityManager->entities.end();

						for (; entity != entityEnd; ++entity)
						{
							if (((*entity)->type) == EntityType::ICE_BLOCK)
							{
								bounds = (*entity)->bounds;
								break;
							}
						}

						IceBlock* tmpIceBlock = (IceBlock*)(*entity);
						if ((*entity) != nullptr)
						{
							if (*entity != nullptr && CollisionMapEntity(bounds, EntityType::ICE_BLOCK) == true)
							{
								bounds = iceBlockRect;

								if (tmpIceBlock->tmpState == EntityState::WALKING_RIGHT)
								{
									(*entity)->SetBounds({ tmpIceBlock->bounds.x - 1,  tmpIceBlock->bounds.y,  tmpIceBlock->bounds.w,  tmpIceBlock->bounds.h });
									(*entity)->SetState(EntityState::STOP_DOWN);
								}
								if (tmpIceBlock->tmpState == EntityState::WALLKING_LEFT)
								{
									(*entity)->SetBounds({ tmpIceBlock->bounds.x + 1,  tmpIceBlock->bounds.y,  tmpIceBlock->bounds.w,  tmpIceBlock->bounds.h });
									(*entity)->SetState(EntityState::STOP_DOWN);
								}
								if (tmpIceBlock->tmpState == EntityState::WALLKING_UP)
								{
									(*entity)->SetBounds({ tmpIceBlock->bounds.x,  tmpIceBlock->bounds.y + 1,  tmpIceBlock->bounds.w,  tmpIceBlock->bounds.h });
									(*entity)->SetState(EntityState::STOP_DOWN);
								}
								if (tmpIceBlock->tmpState == EntityState::WALLKING_DOWN)
								{
									(*entity)->SetBounds({ tmpIceBlock->bounds.x,  tmpIceBlock->bounds.y - 1,  tmpIceBlock->bounds.w,  tmpIceBlock->bounds.h });
									(*entity)->SetState(EntityState::STOP_DOWN);
								}
							}
							if (*entity != nullptr && CollisionMapEntity(bounds, EntityType::ICE_BLOCK) == false)
							{
								iceBlockRect = bounds;
							}
						}
					}

					CameraFollow(app->render);

					int tmpId = -1;
					entityManager->Update(dt, currentPlayer, dialogueManager->isDialogueActive, tmpId, this);
					if (tmpId != -1)
					{
						dialogueManager->LoadDialogue(tmpId);
						dialogueManager->printText = true;
					}

					eastl::list<Item*>::iterator it = items.begin();
					for (; it != items.end(); ++it)
					{
						(*it)->Update(dt);
						if (CheckCollision(currentPlayer->bounds, (*it)->bounds) && (*it)->isDropped == false)
						{
							QuestManager::GetInstance()->CheckQuests(*it);
							inventory->AddItem(*it);
							items.erase(it);

							if (inventory->ObjectQuantity(ItemType::ORB) == 1)
							{
								entityManager->DeleteEntity(EntityType::STATUE, 1);
								entityManager->DeleteEntity(EntityType::STATUE, 2);
								entityManager->DeleteEntity(EntityType::DOOR, 3);
							}
						}
					}
				}
			}
			else
			{
				dialogueManager->Update(dt);
			}
			break;
		case GameplayMenuState::CHARACTER_SWAP:
			charManager->Update(dt);
			break;

		case GameplayMenuState::INVENTORY:
			inventory->Update(dt);
			HandleInput(dt);
			break;

		case GameplayMenuState::PAUSE:
			ret = pause->Update(dt);
			break;
		}
		break;
	case GameplayState::BATTLE:
		if (sceneBattle->Update(dt) == false)
		{
			transition = true;
			fadeOut = true;
		}
	break;
	}

	if (transition) Transitioning(dt);

	return ret;
}

void SceneGameplay::Draw()
{
	eastl::list<Enemy*>::iterator enemies = enemyList.begin();
	eastl::list<Enemy*>::iterator enemyEnd = enemyList.end();
	switch (gameState)
	{
	case GameplayState::ROAMING:
		map->Draw(showColliders,entityManager,currentPlayer);
		//entityManager->Draw(showColliders);
		particles->PostUpdate();
		//currentPlayer->Draw(showColliders);
		
		app->render->DrawTexture(goldTexture, 5, 60, NULL, false);

		char tmp[32];
		sprintf_s(tmp, "%i", currentPlayer->gold);
		app->render->DrawText(font, tmp, 80, 75, 50, 10, SDL_Color({ 0,0,0,255 }));
		app->render->DrawText(font, tmp, 77, 72, 50, 10, SDL_Color({ 255,255,255,255 }));

		{
			eastl::list<Item*>::iterator it = items.begin();
			for (; it != items.end(); ++it)
			{
				(*it)->Draw(showColliders);
			}
		}	

		if (isTown)
		{
			for (; enemies != enemyEnd; ++enemies)
			{
				(*enemies)->Draw(showColliders);
			}
		}

		if (dialogueManager->isDialogueActive)
		{
			app->render->DrawRectangle({ 0, 0,1280, 720 }, 0, 0, 0, 150, true, false);
			dialogueManager->Draw();
		}
		else
		{
			if (!transition) QuestManager::GetInstance()->Draw(app->render, font);
		}

		switch (menuState)
		{
		case GameplayMenuState::CHARACTER_SWAP:
			app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 150, true, false);
			charManager->Draw(font, showColliders);
			break;

		case GameplayMenuState::PAUSE:
			pause->Draw(font, showColliders);
			break;

		case GameplayMenuState::INVENTORY:
			inventory->Draw(font, showColliders);
			break;
		}
		break;
	case GameplayState::BATTLE:
		if (sceneBattle != nullptr) sceneBattle->Draw(showColliders);
		break;
	}

	if (transition) TransitionsManager::GetInstance()->Draw();
}

bool SceneGameplay::UnLoad()
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	entityManager->UnLoad();
	RELEASE(entityManager);

	QuestManager::GetInstance()->UnLoad();

	map->CleanUp();
	RELEASE(map);
	
	eastl::list<Player*>::iterator it = playerList.begin();
	eastl::list<Player*>::iterator itEnd = playerList.begin();
	for (; it != itEnd; ++it)
	{
		(*it)->UnLoad();
		RELEASE((*it));
		playerList.erase(it);
	}
	playerList.clear();

	charManager->UnLoad();
	RELEASE(charManager);

	pause->UnLoad();
	RELEASE(pause);

	inventory->UnLoad();
	RELEASE(inventory);
	
	dialogueManager->UnLoad();
	RELEASE(dialogueManager);

	font->UnLoad(app->tex);
	RELEASE(font);
	
	particles->CleanUp();

	/*app->audio->UnLoadFx(doorClosedFx);
	app->audio->UnLoadFx(doorOpenedFx);*/
	app->audio->UnLoadFxs();
	app->tex->UnLoad(goldTexture);

	return ret;
}

void SceneGameplay::CharacterSwap(PlayerType player)
{
	SDL_Rect tmpBounds = currentPlayer->bounds;
	if (player != currentPlayer->playerType)
	{
		eastl::list<Player*>::iterator it = playerList.begin();
		eastl::list<Player*>::iterator itEnd = playerList.end();
		for (; it != itEnd; ++it)
		{
			Player* pl = (*it);
			if (pl->playerType == player)
			{
				currentPlayer = pl;
				break;
			}
		}
	}
	currentPlayer->bounds = tmpBounds;
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{
	// If the player is not in the map he saved at, set the current map to that one
	SString mapName = load.attribute("map_name").as_string();
	if (mapName != map->name)
	{
		map->CleanUp();
		map->Load(mapName.GetString(), app->tex);
	}

	pugi::xml_node toLoadEntities = load.child("entities");
	pugi::xml_document animations;
	pugi::xml_node anims;
	pugi::xml_parse_result result = animations.load_file("animations.xml");
	anims = animations.child("animations");

	entityManager->LoadState(&toLoadEntities, &anims);

	eastl::list<Player*>::iterator item = playerList.begin();
	eastl::list<Player*>::iterator itEnd = playerList.end();
	for (item; item != itEnd; ++item)
	{
		(*item)->UnLoad();
		RELEASE((*item));
		playerList.erase(item);
	}

	int playerAmount = toLoadEntities.child("players").attribute("amount").as_int();
	pugi::xml_node NodePlayer = toLoadEntities.child("players");
	pugi::xml_node NodePlayerAuxiliar = NodePlayer.child("player");
	bool iscurrent = false;
	for (int i = 0; i < playerAmount; ++i) 
	{
		iscurrent = false;

		SString string;
		SString string1;
		string = NodePlayerAuxiliar.child("playerType").attribute("type").as_string();
		string1 = NodePlayerAuxiliar.child("isCurrent").attribute("current").as_string();
		if (string1 == "true") iscurrent = true;

		EntityType plType;
		Player* player = nullptr;
		if (string == "HUNTER") 
			player = new Hunter({ NodePlayerAuxiliar.child("bounds").attribute("X").as_int(), NodePlayerAuxiliar.child("bounds").attribute("Y").as_int() }, anims,particles);
		else if (string == "WIZARD") 
			player = new Wizard({ NodePlayerAuxiliar.child("bounds").attribute("X").as_int(), NodePlayerAuxiliar.child("bounds").attribute("Y").as_int() }, anims, particles);
		else if (string == "WARRIOR") 
			player = new Warrior({ NodePlayerAuxiliar.child("bounds").attribute("X").as_int(), NodePlayerAuxiliar.child("bounds").attribute("Y").as_int() }, anims, particles);
		else if (string == "THIEF") 
			player = new Thief({ NodePlayerAuxiliar.child("bounds").attribute("X").as_int(), NodePlayerAuxiliar.child("bounds").attribute("Y").as_int() }, anims, particles);
		else 
			plType = EntityType::UNKNOWN;
		
		if (iscurrent == true) currentPlayer = player;
		playerList.push_back(player);
		player->Load();
		NodePlayerAuxiliar = NodePlayerAuxiliar.next_sibling();
	}
	
	eastl::list<Enemy*>::iterator enemies = enemyList.begin();
	eastl::list<Enemy*>::iterator enemyEnd = enemyList.end();

	for (; enemies != enemyEnd; ++enemies)
	{
		(*enemies)->UnLoad();
		RELEASE(*enemies);
		enemyList.remove((*enemies));
	}
	enemyList.clear();
	int enemyAmount = toLoadEntities.child("Enemies").attribute("amount").as_int();
	pugi::xml_node NodeEnemy = toLoadEntities.child("Enemies");
	pugi::xml_node NodeEnemyAuxiliar = NodeEnemy.child("Enemy");
	for (int a = 0; a < enemyAmount; a++)
	{
		SString string;
		string = NodeEnemyAuxiliar.child("EnemyType").attribute("type").as_string();

		EntityType plType;
		Enemy* enemy = nullptr;
		if (string == "BAT")
			enemy = new Bat({ NodeEnemyAuxiliar.child("bounds").attribute("X").as_int(), NodeEnemyAuxiliar.child("bounds").attribute("Y").as_int() }, anims);
		else if (string == "GOLEM")
			enemy = new Golem({ NodeEnemyAuxiliar.child("bounds").attribute("X").as_int(), NodeEnemyAuxiliar.child("bounds").attribute("Y").as_int() }, anims);
		else if (string == "SKULL")
			enemy = new Skull({ NodeEnemyAuxiliar.child("bounds").attribute("X").as_int(), NodeEnemyAuxiliar.child("bounds").attribute("Y").as_int() }, anims);
		enemy->SetCurrentState(EnemyState::ROAMING);
		enemyList.push_back(enemy);
		NodeEnemyAuxiliar = NodeEnemyAuxiliar.next_sibling();
	}

	QuestManager::GetInstance()->LoadQuests(load);

	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{
	//if (isTown == false) return true;

	save.append_attribute("map_name").set_value(map->name.GetString());
	
	pugi::xml_node toSaveEntites = save.append_child("entities");
	entityManager->SaveState(&toSaveEntites);

	pugi::xml_node toSaveEnemies = toSaveEntites.append_child("Enemies");
	pugi::xml_node enemiesAuxiliar;
	toSaveEnemies.append_attribute("amount").set_value(enemyList.size());
	eastl::list<Enemy*>::iterator enemies = enemyList.begin().mpNode;
	eastl::list<Enemy*>::iterator enemyEnd = enemyList.end().mpNode;

	for (; enemies != enemyEnd; ++enemies)
	{
		enemiesAuxiliar = toSaveEnemies.append_child("Enemy");
		(*enemies)->SaveState(enemiesAuxiliar);
	}

	pugi::xml_node nodePlayers = toSaveEntites.append_child("players");
	pugi::xml_node nodePlayersAuxiliar;
	int playerAmount = 0;
	nodePlayers.append_attribute("amount").set_value(playerList.size());
	nodePlayersAuxiliar = nodePlayers.append_child("player");
	eastl::list<Player*>::iterator aux;
	eastl::list<Player*>::iterator auxEnd;
	aux = playerList.begin().mpNode;
	auxEnd = playerList.end().mpNode;
	for (aux; aux != auxEnd; ++aux)
	{
		Player* pl = (*aux);
		if (pl == currentPlayer)
		{
			nodePlayersAuxiliar.append_child("isCurrent").append_attribute("current").set_value("true");
		}
		else
		{
			nodePlayersAuxiliar.append_child("isCurrent").append_attribute("current").set_value("false");
		}
		pl->SaveState(nodePlayersAuxiliar);
		nodePlayersAuxiliar = nodePlayers.append_child("player");
	}

	QuestManager::GetInstance()->SaveQuests(save);

	return true;
}

void SceneGameplay::ChangeState(GameplayMenuState type)
{
	menuState = type;
}

void SceneGameplay::HandleInput(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
	{
		// Instantiate and load scene battle
		GenerateBattle();
	}

	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN) menuState = GameplayMenuState::CHARACTER_SWAP;
	
	if (menuState != GameplayMenuState::INVENTORY && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN) menuState = GameplayMenuState::PAUSE;

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		if (menuState == GameplayMenuState::INVENTORY)
		{
			menuState = GameplayMenuState::NONE;
		}
		else
		{
			menuState = GameplayMenuState::INVENTORY;
			inventory->GetEquipment(currentPlayer);
		}
	}
}

bool SceneGameplay::CollisionMapEntity(SDL_Rect rect, EntityType type)
{
	iPoint pos = map->WorldToMap(rect.x, rect.y);

	if (rect.x + rect.w > map->data.width * map->data.tileWidth) return true;
	else if (rect.x < 0) return true;
	else if (rect.y > map->data.tileWidth * map->data.height) return true;
	else if (rect.y < 0) return true;

	int x = pos.x + (rect.w / map->data.tileWidth);
	int y = pos.y + (rect.h / map->data.tileHeight);

	if (rect.w < map->data.tileWidth) x = pos.x + 1;
	if (rect.h < map->data.tileHeight) y = pos.y + 1;

	bool exit = false;
	MapLayer* layer = (*map->data.layers.end().prev());

	// Only check adyacent tiles
	for (int j = pos.y; j <= y; ++j)
	{
		for (int i = pos.x; i <= x; ++i)
		{
			if (i >= 0 && j >= 0)
			{
				if ((layer->Get(i, j) == 769) && CheckCollision(map->GetTilemapRec(i, j), rect) ||
					(layer->Get(i, j) == 1537) && CheckCollision(map->GetTilemapRec(i, j), rect) ||
					(layer->Get(i, j) == 1) && CheckCollision(map->GetTilemapRec(i, j), rect) ||
					(layer->Get(i, j) == 3073) && CheckCollision(map->GetTilemapRec(i, j), rect))
				{
					return true;
				}
				if (type != EntityType::TOWN && type != EntityType::TABERN && type != EntityType::NPC_WIZARD)
				{
					if (isTown && (layer->Get(i, j) == 771) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();

						iPoint position = { 625,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("house1.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 771) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 97,520 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 772) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 625,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("house2.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 772) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 70,765 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 773) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 625,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("house3.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 773) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 380,1120 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 774) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 625,430 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("cave_house.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 6) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 97,1100 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 775) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position;

						position = { 655,430 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("pub.tmx", app->tex);
						isDungeon = false;
						particles->SetAllParticlesDesactivated();
						QuestManager::GetInstance()->CheckQuests(map->name);

						LoadNpc(map->name);
						exit = true;
						break;						
					}
					else if ((layer->Get(i, j) == 7) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 770,710 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 776) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 630,450 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("adventurer_house.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
					
						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 1544) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 385,610 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 778) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						//entityManager->SetAllNpcInactive();
						entityManager->DeleteAllNpcActive();
						iPoint position = { 650,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("inn.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 1546) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 380,136 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 779) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 615,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						particles->SetAllParticlesDesactivated();
						map->CleanUp();
						map->Load("library.tmx", app->tex);
						LoadNpc(map->name);

						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);

						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 1547) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 160,136 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 780) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 615,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("shop.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 12) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 898,152 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);

						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 782) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 615,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("shop2.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 14) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 900,1250 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);

						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if (isTown && (layer->Get(i, j) == 783) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorOpenedFx);
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 615,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("red_house.tmx", app->tex);
						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);

						exit = true;
						break;
					}
					else if ((layer->Get(i, j) == 1551) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						app->audio->PlayFx(channel, doorClosedFx);
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 1350,390 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);

						isDungeon = false;
						QuestManager::GetInstance()->CheckQuests(map->name);
						particles->SetAllParticlesDesactivated();
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 784) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 705,950 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("graveyard_path.tmx", app->tex);
						isDungeon = false;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 786) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = true;
						entityManager->DeleteAllNpcActive();
						//entityManager->SetAllNpcActive();
						iPoint position = { 705,30 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("town_map.tmx", app->tex);
						LoadNpc(map->name);
						isDungeon = false;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 785) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 620,480 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("bath_house.tmx", app->tex);
						isDungeon = false;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 3089) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = true;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 320,380 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("graveyard_path.tmx", app->tex);
						isDungeon = false;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 787) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 630,960 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("graveyard.tmx", app->tex);
						isDungeon = false;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 19) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = true;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 500,50 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("graveyard_path.tmx", app->tex);
						isDungeon = false;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 20) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = false;
						entityManager->DeleteAllNpcActive();
						iPoint position = { 1420,720 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("dungeon_map.tmx", app->tex);
						isDungeon = true;
						if (loadObjects)
						{
							IceBlock* iceBlock = nullptr;
							position = { 2048,320 };
							iceBlock = (IceBlock*)entityManager->CreateEntity2(EntityType::ICE_BLOCK, position, currentPlayer, 1);
							Door* door = nullptr;
							position = { 2240,992 };
							door = (Door*)entityManager->CreateEntity2(EntityType::DOOR, position, currentPlayer, 1);
							Door* door2 = nullptr;
							position = { 576,288 };
							door2 = (Door*)entityManager->CreateEntity2(EntityType::DOOR, position, currentPlayer, 2);
							Door* door3 = nullptr;
							position = { 1408,2496 };
							door3 = (Door*)entityManager->CreateEntity2(EntityType::DOOR, position, currentPlayer, 3);
							Door* door4 = nullptr;
							position = { 1408,608 };
							door4 = (Door*)entityManager->CreateEntity2(EntityType::DOOR, position, currentPlayer, 4);

							Item* item = new OrbFragment(iPoint(2144, 2496), atlas);
							items.push_back(item);

							item = new OrbFragment(iPoint(2368, 2496), atlas);
							items.push_back(item);

							Statue* statue = nullptr;
							position = { 1184, 1937 };
							statue = (Statue*)entityManager->CreateEntity2(EntityType::STATUE, position, currentPlayer, 1);
							Statue* statue2 = nullptr;
							position = { 1184, 1990 };
							statue2 = (Statue*)entityManager->CreateEntity2(EntityType::STATUE, position, currentPlayer, 2);
							Statue* statue3 = nullptr;
							position = { 1184, 730 };
							statue3 = (Statue*)entityManager->CreateEntity2(EntityType::STATUE, position, currentPlayer, 3);
							Statue* statue4 = nullptr;
							position = { 1664, 730 };
							statue4 = (Statue*)entityManager->CreateEntity2(EntityType::STATUE, position, currentPlayer, 4);

							interruptorBlock = new InterruptorBlock(iPoint(1024, 416), atlas);
							items.push_back(interruptorBlock);

							loadObjects = false;
						}
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 18) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
						{
							// TODO IMPLEMENT PUZZLE BOX INTERRUPTOR
							interruptorBlock->bounds.x = currentPlayer->bounds.x;
							interruptorBlock->bounds.y = currentPlayer->bounds.y;
							items.push_back(interruptorBlock);
							interruptorBlock->isDropped = true;
						}
					}
					if ((layer->Get(i, j) == 21) && CheckCollision(map->GetTilemapRec(i, j), rect) && interruptorBlock->isDropped)
					{
						entityManager->DeleteEntity(EntityType::DOOR, 2);
					}
					if ((layer->Get(i, j) == 17) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						entityManager->DeleteEntity(EntityType::STATUE, 4);
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 15) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						entityManager->DeleteEntity(EntityType::DOOR, 4);
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 16) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						entityManager->DeleteEntity(EntityType::STATUE, 3);
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 9) && CheckCollision(map->GetTilemapRec(i, j), iceBlockRect))
					{
						isTown = false;
						entityManager->SetAllNpcInactive();
						iPoint position = { 1950,360 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						//map->CleanUp();
						//map->Load("dungeon_map.tmx", app->tex);
						isDungeon = true;
						ChangeBlockBounds(2048, 320);
						exit = true;
						break;
					}
					if (deleteDoor && (layer->Get(i, j) == 10) && CheckCollision(map->GetTilemapRec(i, j), iceBlockRect))
					{
						isTown = false;
						entityManager->SetAllNpcInactive();
						//map->CleanUp();
						entityManager->DeleteEntity(EntityType::DOOR, 1);
						app->audio->PlayFx(channel, puzzleCompletedFx);
						deleteDoor = false;
						//map->Load("dungeon_map.tmx", app->tex);
						isDungeon = true;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 3) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = false;
						entityManager->SetAllNpcInactive();
						iPoint position = { 1950,1650 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						//map->CleanUp();
						//map->Load("dungeon_map.tmx", app->tex);
						isDungeon = true;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 4) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = false;
						entityManager->SetAllNpcInactive();
						iPoint position = { 2570,1650 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						//map->CleanUp();
						//map->Load("dungeon_map.tmx", app->tex);
						isDungeon = true;
						exit = true;
						break;
					}
					if ((layer->Get(i, j) == 13) && CheckCollision(map->GetTilemapRec(i, j), rect))
					{
						isTown = true;
						iPoint position = { 630,360 };
						currentPlayer->bounds.x = position.x;
						currentPlayer->bounds.y = position.y;
						map->CleanUp();
						map->Load("graveyard.tmx", app->tex);
						isDungeon = false;
						entityManager->DeleteEntity(EntityType::DOOR);
						entityManager->DeleteEntity(EntityType::ICE_BLOCK);
						entityManager->DeleteEntity(EntityType::STATUE);
						loadObjects = true;
						exit = true;
						break;
					}
				}
			}
		}
		if (exit) break;
	}

	return false;
}

bool SceneGameplay::CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;

	else return false;
}

void SceneGameplay::GenerateBattle()
{
	tmpPosPlayer = iPoint(currentPlayer->bounds.x, currentPlayer->bounds.y);
	transition = true;
	fadeOut = true;
	TransitionsManager::GetInstance()->SetType(TransitionType::WIPE);
	TransitionsManager::GetInstance()->SetStep(TransitionStep::ENTERING);
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

void SceneGameplay::Transitioning(float dt)
{
	if (fadeOut)
	{
		TransitionsManager::GetInstance()->EnteringTransition(dt);
		if (TransitionsManager::GetInstance()->GetStep() == TransitionStep::CHANGING)
		{
			if (sceneBattle == nullptr)
			{
				sceneBattle = new SceneBattle(playerList, tmp, this, inventory);
				sceneBattle->Load();
				gameState = GameplayState::BATTLE;
				fadeOut = false;
			}
			else
			{
				sceneBattle->UnLoad();
				RELEASE(sceneBattle);
				eastl::list<Enemy*>::iterator en = enemyList.begin();
				eastl::list<Enemy*>::iterator enEnd = enemyList.end();
				for (; en != enEnd; ++en)
				{
					Enemy* enemy = (*en);
					if (tmp != nullptr && enemy == tmp)
					{
						QuestManager::GetInstance()->CheckQuests(*en);
						tmp = nullptr;
						enemyList.erase(en);
						break;
					}
				}
				eastl::list<Player*>::iterator pl = playerList.begin();
				eastl::list<Player*>::iterator plEnd = playerList.end();
				int num = 0;
				for (; pl != plEnd; ++pl)
				{
					if ((*pl)->GetHealthPoints() <= 0) ++num;

					(*pl)->GetHealed((*pl)->GetMaxHealthPoints());
					(*pl)->GetMana((*pl)->GetMaxManaPoints());
				}

				if (num == playerList.size())
				{
					TransitionToScene(SceneType::ENDING, TransitionType::ALTERNATING_BARS, false);
				}
				else
				{
					fadeOut = false;
					gameState = GameplayState::ROAMING;
				}
				
				currentPlayer->bounds.x = tmpPosPlayer.x;
				currentPlayer->bounds.y = tmpPosPlayer.y;
				app->audio->PlayMusic("Audio/Music/village_theme_1.ogg", 0);
			}
		}
	}
	else
	{
		if (TransitionsManager::GetInstance()->ExitingTransition(dt) == TransitionStep::NONE) transition = false;
	}
}

void SceneGameplay::ChangeBlockBounds(int bounds_x, int bounds_y)
{
	if (isDungeon)
	{
		eastl::list<Entity*>::iterator entity = entityManager->entities.begin();
		eastl::list<Entity*>::iterator entityEnd = entityManager->entities.end();

		for (; entity != entityEnd; ++entity)
		{
			if (((*entity)->type) == EntityType::ICE_BLOCK)
			{
				(*entity)->bounds.x = bounds_x;
				(*entity)->bounds.y = bounds_y;
				(*entity)->SetState(EntityState::STOP_DOWN);
				break;
			}
		}
	}
}

void SceneGameplay::LoadNpc(SString mapName)
{
	pugi::xml_document animations;
	pugi::xml_node anims;
	pugi::xml_parse_result result = animations.load_file("animations.xml");
	if (result == NULL)
		LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else
		anims = animations.child("animations");

	if (mapName == "town_map.tmx")
	{
		iPoint position = { 200,250 };
		entityManager->CreateEntity(EntityType::TOWN, position, anims, 7);

		position = { 700,1060 };
		entityManager->CreateEntity(EntityType::RAY, position, anims, 3);

		position = { 700,810 };
		entityManager->CreateEntity(EntityType::TABERN, position, anims, 2);

		position = { 500,350 };
		entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 1);
	}
	else if (mapName == "pub.tmx")
	{
		iPoint position = { 670,280 };
		Npc* npc = (Npc*)entityManager->CreateEntity(EntityType::TOWN, position, anims, 4);
		npc->NpcMove(false);

		position = { 670,360 };
		entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 1);
	}
	else if (mapName == "adventurer_house.tmx")
	{
		iPoint position = { 630,450 };
		entityManager->CreateEntity(EntityType::TABERN, position, anims, 4);

		position = { 630,420 };
		entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 1);
	}
	else if (mapName == "library.tmx")
	{
		iPoint position = { 615,460 };
		entityManager->CreateEntity(EntityType::TOWN, position, anims, 5);

		position = { 600,450 };
		entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 6);
	}
}