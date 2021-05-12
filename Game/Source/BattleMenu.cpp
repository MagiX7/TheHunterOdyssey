#include "App.h"
#include "Textures.h"
#include "Easings.h"

#include "BattleMenu.h"
#include "SceneBattle.h"
#include "Enemy.h"
#include "Player.h"
#include <time.h>

BattleMenu::BattleMenu(SceneBattle* s) : type(BattleState::NONE), sceneBattle(s)
{
	enemyKilled = false;
	playerSelected = false;

	easingArrow = new Easing();
	easingArrowBack = new Easing();
	playerEasing = new Easing();
	enemyEasing = new Easing();
	playerEasing2 = new Easing();
	enemyEasing2 = new Easing();

	easingArrow->totalIterations = 60;
	easingArrow->currentIteration = 0;
	easingArrow->deltaPos = 10;
	easingArrow->initialPos = 20;
	easingArrow->easingsActivated = false;

	easingArrowBack->totalIterations = 60;
	easingArrowBack->currentIteration = 0;
	easingArrowBack->deltaPos = -10;
	easingArrowBack->initialPos = 30;
	easingArrowBack->easingsActivated = false;

	playerEasing->totalIterations = 30;
	playerEasing->currentIteration = 0;
	playerEasing->deltaPos = -10;
	playerEasing->initialPos = 0;
	playerEasing->easingsActivated = true;

	enemyEasing->totalIterations = 30;
	enemyEasing->currentIteration = 0;
	enemyEasing->deltaPos = 10;
	enemyEasing->initialPos = 0;
	enemyEasing->easingsActivated = true;

	playerEasing2->totalIterations = 30;
	playerEasing2->currentIteration = 0;
	playerEasing2->deltaPos = 10;
	playerEasing2->initialPos = 0;
	playerEasing2->easingsActivated = false;

	enemyEasing2->totalIterations = 30;
	enemyEasing2->currentIteration = 0;
	enemyEasing2->deltaPos = -10;
	enemyEasing2->initialPos = 0;
	enemyEasing2->easingsActivated = false;

	alpha = 255;
	position_x = 20;
	playerPos = 0;
	enemyPos = 0;
}

BattleMenu::~BattleMenu()
{
}

bool BattleMenu::Load(Font* font)
{
	guiTex = app->tex->Load("Assets/Textures/UI/gui_battle_menu.png");

	btnAttack = new GuiButton(1, { 160, 480, 204, 43 }, "Attack", this, font);
	btnAttack->texture = guiTex;
	btnAttack->sectionFocused = { 0,260,204,43 };
	btnAbility = new GuiButton(2, { 160, 530, 204, 43 }, "Ability", this, font);
	btnAbility->texture = guiTex;
	btnAbility->sectionFocused = { 0,260,204,43 };
	btnDefense = new GuiButton(3, { 160, 580, 204, 43 }, "Defense", this, font);
	btnDefense->texture = guiTex;
	btnDefense->sectionFocused = { 0,260,204,43 };
	btnObject = new GuiButton(4, { 160, 630, 204, 43 }, "Object", this, font);
	btnObject->texture = guiTex;
	btnObject->sectionFocused = { 0,260,204,43 };

	btnAbilitySlot1 = new GuiButton(5, { 461, 480, 204, 43 }, "Ability1", this, font);
	btnAbilitySlot1->texture = guiTex;
	btnAbilitySlot1->sectionFocused = { 0,260,204,43 };
	btnAbilitySlot2 = new GuiButton(6, { 461, 530, 204, 43 }, "Ability2", this, font);
	btnAbilitySlot2->texture = guiTex;
	btnAbilitySlot2->sectionFocused = { 0,260,204,43 };
	btnAbilitySlot3 = new GuiButton(7, { 461, 580, 204, 43 }, "Ability3", this, font);
	btnAbilitySlot3->texture = guiTex;
	btnAbilitySlot3->sectionFocused = { 0,260,204,43 };
	btnAbilitySlot4 = new GuiButton(8, { 461, 630, 204, 43 }, "Ability4", this, font);
	btnAbilitySlot4->texture = guiTex;
	btnAbilitySlot4->sectionFocused = { 0,260,204,43 };

	btnObjectSlot1 = new GuiButton(9, { 461, 480, 204, 43 }, "Potion", this, font);
	btnObjectSlot1->texture = guiTex;
	btnObjectSlot1->sectionFocused = { 0,260,204,43 };
	btnObjectSlot2 = new GuiButton(10, { 461, 530, 204, 43 }, "Ultra Potion", this, font);
	btnObjectSlot2->texture = guiTex;
	btnObjectSlot2->sectionFocused = { 0,260,204,43 };
	btnObjectSlot3 = new GuiButton(11, { 461, 580, 204, 43 }, "Eter", this, font);
	btnObjectSlot3->texture = guiTex;
	btnObjectSlot3->sectionFocused = { 0,260,204,43 };
	btnObjectSlot4 = new GuiButton(12, { 461, 630, 204, 43 }, "Ultra Eter", this, font);
	btnObjectSlot4->texture = guiTex;
	btnObjectSlot4->sectionFocused = { 0,260,204,43 };

	DefaultStateButtons();

	currentButton = nullptr;
	lastButton = nullptr;

	currEnemy = (*sceneBattle->enemyList.begin());
	currPlayer = (*sceneBattle->playerList.begin());

	return true;
}

