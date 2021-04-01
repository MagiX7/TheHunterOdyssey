#include "App.h"
#include "Font.h"
#include "BattleMenu.h"

#include "Enemy.h"
#include "Player.h"
#include "Input.h"

BattleMenu::BattleMenu(eastl::list<Player*> list, eastl::list<Enemy*> enList) : playerList(list), enemyList(enList), type(BattleState::NONE)
{
}

BattleMenu::~BattleMenu()
{
}

bool BattleMenu::Load()
{
	font = new Font("Assets/Font/font3.xml", app->tex);

	btnAttack = new GuiButton(1, { 72, 480, 204, 43 }, "Attack", this);
	btnAbility = new GuiButton(2, { 72, 530, 204, 43 }, "Ability", this);
	btnDefense = new GuiButton(3, { 72, 580, 204, 43 }, "Defense", this);
	btnObject = new GuiButton(4, { 72, 630, 204, 43 }, "Object", this);

	currEnemy = (*enemyList.begin());
	currPlayer = (*playerList.begin());

	return true;
}

bool BattleMenu::Update(float dt)
{
	switch (type)
	{
	case BattleState::NONE:
		type = BattleState::DEFAULT;
		break;
	case BattleState::DEFAULT:
		btnAttack->Update(app->input, dt);
		btnAbility->Update(app->input, dt);
		btnDefense->Update(app->input, dt);
		btnObject->Update(app->input, dt);
		break;
	case BattleState::ATTACK:
		HandleInput(app->input);
		break;
	case BattleState::ABILITY:
		HandleInput(app->input);
		break;
	case BattleState::DEFENSE:
		break;
	case BattleState::OBJECT:
		break;
	}

	return true;
}

void BattleMenu::Draw(bool showColliders)
{
	switch (type)
	{
	case BattleState::NONE:
		type = BattleState::DEFAULT;
		break;
	case BattleState::DEFAULT:
		btnAttack->Draw(app->render, showColliders);
		btnAbility->Draw(app->render, showColliders);
		btnDefense->Draw(app->render, showColliders);
		btnObject->Draw(app->render, showColliders);
		break;
	case BattleState::ATTACK:
		app->render->DrawRectangle({ currEnemy->bounds.x + 100, currEnemy->bounds.y, 32, 16 }, 255, 0, 0);
		app->render->DrawRectangle({ currPlayer->bounds.x - 100, currPlayer->bounds.y, 32, 16 }, 0, 255, 0);
		break;
	case BattleState::ABILITY:
		app->render->DrawRectangle({ currEnemy->bounds.x + 100, currEnemy->bounds.y, 32, 16 }, 255, 0, 0);
		app->render->DrawRectangle({ currPlayer->bounds.x - 100, currPlayer->bounds.y, 32, 16 }, 0, 255, 0);
		break;
	case BattleState::DEFENSE:
		break;
	case BattleState::OBJECT:
		break;
	}

	if (type != BattleState::NONE) DrawStats();
}

bool BattleMenu::UnLoad()
{
	RELEASE(btnAttack);
	RELEASE(btnAbility);
	RELEASE(btnDefense);
	RELEASE(btnObject);

	return true;
}

bool BattleMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		// TODO implement functionality for buttons
		if (control->id == 1) type = BattleState::ATTACK;
		else if (control->id == 2) type = BattleState::ABILITY;
		else if (control->id == 3) type = BattleState::DEFENSE;
		else if (control->id == 4) type = BattleState::OBJECT;
		break;
	}
	return true;
}

void BattleMenu::DrawStats()
{
	eastl::list<Player*>::iterator it = playerList.begin();

	for (int i = 0; it != playerList.end(); ++it, ++i)
	{
		app->render->DrawText(font, (*it)->GetName().c_str(), 850, 500+(i*50), 25, 2, { 255,255,255 });
		app->render->DrawText(font, "HP", 1000, 500 + (i * 50), 25, 2, { 255,255,255 });
		std::string points = std::to_string((*it)->GetHealthPoints());
		app->render->DrawText(font, points.c_str(), 1030, 500 + (i * 50), 25, 2, { 255,255,255 });
		points = std::to_string((*it)->GetManaPoints());
		app->render->DrawText(font, "MP", 1130, 500 + (i * 50), 25, 2, { 255,255,255 });
		app->render->DrawText(font, points.c_str(), 1160, 500 + (i * 50), 25, 2, { 255,255,255 });
	}
}

void BattleMenu::HandleInput(Input* input)
{
	if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		eastl::list<Enemy*>::iterator it = enemyList.begin();
		for (; it != enemyList.end(); ++it)
		{
			if (currEnemy == (*enemyList.end().prev()))
			{
				currEnemy = (*enemyList.begin());
				break;
			}
			else if ((*it) == currEnemy)
			{
				currEnemy = (*it.next());
				break;
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		eastl::list<Enemy*>::iterator it = enemyList.begin();
		for (; it != enemyList.end(); ++it)
		{
			if (currEnemy == (*enemyList.begin()))
			{
				currEnemy = (*enemyList.end().prev());
				break;
			}
			else if ((*it) == currEnemy)
			{
				currEnemy = (*it.prev());
				break;
			}
		}
	}
}