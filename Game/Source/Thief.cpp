#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Thief.h"

Thief::Thief(iPoint position) : Player(PlayerType::THIEF, EntityType::THIEF,position)
{
	stance = PlayerStance::ROAMING;
	healthPoints = 1500;
	manaPoints = 500;
	name = "Thief";
}

Thief::~Thief()
{
}

bool Thief::Load()
{
	texture = app->tex->Load("Assets/Textures/");

	return true;
}

bool Thief::Update(float dt)
{
	HandleInput(dt);

	return true;
}

void Thief::Draw(bool showColliders)
{
	if (showColliders) app->render->DrawRectangle(bounds, 255,165,0);
	SDL_Rect textureRect = { 23, 14, 51,81 };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &textureRect);
}

bool Thief::UnLoad()
{
	return true;
}

void Thief::HandleInput(float dt)
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

bool Thief::SaveState(pugi::xml_node& node)
{
	Player::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("playerType");

	auxiliar1.append_attribute("type").set_value("THIEF");
	return true;
}