bool BattleMenu::Update(float dt)
{
	bool ret = true;

	srand(time(NULL));

	UpdatingButtons(app->input);

	int id = -1;
	if (lastUserInput == 0 && currentButton != nullptr)
	{
		id = currentButton->id;
	}

	switch (type)
	{
	case BattleState::NONE:
		type = BattleState::DEFAULT;
		break;
	case BattleState::DEFAULT:
		btnAttack->Update(app->input, dt, id);
		btnAbility->Update(app->input, dt, id);
		btnDefense->Update(app->input, dt, id);
		btnObject->Update(app->input, dt, id);
		break;
	case BattleState::ATTACK:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			type = BattleState::DEFAULT;
			DefaultStateButtons();
		}
		else ret = HandleInput(app->input);
		
		break;
	case BattleState::ATTACKING:
		ret = HandleInput(app->input);
		break;
	case BattleState::ABILITY_SELECT:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			type = BattleState::DEFAULT;
			DefaultStateButtons();
		}
		else
		{
			btnAbilitySlot1->Update(app->input, dt, id);
			btnAbilitySlot2->Update(app->input, dt, id);
			btnAbilitySlot3->Update(app->input, dt, id);
			btnAbilitySlot4->Update(app->input, dt, id);
		}
		//ret = HandleAbilities(app->input);
		break;
	case BattleState::ABILITY:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			type = BattleState::ABILITY_SELECT;
			AbilityStateButtons();
		}
		else ret = HandleAbilities(app->input, currPlayer->GetAbilitySelected());
		
		break;
	case BattleState::ENEMY_TURN:
		if (tempPlayer->stance == PlayerStance::BATTLE && enemyKilled == false)
			EnemyTurn();
		break;
	case BattleState::DEFENSE:
		ret = HandleDefense(app->input);
		break;
	case BattleState::OBJECT_SELECT:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			type = BattleState::DEFAULT;
			DefaultStateButtons();
		}
		else
		{
			btnObjectSlot1->Update(app->input, dt, id);
			btnObjectSlot2->Update(app->input, dt, id);
			btnObjectSlot3->Update(app->input, dt, id);
			btnObjectSlot4->Update(app->input, dt, id);
		}
		break;
	case BattleState::OBJECT:
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_DOWN)
		{
			currPlayer = tempPlayer;
			type = BattleState::OBJECT_SELECT;
			ObjectStateButtons();
		}
		else ret = HandleObjects(app->input, currPlayer->GetObjectSelected());
		
		break;
	}

	EraseEnemy();

	if (sceneBattle->enemyList.size() == 0) return false;

	eastl::list<Player*>::iterator it = sceneBattle->playerList.begin();
	int i = 0;
	for (; it != sceneBattle->playerList.end(); ++it)
	{
		if ((*it)->GetHealthPoints() <= 0) ++i;
		else break;
	}

	if (i == 4) return false;

	// Update Easings

	if (easingArrow->easingsActivated)
	{
		position_x = easingArrow->circularEaseInOut(easingArrow->currentIteration, easingArrow->initialPos, easingArrow->deltaPos, easingArrow->totalIterations);
		position_x2 = easingArrow->circularEaseInOut(easingArrow->currentIteration, 200, 10, easingArrow->totalIterations);
		alpha = easingArrow->circularEaseInOut(easingArrow->currentIteration, 0, 255, easingArrow->totalIterations);

		if (easingArrow->currentIteration < easingArrow->totalIterations)
		{
			easingArrow->currentIteration++;
		}
		else
		{
			easingArrow->currentIteration = 0;
			easingArrow->easingsActivated = false;
			easingArrowBack->easingsActivated = true;
		}
	}

	if (easingArrowBack->easingsActivated)
	{
		position_x = easingArrowBack->circularEaseInOut(easingArrowBack->currentIteration, easingArrowBack->initialPos, easingArrowBack->deltaPos, easingArrowBack->totalIterations);
		position_x2 = easingArrowBack->circularEaseInOut(easingArrowBack->currentIteration, 210, -10, easingArrowBack->totalIterations);
		alpha = easingArrowBack->circularEaseInOut(easingArrowBack->currentIteration, 255, -255, easingArrowBack->totalIterations);

		if (easingArrowBack->currentIteration < easingArrowBack->totalIterations)
		{
			easingArrowBack->currentIteration++;
		}
		else
		{
			easingArrowBack->currentIteration = 0;
			easingArrowBack->easingsActivated = false;
			easingArrow->easingsActivated = true;
		}
	}
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------
	if (playerEasing->easingsActivated && currPlayer != nullptr)
	{
		playerEasing->initialPos = currPlayer->bounds.x - 40;
		playerPos = playerEasing->circularEaseInOut(playerEasing->currentIteration, playerEasing->initialPos, playerEasing->deltaPos, playerEasing->totalIterations);

		if (playerEasing->currentIteration < playerEasing->totalIterations)
		{
			playerEasing->currentIteration++;
		}
		else
		{
			playerEasing->currentIteration = 0;
			playerEasing->easingsActivated = false;
			playerEasing2->easingsActivated = true;
		}
	}

	if (playerEasing2->easingsActivated && currPlayer != nullptr)
	{
		playerEasing2->initialPos = playerEasing->initialPos + playerEasing->deltaPos;
		playerPos = playerEasing2->circularEaseInOut(playerEasing2->currentIteration, playerEasing2->initialPos, playerEasing2->deltaPos, playerEasing2->totalIterations);

		if (playerEasing2->currentIteration < playerEasing2->totalIterations)
		{
			playerEasing2->currentIteration++;
		}
		else
		{
			playerEasing2->currentIteration = 0;
			playerEasing2->easingsActivated = false;
			playerEasing->easingsActivated = true;
		}
	}

	if (enemyEasing->easingsActivated && currEnemy != nullptr)
	{
		enemyEasing->initialPos = currEnemy->bounds.x + 150;
		enemyPos = enemyEasing->circularEaseInOut(enemyEasing->currentIteration, enemyEasing->initialPos, enemyEasing->deltaPos, enemyEasing->totalIterations);

		if (enemyEasing->currentIteration < enemyEasing->totalIterations)
		{
			enemyEasing->currentIteration++;
		}
		else
		{
			enemyEasing->currentIteration = 0;
			enemyEasing->easingsActivated = false;
			enemyEasing2->easingsActivated = true;
		}
	}

	if (enemyEasing2->easingsActivated && currEnemy != nullptr)
	{
		enemyEasing2->initialPos = enemyEasing->initialPos + enemyEasing->deltaPos;
		enemyPos = enemyEasing2->circularEaseInOut(enemyEasing2->currentIteration, enemyEasing2->initialPos, enemyEasing2->deltaPos, enemyEasing2->totalIterations);

		if (enemyEasing2->currentIteration < enemyEasing2->totalIterations)
		{
			enemyEasing2->currentIteration++;
		}
		else
		{
			enemyEasing2->currentIteration = 0;
			enemyEasing2->easingsActivated = false;
			enemyEasing->easingsActivated = true;
		}
	}

	return ret;
}

