#include "App.h"
#include "Font.h"
#include "BattleMenu.h"
#include "SceneBattle.h"


#include "Enemy.h"
#include "Player.h"
#include "Input.h"
#include <time.h>
#include "Textures.h"

BattleMenu::BattleMenu(SceneBattle* s) : type(BattleState::NONE), sceneBattle(s)
{
	enemyKilled = false;
}

BattleMenu::~BattleMenu()
{
}

bool BattleMenu::Load(Font* font)
{
	guiTex = app->tex->Load("Assets/Textures/UI/gui_battle_menu.png");

	btnAttack = new GuiButton(1, { 72, 480, 204, 43 }, "Attack", this, font);
	btnAttack->texture = guiTex;
	btnAttack->sectionFocused = { 0,260,204,43 };
	btnAbility = new GuiButton(2, { 72, 530, 204, 43 }, "Ability", this, font);
	btnAbility->texture = guiTex;
	btnAbility->sectionFocused = { 0,260,204,43 };
	btnDefense = new GuiButton(3, { 72, 580, 204, 43 }, "Defense", this, font);
	btnDefense->texture = guiTex;
	btnDefense->sectionFocused = { 0,260,204,43 };
	btnObject = new GuiButton(4, { 72, 630, 204, 43 }, "Object", this, font);
	btnObject->texture = guiTex;
	btnObject->sectionFocused = { 0,260,204,43 };

	btnAbilitySlot1 = new GuiButton(5, { 72, 500, 204, 43 }, "Ability1", this, font);
	btnAbilitySlot1->texture = guiTex;
	btnAbilitySlot1->sectionFocused = { 0,260,204,43 };
	btnAbilitySlot2 = new GuiButton(6, { 314, 500, 204, 43 }, "Ability2", this, font);
	btnAbilitySlot2->texture = guiTex;
	btnAbilitySlot2->sectionFocused = { 0,260,204,43 };
	btnAbilitySlot3 = new GuiButton(7, { 72, 565, 204, 43 }, "Ability3", this, font);
	btnAbilitySlot3->texture = guiTex;
	btnAbilitySlot3->sectionFocused = { 0,260,204,43 };
	btnAbilitySlot4 = new GuiButton(8, { 314, 565, 204, 43 }, "Ability4", this, font);
	btnAbilitySlot4->texture = guiTex;
	btnAbilitySlot4->sectionFocused = { 0,260,204,43 };

	btnObjectSlot1 = new GuiButton(9, { 72, 500, 204, 43 }, "Object1", this, font);
	btnObjectSlot1->texture = guiTex;
	btnObjectSlot1->sectionFocused = { 0,260,204,43 };
	btnObjectSlot2 = new GuiButton(10, { 314, 500, 204, 43 }, "Object2", this, font);
	btnObjectSlot2->texture = guiTex;
	btnObjectSlot2->sectionFocused = { 0,260,204,43 };
	btnObjectSlot3 = new GuiButton(11, { 72, 565, 204, 43 }, "Object3", this, font);
	btnObjectSlot3->texture = guiTex;
	btnObjectSlot3->sectionFocused = { 0,260,204,43 };
	btnObjectSlot4 = new GuiButton(12, { 314, 565, 204, 43 }, "Object4", this, font);
	btnObjectSlot4->texture = guiTex;
	btnObjectSlot4->sectionFocused = { 0,260,204,43 };

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
		btnAttack->Update(app->input, dt, -1);
		btnAbility->Update(app->input, dt, -1);
		btnDefense->Update(app->input, dt, -1);
		btnObject->Update(app->input, dt, -1);
		break;
	case BattleState::ATTACK:
		ret = HandleInput(app->input);
		break;
	case BattleState::ABILITY_SELECT:
		btnAbilitySlot1->Update(app->input, dt, -1);
		btnAbilitySlot2->Update(app->input, dt, -1);
		btnAbilitySlot3->Update(app->input, dt, -1);
		btnAbilitySlot4->Update(app->input, dt, -1);
		//ret = HandleAbilities(app->input);
		break;
	case BattleState::ABILITY:
		ret = HandleAbilities(app->input, currPlayer->GetAbilitySelected());
		break;
	case BattleState::ENEMY_TURN:
		EnemyTurn();
		break;
	case BattleState::DEFENSE:
		ret = HandleDefense(app->input);
		break;
	case BattleState::OBJECT_SELECT:
		btnObjectSlot1->Update(app->input, dt, -1);
		btnObjectSlot2->Update(app->input, dt, -1);
		btnObjectSlot3->Update(app->input, dt, -1);
		btnObjectSlot4->Update(app->input, dt, -1);
		break;
	case BattleState::OBJECT:
		ret = HandleObjects(app->input, currPlayer->GetObjectSelected());
		break;
	}

	if (enemyKilled) EraseEnemy();
	if (sceneBattle->enemyList.size() == 0) ret = false;

	return ret;
}

