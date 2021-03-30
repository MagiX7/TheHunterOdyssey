#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Warrior.h"

Warrior::Warrior(iPoint position) : Player(PlayerType::WARRIOR, EntityType::WARRIOR,position)
{
	stance = PlayerStance::ROAMING;
	healthPoints = 3500;
	manaPoints = 500;
	name = "Warrior";
}

Warrior::~Warrior()
{
}

bool Warrior::Load()
{
	texture = app->tex->Load("Assets/Textures/");

	return true;
}

bool Warrior::Update(float dt)
{
	HandleInput(dt);

	return true;
}

void Warrior::Draw(bool showColliders)
{
	if (showColliders) app->render->DrawRectangle(bounds, 0, 0, 255);
	SDL_Rect textureRect = { 23, 14, 51,81 };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &textureRect);
}

bool Warrior::UnLoad()
{
	return true;
}

void Warrior::HandleInput(float dt)
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

bool Warrior::SaveState(pugi::xml_node& node)
{
	Player::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("playerType");

	auxiliar1.append_attribute("type").set_value("WARRIOR");
	return true;
}