void BattleMenu::Draw(Font* font, bool showColliders)
{
	SDL_Rect section = { 204,0,1076,296 };
	app->render->DrawTexture(guiTex, 96, 420, &section);

	SDL_Rect gauntletPlayers = { 561, 322, 29, 25 };
	SDL_Rect gauntletEnemies = { 642, 321, 29, 25 };

	//AROCA BORRA ESTAS 4 LINEAS CUANDO ARREGLES LO DE LOS EASINGS Y DESCOMENTES LO DE MAS ABAJO
	btnAttack->Draw(app->render, showColliders, 25, { 255,255,255,225 });
	btnAbility->Draw(app->render, showColliders, 25, { 255,255,255,225 });
	btnDefense->Draw(app->render, showColliders, 25, { 255,255,255,225 });
	btnObject->Draw(app->render, showColliders, 25, { 255,255,255,225 });

	switch (type)
	{
	case BattleState::NONE:
		break;
	case BattleState::DEFAULT:
		app->render->DrawTexture(guiTex, playerPos, currPlayer->bounds.y, &gauntletPlayers);
		/*if (btnAttack->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnAttack->bounds.x = position_x;
			btnAttack->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnAttack->bounds.x = 20;
			btnAttack->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		}
		if (btnAbility->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnAbility->bounds.x = position_x;
			btnAbility->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnAbility->bounds.x = 20;
			btnAbility->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		}

		if (btnDefense->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnDefense->bounds.x = position_x;
			btnDefense->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnDefense->bounds.x = 20;
			btnDefense->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		}

		if (btnObject->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnObject->bounds.x = position_x;
			btnObject->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnObject->bounds.x = 20;
			btnObject->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		}*/
		break;
	case BattleState::ATTACK:
		app->render->DrawTexture(guiTex, enemyPos, currEnemy->bounds.y - 20, &gauntletEnemies);
		app->render->DrawTexture(guiTex, playerPos, currPlayer->bounds.y, &gauntletPlayers);
		break;
	case BattleState::ABILITY_SELECT:
		/*if (btnAbilitySlot1->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnAbilitySlot1->bounds.x = position_x;
			btnAbilitySlot1->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnAbilitySlot1->bounds.x = 20;*/
			btnAbilitySlot1->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		/*}
		if (btnAbilitySlot2->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnAbilitySlot2->bounds.x = position_x2;
			btnAbilitySlot2->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnAbilitySlot2->bounds.x = 200;*/
			btnAbilitySlot2->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		/*}
		if (btnAbilitySlot3->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnAbilitySlot3->bounds.x = position_x;
			btnAbilitySlot3->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnAbilitySlot3->bounds.x = 20;*/
			btnAbilitySlot3->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		/*}
		if (btnAbilitySlot4->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnAbilitySlot4->bounds.x = position_x2;
			btnAbilitySlot4->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnAbilitySlot4->bounds.x = 200;*/
			btnAbilitySlot4->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		/*}*/
		app->render->DrawTexture(guiTex, playerPos, currPlayer->bounds.y, &gauntletPlayers);
		break;
	case BattleState::ABILITY:
		app->render->DrawTexture(guiTex, enemyPos, currEnemy->bounds.y - 20, &gauntletEnemies);
		app->render->DrawTexture(guiTex, playerPos, currPlayer->bounds.y, &gauntletPlayers);
		break;
	case BattleState::ENEMY_TURN:

		break;
	case BattleState::DEFENSE:
		
		break;
	case BattleState::OBJECT_SELECT:
		/*if (btnObjectSlot1->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnObjectSlot1->bounds.x = position_x;
			btnObjectSlot1->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnObjectSlot1->bounds.x = 20;*/
			btnObjectSlot1->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		/*}
		if (btnObjectSlot2->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnObjectSlot2->bounds.x = position_x2;
			btnObjectSlot2->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnObjectSlot2->bounds.x = 200;*/
			btnObjectSlot2->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		/*}
		if (btnObjectSlot3->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnObjectSlot3->bounds.x = position_x;
			btnObjectSlot3->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnObjectSlot3->bounds.x = 20;*/
			btnObjectSlot3->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		/*}
		if (btnObjectSlot4->state == GuiControlState::FOCUSED)
		{
			if (easingArrowBack->easingsActivated == false) easingArrow->easingsActivated = true;
			btnObjectSlot4->bounds.x = position_x2;
			btnObjectSlot4->Draw(app->render, showColliders, 25, { alpha,0,0,225 });
		}
		else
		{
			btnObjectSlot4->bounds.x = 200;*/
			btnObjectSlot4->Draw(app->render, showColliders, 25, { 255,255,255,225 });
		/*}*/

		app->render->DrawTexture(guiTex, playerPos, currPlayer->bounds.y, &gauntletPlayers);
		break;
	case BattleState::OBJECT:
		app->render->DrawTexture(guiTex, playerPos, currPlayer->bounds.y, &gauntletPlayers);
		break;
	}

	if (type != BattleState::NONE) DrawStats(font);
}