void BattleMenu::Draw(Font* font, bool showColliders)
{
	SDL_Rect section = { 0,0,1240,260 };
	app->render->DrawTexture(guiTex, 20, 440, &section);

	switch (type)
	{
	case BattleState::NONE:
		type = BattleState::DEFAULT;
		break;
	case BattleState::DEFAULT:
		app->render->DrawRectangle({ currPlayer->bounds.x - 100, currPlayer->bounds.y, 32, 16 }, 0, 255, 0);
		btnAttack->Draw(app->render, showColliders, 25, { 0,0,0,225 });
		btnAbility->Draw(app->render, showColliders, 25, { 0,0,0,225 });
		btnDefense->Draw(app->render, showColliders, 25, { 0,0,0,225 });
		btnObject->Draw(app->render, showColliders, 25, { 0,0,0,225 });
		break;
	case BattleState::ATTACK:
		app->render->DrawRectangle({ currEnemy->bounds.x + 100, currEnemy->bounds.y, 32, 16 }, 255, 0, 0);
		app->render->DrawRectangle({ currPlayer->bounds.x - 100, currPlayer->bounds.y, 32, 16 }, 0, 255, 0);
		break;
	case BattleState::ABILITY_SELECT:
		btnAbilitySlot1->Draw(app->render, showColliders, 25, { 0,0,0,225 });
		btnAbilitySlot2->Draw(app->render, showColliders, 25, { 0,0,0,225 });
		btnAbilitySlot3->Draw(app->render, showColliders, 25, { 0,0,0,225 });
		btnAbilitySlot4->Draw(app->render, showColliders, 25, { 0,0,0,225 });
		break;
	case BattleState::ABILITY:
		app->render->DrawRectangle({ currPlayer->bounds.x - 100, currPlayer->bounds.y, 32, 16 }, 0, 255, 0);
		app->render->DrawRectangle({ currEnemy->bounds.x + 100, currEnemy->bounds.y, 32, 16 }, 255, 0, 0);
		break;
	case BattleState::ENEMY_TURN:

		break;
	case BattleState::DEFENSE:
		
		break;
	case BattleState::OBJECT_SELECT:
		btnObjectSlot1->Draw(app->render, showColliders, 25, { 0,0,0,255 });
		btnObjectSlot2->Draw(app->render, showColliders, 25, { 0,0,0,255 });
		btnObjectSlot3->Draw(app->render, showColliders, 25, { 0,0,0,255 });
		btnObjectSlot4->Draw(app->render, showColliders, 25, { 0,0,0,255 });

		app->render->DrawRectangle({ currPlayer->bounds.x - 100, currPlayer->bounds.y, 32, 16 }, 0, 255, 0);
		break;
	case BattleState::OBJECT:
		app->render->DrawRectangle({ currPlayer->bounds.x - 100, currPlayer->bounds.y, 32, 16 }, 0, 255, 0);
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

	RELEASE(btnObjectSlot1);
	RELEASE(btnObjectSlot2);
	RELEASE(btnObjectSlot3);
	RELEASE(btnObjectSlot4);

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
		else if (control->id == 4)
		{
			type = BattleState::OBJECT_SELECT;
			tempPlayer = currPlayer;
		}
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
		else if (control->id == 9)
		{
			type = BattleState::OBJECT;
			currPlayer->SetObjectSelected(1);
		}
		else if (control->id == 10)
		{
			type = BattleState::OBJECT;
			currPlayer->SetObjectSelected(2);
		}
		else if (control->id == 11)
		{
			type = BattleState::OBJECT;
			currPlayer->SetObjectSelected(3);
		}
		else if (control->id == 12)
		{
			type = BattleState::OBJECT;
			currPlayer->SetObjectSelected(4);
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
		app->render->DrawText(font, (*it)->GetName().c_str(), 850, 480+(i*55), 25, 2, { 0,0,0 });
		app->render->DrawText(font, "HP", 1000, 480 + (i * 55), 25, 2, { 0,0,0 });
		std::string points = std::to_string((*it)->GetHealthPoints());
		app->render->DrawText(font, points.c_str(), 1030, 480 + (i * 55), 25, 2, { 0,0,0 });
		points = std::to_string((*it)->GetManaPoints());
		app->render->DrawText(font, "MP", 1130, 480 + (i * 55), 25, 2, { 0,0,0 });
		app->render->DrawText(font, points.c_str(), 1160, 480 + (i * 55), 25, 2, { 0,0,0 });
	}
}

bool BattleMenu::HandleInput(Input* input)
{
	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pads->down)
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
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pads->up)
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
	else if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->pads->a)
	{
		currPlayer->Attack(currEnemy);
		
		if (currEnemy->GetHealth() <= 0)
		{
			enemyKilled = true;
		}
		if (sceneBattle->enemyList.size() != 0)
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
	if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || input->pads->down)
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
	else if (input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || input->pads->up)
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
	else if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->pads->a)
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

