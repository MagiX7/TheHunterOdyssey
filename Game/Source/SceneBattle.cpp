#include "App.h"
#include "Render.h"

#include "SceneBattle.h"
#include "SceneGameplay.h"
#include "BattleMenu.h"
#include "Player.h"
#include "Enemy.h"
#include "Golem.h"

SceneBattle::SceneBattle(eastl::list<Player*> list, int enemies, SceneGameplay* s) : playerList(list), numEnemies(enemies), scene(s)
{
	battleMenu = new BattleMenu(this);
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load()
{
	font = new Font("Assets/Font/font3.xml", app->tex);

	eastl::list<Player*>::iterator it = playerList.begin();
	for (int i = 0; it != playerList.end(); ++it, ++i)
	{
		(*it)->stance = PlayerStance::BATTLE;
		(*it)->SetIdleBattle();
		(*it)->bounds.x = 400;
		(*it)->bounds.y = 221 + (i * 50);
	}
	
	for (int i = 0; i < numEnemies; ++i)
	{
		Golem* enemy = new Golem(iPoint(650,250+(i*50)));
		enemyList.push_back(enemy);
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

	ret = battleMenu->Update(dt);

	return ret;
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
	}

	enemyList.clear();


	return true;
}