bool BattleMenu::UnLoad()
{
	app->tex->UnLoad(guiTex);

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

	RELEASE(easingArrow);
	RELEASE(easingArrowBack);
	RELEASE(playerEasing);
	RELEASE(playerEasing2);
	RELEASE(enemyEasing);
	RELEASE(enemyEasing2);

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
		else if (control->id == 2)
		{
			type = BattleState::ABILITY_SELECT;
			AbilityStateButtons();
		}
		else if (control->id == 3) type = BattleState::DEFENSE;
		else if (control->id == 4)
		{
			type = BattleState::OBJECT_SELECT;
			ObjectStateButtons();
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
			tempPlayer->SetObjectSelected(1);
		}
		else if (control->id == 10)
		{
			type = BattleState::OBJECT;
			tempPlayer->SetObjectSelected(2);
		}
		else if (control->id == 11)
		{
			type = BattleState::OBJECT;
			tempPlayer->SetObjectSelected(3);
		}
		else if (control->id == 12)
		{
			type = BattleState::OBJECT;
			tempPlayer->SetObjectSelected(4);
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
		app->render->DrawText(font, (*it)->GetName().c_str(), 746, 465+(i*55), 25, 2, { 0,0,0 });
		app->render->DrawText(font, (*it)->GetName().c_str(), 744, 463+(i*55), 25, 2, { 255, 255, 255 });
		app->render->DrawText(font, "HP", 892, 465 + (i * 55), 25, 2, { 0,0,0 });
		app->render->DrawText(font, "HP", 890, 463 + (i * 55), 25, 2, { 102, 230, 151 });
		std::string points = std::to_string((*it)->GetHealthPoints());
		app->render->DrawText(font, points.c_str(), 924, 465 + (i * 55), 25, 2, { 0,0,0 });
		app->render->DrawText(font, points.c_str(), 922, 463 + (i * 55), 25, 2, { 255, 255, 255 });
		points = std::to_string((*it)->GetManaPoints());
		app->render->DrawText(font, "MP", 1024, 465 + (i * 55), 25, 2, { 0,0,0 });
		app->render->DrawText(font, "MP", 1022, 463 + (i * 55), 25, 2, { 43, 142, 226 });
		app->render->DrawText(font, points.c_str(), 1054, 465 + (i * 55), 25, 2, { 0,0,0 });
		app->render->DrawText(font, points.c_str(), 1052, 463 + (i * 55), 25, 2, { 255, 255, 255 });

		SDL_Rect statsBar = { 124, 357, 367 , 9 };
		app->render->DrawTexture(guiTex, 747, 489 + (i * 55), &statsBar);
		app->render->DrawTexture(guiTex, 747, 504 + (i * 55), &statsBar);
		
		int currHealth = (*it)->GetHealthPoints();
		int maxHealth = (*it)->GetMaxHealthPoints();
		if (currHealth < (maxHealth / 4))
			statsBar = { 127, 333, ((361 * currHealth) / maxHealth), 5 };
		else if (currHealth < (maxHealth / 2))
			statsBar = { 127, 322, ((361 * currHealth) / maxHealth), 5 };
		else
			statsBar = { 127, 347, ((361 * currHealth) / maxHealth), 5 };

		app->render->DrawTexture(guiTex, 750, 491 + (i * 55), &statsBar);

		
		statsBar = { 127, 312, ((361 * (*it)->GetManaPoints()) / (*it)->GetMaxManaPoints()), 5 };
		app->render->DrawTexture(guiTex, 750, 506 + (i * 55), &statsBar);
	}
	
	if (type != BattleState::ENEMY_TURN)
	{
		SDL_Rect rectangle = { 124, 406, 191, 55 };
		app->render->DrawRectangle(rectangle, 68, 67, 109, 200);
		app->render->DrawText(font, currPlayer->GetName().c_str(), 135, 423, 25, 2, { 255, 255, 255 });
	}
}

