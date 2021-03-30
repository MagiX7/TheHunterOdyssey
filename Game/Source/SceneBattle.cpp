#include "App.h"
#include "Render.h"

#include "SceneBattle.h"
#include "BattleMenu.h"
#include "Player.h"
#include "Enemy.h"
#include "Golem.h"

SceneBattle::SceneBattle(eastl::list<Player*> list, int enemies) : playerList(list), numEnemies(enemies)
{
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load()
{
	eastl::list<Player*>::iterator it = playerList.begin();
	for (int i = 0; it != playerList.end(); ++it, ++i)
	{
		(*it)->stance = PlayerStance::BATTLE;
		(*it)->bounds.x = 400;
		(*it)->bounds.y = 221 + (i * 50);
	}
	
	for (int i = 0; i < numEnemies; ++i)
	{
		Golem* enemy = new Golem(iPoint(650,250+(i*50)));
		enemyList.push_back(enemy);
	}

	battleMenu = new BattleMenu(playerList, enemyList);
	battleMenu->Load();

	return true;
}

bool SceneBattle::Update(float dt)
{
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

	battleMenu->Update(dt);

	return true;
}

void SceneBattle::Draw(bool colliders)
{
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

	battleMenu->Draw(colliders);
}

bool SceneBattle::UnLoad()
{
	battleMenu->UnLoad();
	RELEASE(battleMenu);

	return true;
}