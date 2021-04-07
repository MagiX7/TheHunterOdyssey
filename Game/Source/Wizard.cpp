#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiButton.h"

#include "Wizard.h"
#include "Enemy.h"

Wizard::Wizard(iPoint position) : Player(PlayerType::WIZARD, EntityType::HUNTER, position)
{
	//bounds = { 0,0, 16,32 };
	stance = PlayerStance::ROAMING;
	healthPoints = 1000;
	defense = 5;
	manaPoints = 5000;
	meleeDamage = 1;
	magicDamage = 5;
	name = "Wizard";
}

Wizard::~Wizard()
{
}

bool Wizard::Load()
{
	// TODO implement textures for wizard. 
	//app->tex->Load();
	return true;
}

bool Wizard::Update(float dt)
{
	HandleInput(dt);

	return true;
}

void Wizard::Draw(bool showColliders)
{
	if (showColliders) app->render->DrawRectangle(bounds, 0, 255, 0);
}

bool Wizard::UnLoad()
{
	return true;
}

void Wizard::HandleInput(float dt)
{
	switch (stance)
	{
	case PlayerStance::ROAMING:
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			bounds.y -= SPEED_Y * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			bounds.y += SPEED_Y * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			bounds.x -= SPEED_X * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			bounds.x += SPEED_X * dt;
		}
		break;
	}
}

bool Wizard::SaveState(pugi::xml_node& node)
{
	Player::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("playerType");

	auxiliar1.append_attribute("type").set_value("WIZARD");

	return true;
}

void Wizard::Attack(Enemy* enemy)
{
	enemy->GetDamage(meleeDamage);
}

void Wizard::GetDamage(int dmg)
{
	healthPoints -= dmg * dmg / (dmg + defense);
}

void Wizard::Ability(Enemy* enemy, int currentAbility)
{
	switch (currentAbility)
	{
	case 1:
		enemy->GetDamage(magicDamage + 10);
		LOG("Casting FIRE");
		break;
	case 2:
		enemy->GetDamage(magicDamage);
		LOG("Casting WATER");
		break;
	case 3:
		enemy->GetDamage(magicDamage);
		LOG("Casting THUNDER");
		break;
	case 4:
		enemy->GetDamage(magicDamage);
		LOG("Casting GRAVITY");
		break;
	}
}

void Wizard::UseObject(int currentObject)
{
	switch (currentObject)
	{
	case 1:
		// TODO Object functionality
		break;
	case 2:
		// TODO Object functionality
		break;
	case 3:
		// TODO Object functionality
		break;
	}
}