bool BattleMenu::HandleInput(Input* input)
{
	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
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
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
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
	else if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP)
	{
		currPlayer->Attack(currEnemy);
		
		type = BattleState::ATTACKING;
	}

	if (currPlayer->stance == PlayerStance::ATTACK_FINISHED && type != BattleState::ENEMY_TURN)
	{
		eastl::list<Player*>::iterator it = sceneBattle->playerList.begin();
		for (; it != sceneBattle->playerList.end(); ++it)
		{
			if ((*it) == currPlayer)
			{
				tempPlayer = currPlayer;
				currPlayer->stance = PlayerStance::BATTLE;
				eastl::list<Player*>::iterator item = it;
				for (; item != sceneBattle->playerList.end(); ++item)
				{
					if (item.next() == sceneBattle->playerList.end())
					{
						currPlayer = (*sceneBattle->playerList.begin());
						if (sceneBattle->enemyList.size() != 0) type = BattleState::ENEMY_TURN;
						break;
					}
					else if ((*item.next())->GetHealthPoints() != 0)
					{
						currPlayer = (*item.next());
						type = BattleState::DEFAULT;
						DefaultStateButtons();
						break;
					}
				}
				break;
			}
		}
	}
	return true;
}

bool BattleMenu::HandleAbilities(Input* input, int currentAbility)
{
	int prevX = xMouse;
	int prevY = yMouse;
	input->GetMousePosition(xMouse, yMouse);
	if (prevX != xMouse || prevY != yMouse)
	{
		lastUserInput = 1;
		SDL_ShowCursor(SDL_ENABLE);
	}
	else
	{
		lastUserInput = 0;
	}

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
	{
		if (currentButton == nullptr)
		{
			currentButton = (*buttons.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
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
	}
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
	{
		if (currentButton == nullptr)
		{
			currentButton = (*buttons.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
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
	}
	else if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP)
	{
		currPlayer->Ability(currEnemy, currentAbility);

		type = BattleState::ATTACKING;

		if (sceneBattle->enemyList.size() != 0)
		{
			if (currPlayer == (*sceneBattle->playerList.end().prev()))
			{
				currPlayer->stance = PlayerStance::BATTLE;
				tempPlayer = currPlayer;
				currPlayer = (*sceneBattle->playerList.begin());
				type = BattleState::ENEMY_TURN;
			}
		}
	}

	if (type != BattleState::ENEMY_TURN && currPlayer->stance == PlayerStance::ABILITY_FINISHED)
	{
		eastl::list<Player*>::iterator it = sceneBattle->playerList.begin();
		for (int i = 0; it != sceneBattle->playerList.end(); ++it, ++i)
		{
			if ((*it) == currPlayer)
			{
				tempPlayer = currPlayer;
				currPlayer->stance = PlayerStance::BATTLE;
				eastl::list<Player*>::iterator item = it;
				for (; item != sceneBattle->playerList.end(); ++item)
				{
					if (item.next() == sceneBattle->playerList.end())
					{
						currPlayer = (*sceneBattle->playerList.begin());
						if (sceneBattle->enemyList.size() != 0) type = BattleState::ENEMY_TURN;
						break;
					}
					else if ((*item.next())->GetHealthPoints() != 0)
					{
						currPlayer = (*item.next());
						type = BattleState::DEFAULT;
						DefaultStateButtons();
						break;
					}
				}
				break;
			}
		}
	}
	return true;
}

bool BattleMenu::HandleObjects(Input* input, int currentObject)
{
	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
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
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
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
	else if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP)
	{
		tempPlayer->UseObject(currPlayer, tempPlayer->GetObjectSelected());

		currPlayer = tempPlayer;
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
				tempPlayer = currPlayer;
				currPlayer->stance = PlayerStance::BATTLE;
				eastl::list<Player*>::iterator item = it;
				for (; item != sceneBattle->playerList.end(); ++item)
				{
					if (item.next() == sceneBattle->playerList.end())
					{
						currPlayer = (*sceneBattle->playerList.begin());
						if (sceneBattle->enemyList.size() != 0) type = BattleState::ENEMY_TURN;
						break;
					}
					else if ((*item.next())->GetHealthPoints() != 0)
					{
						currPlayer = (*item.next());
						type = BattleState::DEFAULT;
						DefaultStateButtons();
						break;
					}
				}
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
			tempPlayer = currPlayer;
			currPlayer = (*sceneBattle->playerList.begin());
			type = BattleState::ENEMY_TURN;
			break;
		}
		else if ((*it) == currPlayer)
		{
			tempPlayer = currPlayer;
			currPlayer->stance = PlayerStance::BATTLE;
			eastl::list<Player*>::iterator item = it;
			for (; item != sceneBattle->playerList.end(); ++item)
			{
				if (item.next() == sceneBattle->playerList.end())
				{
					currPlayer = (*sceneBattle->playerList.begin());
					if (sceneBattle->enemyList.size() != 0) type = BattleState::ENEMY_TURN;
					break;
				}
				else if ((*item.next())->GetHealthPoints() != 0)
				{
					currPlayer = (*item.next());
					type = BattleState::DEFAULT;
					DefaultStateButtons();
					break;
				}
			}
			break;
		}
	}
	return true;
}

void BattleMenu::EraseEnemy()
{
	eastl::list<Enemy*>::iterator it = sceneBattle->enemyList.begin();
	for (; it != sceneBattle->enemyList.end(); ++it)
	{
		if ((*it)->GetHealth() <= 0)
		{
			enemyKilled = true;
			if ((*it)->AnimationFinished())
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
}

void BattleMenu::EnemyTurn()
{
	eastl::list<Player*>::iterator pIt;
	while (playerSelected == false)
	{
		int randNum = rand() % sceneBattle->playerList.size();
		pIt = sceneBattle->playerList.begin();
		for (int i = 0; i < randNum; ++i)
		{
			++pIt;
		}
		if ((*pIt)->GetHealthPoints() > 0) playerSelected = true;
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
			if (playerSelected) playerSelected = false;
			(*eIt)->SetCurrentState(EnemyState::NONE);
			break;
		}
		else if ((*eIt)->GetCurrentState() == EnemyState::NONE)
		{
			num++;
		}
	}

	if (num == sceneBattle->enemyList.size())
	{
		bool asigned = false;
		eastl::list<Player*>::iterator itPlayer = sceneBattle->playerList.begin();
		for (; itPlayer != sceneBattle->playerList.end(); ++itPlayer)
		{
			if ((*itPlayer)->GetHealthPoints() > 0 && asigned == false)
			{
				currPlayer = (*itPlayer);
				asigned = true;
			}
			(*itPlayer)->SetDefend(false);
			(*itPlayer)->stance = PlayerStance::BATTLE;
		}
		type = BattleState::DEFAULT;
		DefaultStateButtons();

		eastl::list<Enemy*>::iterator eIt = sceneBattle->enemyList.begin();
		for (; eIt != sceneBattle->enemyList.end(); ++eIt)
		{
			(*eIt)->SetCurrentState(EnemyState::NORMAL);
		}
		playerSelected = false;
	}
}

void BattleMenu::DefaultStateButtons()
{
	buttons.clear();
	buttons.push_back(btnAttack);
	buttons.push_back(btnAbility);
	buttons.push_back(btnDefense);
	buttons.push_back(btnObject);

	lastButton = currentButton;
	currentButton = (*buttons.begin());
}

void BattleMenu::AbilityStateButtons()
{
	buttons.clear();
	buttons.push_back(btnAbilitySlot1);
	buttons.push_back(btnAbilitySlot2);
	buttons.push_back(btnAbilitySlot3);
	buttons.push_back(btnAbilitySlot4);

	lastButton = currentButton;
	currentButton = (*buttons.begin());
}

void BattleMenu::ObjectStateButtons()
{
	buttons.clear();
	buttons.push_back(btnObjectSlot1);
	buttons.push_back(btnObjectSlot2);
	buttons.push_back(btnObjectSlot3);
	buttons.push_back(btnObjectSlot4);

	lastButton = currentButton;
	currentButton = (*buttons.begin());
}

void BattleMenu::UpdatingButtons(Input* input)
{
	int prevX = xMouse;
	int prevY = yMouse;
	input->GetMousePosition(xMouse, yMouse);
	if (prevX != xMouse || prevY != yMouse)
	{
		lastUserInput = 1;
		SDL_ShowCursor(SDL_ENABLE);
	}
	else
	{
		lastUserInput = 0;
	}

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
	{
		if (currentButton == nullptr)
		{
			currentButton = (*buttons.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
		{
			eastl::list<GuiButton*>::iterator it = buttons.begin();
			for (int i = 0; i < buttons.size(); ++i, ++it)
			{
				if ((*it)->id == currentButton->id + 1)
				{
					currentButton = (*it);
					break;
				}
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
	{
		if (currentButton == nullptr)
		{
			currentButton = (*buttons.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else
		{
			eastl::list<GuiButton*>::iterator it = buttons.begin();
			for (int i = 0; i < buttons.size(); ++i, ++it)
			{
				if ((*it)->id == currentButton->id - 1)
				{
					currentButton = (*it);
					break;
				}
			}
		}
	}
}