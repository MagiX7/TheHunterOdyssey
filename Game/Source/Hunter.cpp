#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Hunter.h"

Hunter::Hunter(iPoint position) : Player(PlayerType::HUNTER, EntityType::HUNTER, position)
{
	//bounds = { 0,0, 16,32 };
	stance = PlayerStance::ROAMING;
	healthPoints = 2500;
	manaPoints = 1000;
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
