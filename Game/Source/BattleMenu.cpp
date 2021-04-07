#include "App.h"
#include "Font.h"
#include "BattleMenu.h"
#include "SceneBattle.h"


#include "Enemy.h"
#include "Player.h"
#include "Input.h"
#include <time.h>

BattleMenu::BattleMenu(SceneBattle* s) : type(BattleState::NONE), sceneBattle(s)
{
}

BattleMenu::~BattleMenu()
{
}

bool BattleMenu::Load(Font* font)
{
	btnAttack = new GuiButton(1, { 72, 480, 204, 43 }, "Attack", this, font);
	btnAbility = new GuiButton(2, { 72, 530, 204, 43 }, "Ability", this, font);
	btnDefense = new GuiButton(3, { 72, 580, 204, 43 }, "Defense", this, font);
	btnObject = new GuiButton(4, { 72, 630, 204, 43 }, "Object", this, font);

	btnAbilitySlot1 = new GuiButton(5, { 72, 500, 204, 43 }, "Ability1", this, font);
	btnAbilitySlot2 = new GuiButton(6, { 314, 500, 204, 43 }, "Ability2", this, font);
	btnAbilitySlot3 = new GuiButton(7, { 72, 565, 204, 43 }, "Ability3", this, font);
	btnAbilitySlot4 = new GuiButton(8, { 314, 565, 204, 43 }, "Ability4", this, font);

	currEnemy = (*sceneBattle->enemyList.begin());
	currPlayer = (*sceneBattle->playerList.begin());

	srand(time(NULL));

	return true;
}

bool BattleMenu::Update(float dt)
{
	bool ret = true;
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
		ret = HandleInput(app->input);
		break;
	case BattleState::ABILITY_SELECT:
		btnAbilitySlot1->Update(app->input, dt);
		btnAbilitySlot2->Update(app->input, dt);
		btnAbilitySlot3->Update(app->input, dt);
		btnAbilitySlot4->Update(app->input, dt);
		//ret = HandleAbilities(app->input);
		break;
	case BattleState::ABILITY:
		ret = HandleAbilities(app->input, currPlayer->GetAbilitySelected());
		break;
	case BattleState::ENEMY_TURN:
		EnemyTurn();
		break;
	case BattleState::DEFENSE:
		break;
	case BattleState::OBJECT:
		break;
	}

	return ret;
}

void BattleMenu::Draw(Font* font, bool showColliders)
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
	case BattleState::ABILITY_SELECT:
		btnAbilitySlot1->Draw(app->render, showColliders);
		btnAbilitySlot2->Draw(app->render, showColliders);
		btnAbilitySlot3->Draw(app->render, showColliders);
		btnAbilitySlot4->Draw(app->render, showColliders);
		break;
	case BattleState::ABILITY:
		app->render->DrawRectangle({ currPlayer->bounds.x - 100, currPlayer->bounds.y, 32, 16 }, 0, 255, 0);
		app->render->DrawRectangle({ currEnemy->bounds.x + 100, currEnemy->bounds.y, 32, 16 }, 255, 0, 0);
		break;
	case BattleState::ENEMY_TURN:

		break;
	case BattleState::DEFENSE:
		break;
	case BattleState::OBJECT:
		break;
	}

	if (type != BattleState::NONE) DrawStats(font);
}

bool BattleMenu::UnLoad()
{
	RELEASE(btnAttack);
	RELEASE(btnAbility);
	RELEASE(btnDefense);
	RELEASE(btnObject);

	RELEASE(btnAbilitySlot1);
	RELEASE(btnAbilitySlot2);
	RELEASE(btnAbilitySlot3);
	RELEASE(btnAbilitySlot4);

	currEnemy = nullptr;
	currPlayer = nullptr;
	sceneBattle = nullptr;

	return true;
}

bool BattleMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		// TODO implement functionality for buttons
		if (control->id == 1) type = BattleState::ATTACK;
		else if (control->id == 2) type = BattleState::ABILITY_SELECT;
		else if (control->id == 3) type = BattleState::DEFENSE;
		else if (control->id == 4) type = BattleState::OBJECT;
		else if (control->id == 5)
		{
			type = BattleState::ABILITY;
			currPlayer->SetAbilitySelected(1);
		}
		else if (control->id == 6)
		{
			type = BattleState::ABILITY;
			currPlayer->SetAbilitySelected(2);
		}
		else if (control->id == 7)
		{
			type = BattleState::ABILITY;
			currPlayer->SetAbilitySelected(3);
		}
		else if (control->id == 8)
		{
			type = BattleState::ABILITY;
			currPlayer->SetAbilitySelected(4);
		}
		break;
	}
	return true;
}

