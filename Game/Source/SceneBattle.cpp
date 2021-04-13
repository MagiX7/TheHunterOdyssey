#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "SceneBattle.h"
#include "SceneGameplay.h"
#include "Map.h"
#include "BattleMenu.h"
#include "Player.h"
#include "Enemy.h"
#include "Golem.h"
#include "Skull.h"
#include "Bat.h"

#include <time.h>

SceneBattle::SceneBattle(eastl::list<Player*> list, int enemies, SceneGameplay* s) : playerList(list), numEnemies(enemies), scene(s)
{
	battleMenu = new BattleMenu(this);
	map = new Map();
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load()
{
	font = new Font("Assets/Font/font3.xml", app->tex);

	//backgroundTexture = app->tex->Load("Assets/Textures/Scenes/battle_bg.png");
	map->Load("battle_map.tmx", app->tex);
	
	eastl::list<Player*>::iterator it = playerList.begin();
	for (int i = 0; it != playerList.end(); ++it, ++i)
	{
		(*it)->stance = PlayerStance::BATTLE;
		(*it)->SetIdleBattle();
		(*it)->bounds.x = 400;
		(*it)->bounds.y = 205 + (i * 50);
		(*it)->battlePos = iPoint((*it)->bounds.x, (*it)->bounds.y);
	}

	Enemy* enemy = nullptr;
	for (int i = 0; i < numEnemies; ++i)
	{
		int num = (rand() % 3) + 1;
		switch(num)
		{
		case 1:
			enemy = new Golem(iPoint(650, 190 + (i * 90)));
			break;
		case 2:
			enemy = new Skull(iPoint(650, 190 + (i * 90)));
			break;
		case 3:
			enemy = new Bat(iPoint(650, 190 + (i * 90)));
			break;
		}
		if (enemy != nullptr) enemyList.push_back(enemy);
	}

	battleMenu->Load(font);

	return true;
}

bool SceneBattle::Update(float dt)
{
	bool ret = true;

	eastl::list<Player*>::iterator item = playerList.begin();
	for (; item != playerList.end(); ++item)
	{
		(*item)->Update(dt);
	}

	eastl::list<Enemy*>::iterator it = enemyList.begin();
	for (; it != enemyList.end(); ++it)
	{
		(*it)->Update(dt);
	}

	if (battleMenu->Update(dt) == false) ret = false;

	return ret;
}

void SceneBattle::Draw(bool colliders)
{
	//app->render->DrawTexture(backgroundTexture,0,0);
	map->Draw(colliders);

	eastl::list<Player*>::iterator item = playerList.begin();
	for (; item != playerList.end(); ++item)
	{
		(*item)->Draw(colliders);
	}
	
	eastl::list<Enemy*>::iterator it = enemyList.begin();
	for (; it != enemyList.end(); ++it)
	{
		(*it)->Draw(colliders);
	}

	battleMenu->Draw(font, colliders);
}

bool SceneBattle::UnLoad()
{
	battleMenu->UnLoad();
	RELEASE(battleMenu);

	eastl::list<Player*>::iterator item = playerList.begin();
	for (; item != playerList.end(); ++item)
	{
		(*item)->stance = PlayerStance::ROAMING;
		(*item)->SetIdleRoaming();
	}

	enemyList.clear();
	map->CleanUp();

	return true;
}