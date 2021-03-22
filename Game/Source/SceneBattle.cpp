#include "App.h"
#include "Render.h"

#include "SceneBattle.h"
#include "Player.h"

SceneBattle::SceneBattle(Player* p1, Player* p2) : player1(p1), player2(p2)
{
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load()
{
	player1->bounds.x = 50;
	player1->bounds.y = 350;

	player2->bounds.x = 50;
	player2->bounds.y = 400;

	return true;
}

bool SceneBattle::Update(float dt)
{
	/*player1->Update(dt);
	player2->Update(dt);*/

	return true;
}

void SceneBattle::Draw(bool colliders)
{
	player1->Draw(colliders);
	player2->Draw(colliders);
}

bool SceneBattle::UnLoad()
{
	return true;
}
