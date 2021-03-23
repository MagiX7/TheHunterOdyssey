#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Player.h"

#define SPEED_X 200.0f
#define SPEED_Y 200.0f

Player::Player(PlayerType pType) : Entity(EntityType::PLAYER), type(pType)
{
}

Player::~Player()
{
}

bool Player::Load()
{
	bool ret = true;

	bounds = { 0, 0, 16, 32 };

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	HandleInput(dt);

	return ret;
}

void Player::Draw(bool showColliders)
{
	switch (type)
	{
	case PlayerType::HUNTER:
		if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
		break;
	case PlayerType::WIZARD:
		if (showColliders) app->render->DrawRectangle(bounds, 0, 255, 0);
		break;
	}
}

bool Player::SaveState(pugi::xml_node& node) 
{
	pugi::xml_node auxiliar1 = node.append_child("bounds");
	auxiliar1.append_attribute("X").set_value(bounds.x);
	auxiliar1.append_attribute("Y").set_value(bounds.y);
	auxiliar1.append_attribute("W").set_value(bounds.w);
	auxiliar1.append_attribute("H").set_value(bounds.h);

	auxiliar1=node.append_child("playerType");
	const char* typeName="/";

	if (type == PlayerType::HUNTER) {typeName = "HUNTER";}
	else if(type == PlayerType::WIZARD){typeName = "WIZARD";}
	else if(type == PlayerType::NONE) { typeName = "NONE"; }

	auxiliar1.append_attribute("type").set_value(typeName);
	return true;
}

void Player::setPlayerType(PlayerType Type) 
{
	type = Type;
}
void Player::HandleInput(float dt)
{
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
}