#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Hunter.h"
#include "Enemy.h"

Hunter::Hunter(iPoint position) : Player(PlayerType::HUNTER, EntityType::HUNTER, position)
{
	//bounds = { 0,0, 16,32 };
	stance = PlayerStance::ROAMING;
	healthPoints = 2500;
	manaPoints = 1000;
	defense = 15;
	meleeDamage = 75;
	magicDamage = 10;
	name = "Hunter";
}

Hunter::~Hunter()
{
}

bool Hunter::Load()
{
	texture = app->tex->Load("Assets/Textures/hunter_spritesheet.png");

	return true;
}

bool Hunter::Update(float dt)
{
	HandleInput(dt);

	return true;
}

void Hunter::Draw(bool showColliders)
{
	if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
	SDL_Rect textureRect = { 23, 14, 51,81 };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &textureRect);
}

bool Hunter::UnLoad()
{
	return true;
}

void Hunter::HandleInput(float dt)
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

bool Hunter::SaveState(pugi::xml_node& node)
{
	Player::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("playerType");

	auxiliar1.append_attribute("type").set_value("HUNTER");
	return true;
}

void Hunter::Attack(Enemy* enemy)
{
	enemy->GetDamage(meleeDamage);
}

void Hunter::GetDamage(int dmg)
{
	healthPoints -= dmg * dmg / (dmg + defense);
}

void Hunter::Ability(Enemy* enemy, int currentAbility)
{
	switch (currentAbility)
	{
	case 1:
		enemy->GetDamage(magicDamage + 10);
		LOG("Casting STEADY SHOT");
		break;
	case 2:
		enemy->GetDamage(magicDamage);
		LOG("Casting SERPENT SHOT");
		break;
	case 3:
		enemy->GetDamage(magicDamage + meleeDamage + 50);
		LOG("Casting EXPLOSIVE SHOT");
		break;
	case 4:
		enemy->GetDamage(magicDamage);
		LOG("Casting BOLA");
		break;
	}
}

void Hunter::UseObject(int currentObject)
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