void BattleMenu::DrawStats(Font* font)
{
	eastl::list<Player*>::iterator it = sceneBattle->playerList.begin();

	for (int i = 0; it != sceneBattle->playerList.end(); ++it, ++i)
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

bool BattleMenu::HandleInput(Input* input)
{
	if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		eastl::list<Enemy*>::iterator it = sceneBattle->enemyList.begin();
		for (; it != sceneBattle->enemyList.end(); ++it)
		{
			if (currEnemy == (*sceneBattle->enemyList.end().prev()))
			{
				currEnemy = (*sceneBattle->enemyList.begin());
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
		eastl::list<Enemy*>::iterator it = sceneBattle->enemyList.begin();
		for (; it != sceneBattle->enemyList.end(); ++it)
		{
			if (currEnemy == (*sceneBattle->enemyList.begin()))
			{
				currEnemy = (*sceneBattle->enemyList.end().prev());
				break;
			}
			else if ((*it) == currEnemy)
			{
				currEnemy = (*it.prev());
				break;
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		currPlayer->Attack(currEnemy);
		
		if (currEnemy->GetHealth() <= 0)
		{
			EraseEnemy();
		}
		if (sceneBattle->enemyList.size() == 0)
		{
			return false;
		}
		else
		{
			eastl::list<Player*>::iterator it = sceneBattle->playerList.begin();
			for (int i = 0; it != sceneBattle->playerList.end(); ++it, ++i)
			{
				if (currPlayer == (*sceneBattle->playerList.end().prev()))
				{
					currPlayer = (*sceneBattle->playerList.begin());
					type = BattleState::ENEMY_TURN;
					break;
				}
				else if ((*it) == currPlayer)
				{
					currPlayer = (*it.next());
					type = BattleState::DEFAULT;
					break;
				}
			}
		}
	}
	return true;
}

bool BattleMenu::HandleAbilities(Input* input, int currentAbility)
{
	if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		eastl::list<Enemy*>::iterator it = sceneBattle->enemyList.begin();
		for (; it != sceneBattle->enemyList.end(); ++it)
		{
			if (currEnemy == (*sceneBattle->enemyList.end().prev()))
			{
				currEnemy = (*sceneBattle->enemyList.begin());
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
		eastl::list<Enemy*>::iterator it = sceneBattle->enemyList.begin();
		for (; it != sceneBattle->enemyList.end(); ++it)
		{
			if (currEnemy == (*sceneBattle->enemyList.begin()))
			{
				currEnemy = (*sceneBattle->enemyList.end().prev());
				break;
			}
			else if ((*it) == currEnemy)
			{
				currEnemy = (*it.prev());
				break;
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		currPlayer->Ability(currEnemy, currentAbility);

		if (currEnemy->GetHealth() <= 0)
		{
			EraseEnemy();
		}
		if (sceneBattle->enemyList.size() == 0)
		{
			return false;
		}
		else
		{
			eastl::list<Player*>::iterator it = sceneBattle->playerList.begin();
			for (int i = 0; it != sceneBattle->playerList.end(); ++it, ++i)
			{
				if (currPlayer == (*sceneBattle->playerList.end().prev()))
				{
					currPlayer = (*sceneBattle->playerList.begin());
					type = BattleState::ENEMY_TURN;
					break;
				}
				else if ((*it) == currPlayer)
				{
					currPlayer = (*it.next());
					type = BattleState::DEFAULT;
					break;
				}
			}
		}
	}
	return true;
}

void BattleMenu::EraseEnemy()
{
	eastl::list<Enemy*>::iterator it = sceneBattle->enemyList.begin();
	for (;it != sceneBattle->enemyList.end(); ++it)
	{
		if ((*it) == currEnemy)
		{
			currEnemy = (*it.next());
			(*it)->UnLoad();
			RELEASE((*it));
			sceneBattle->enemyList.erase(it);
			break;
		}
	}
}

void BattleMenu::EnemyTurn()
{
	eastl::list<Enemy*>::iterator it = sceneBattle->enemyList.begin();
	for (; it != sceneBattle->enemyList.end(); ++it)
	{
		int randNum = rand() % sceneBattle->playerList.size();
		eastl::list<Player*>::iterator pIt = sceneBattle->playerList.begin();
		
		for (int i = 0; i < randNum; ++i)
		{
			++pIt;
		}
		(*it)->Attack((*pIt));
	}
	type = BattleState::DEFAULT;
}