bool BattleMenu::HandleObjects(Input* input, int currentObject)
{
	if (input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || input->pads->down)
	{
		eastl::list<Player*>::iterator it = sceneBattle->playerList.begin();
		for (; it != sceneBattle->playerList.end(); ++it)
		{
			if (currPlayer == (*sceneBattle->playerList.end().prev()))
			{
				currPlayer = (*sceneBattle->playerList.begin());
				break;
			}
			else if ((*it) == currPlayer)
			{
				currPlayer = (*it.next());
				break;
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || input->pads->up)
	{
		eastl::list<Player*>::iterator it = sceneBattle->playerList.begin();
		for (; it != sceneBattle->playerList.end(); ++it)
		{
			if (currPlayer == (*sceneBattle->playerList.begin()))
			{
				currPlayer = (*sceneBattle->playerList.end().prev());
				break;
			}
			else if ((*it) == currPlayer)
			{
				currPlayer = (*it.prev());
				break;
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->pads->a)
	{
		currPlayer->UseObject(currPlayer, currentObject);

		currPlayer = tempPlayer;
		tempPlayer = nullptr;
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
	return true;
}

bool BattleMenu::HandleDefense(Input* input)
{
	currPlayer->SetDefend(true);

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
	return true;
}

void BattleMenu::EraseEnemy()
{
	eastl::list<Enemy*>::iterator it = sceneBattle->enemyList.begin();
	for (;it != sceneBattle->enemyList.end(); ++it)
	{
		if ((*it)->AnimationFinished() == true)
		{
			//currEnemy = (*it.next());
			(*it)->UnLoad();
			RELEASE((*it));
			sceneBattle->enemyList.erase(it);
			currEnemy = (*sceneBattle->enemyList.begin()); // Fixed current enemy assign. Before, if you killed the 3rd enemy (the last one) it crashed.
			enemyKilled = false;
			break;
		}
	}
}

void BattleMenu::EnemyTurn()
{
	int randNum = rand() % sceneBattle->playerList.size();
	eastl::list<Player*>::iterator pIt = sceneBattle->playerList.begin();

	for (int i = 0; i < randNum; ++i)
	{
		++pIt;
	}

	int num = 0;

	eastl::list<Enemy*>::iterator eIt = sceneBattle->enemyList.begin();
	for (; eIt != sceneBattle->enemyList.end(); ++eIt)
	{
		if ((*eIt)->GetCurrentState() == EnemyState::ATTACKING) break;
		else if ((*eIt)->GetCurrentState() == EnemyState::NORMAL)
		{
			(*eIt)->Attack((*pIt));
			break;
		}
		else if ((*eIt)->GetCurrentState() == EnemyState::ATTACK_FINISHED)
		{
			num++;
		}
	}

	if (num == sceneBattle->enemyList.size())
	{
		eastl::list<Player*>::iterator itPlayer = sceneBattle->playerList.begin();
		for (; itPlayer != sceneBattle->playerList.end(); ++itPlayer)
		{
			(*itPlayer)->SetDefend(false);
		}
		type = BattleState::DEFAULT;

		eastl::list<Enemy*>::iterator eIt = sceneBattle->enemyList.begin();
		for (; eIt != sceneBattle->enemyList.end(); ++eIt)
		{
			(*eIt)->SetCurrentState(EnemyState::NORMAL);
		}
	}
}