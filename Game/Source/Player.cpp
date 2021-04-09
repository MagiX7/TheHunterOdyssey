#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Player.h"

#define SPEED_X 200.0f
#define SPEED_Y 200.0f

Player::Player(PlayerType t, EntityType type, iPoint position) : Entity(type), playerType(t)
{
	bounds.x= position.x;
	bounds.y = position.y;
	bounds.w = 28;
	bounds.h = 28;
	currentAbilitySelected = 0;
}

Player::~Player()
{
}

bool Player::Load()
{
	bool ret = true;

	//bounds = { 0, 0, 16, 32 };

	return ret;
}

bool Player::Update(float dt)
{
	bool ret = true;

	GamePad& pad = app->input->pads[0];

	HandleInput(dt, pad);

	return ret;
}

void Player::Draw(bool showColliders)
{
	switch (playerType)
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
	return true;
}

void Player::Attack(Enemy* enemy)
{
}

void Player::Ability(Enemy* enemy, int currentAbility)
{
}


void Player::UseObject(Player* player, int currentObject)
{
}

void Player::GetDamage(int dmg)
{
}

void Player::GetHealed(int heal)
{
}

void Player::SetDefend(bool option)
{
}

void Player::HandleInput(float dt, GamePad& pad)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || pad.up == true)
	{
		bounds.y -= SPEED_Y * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || pad.down == true)
	{
		bounds.y += SPEED_Y * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || pad.left == true)
	{
		bounds.x -= SPEED_X * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || pad.right == true)
	{
		bounds.x += SPEED_X * dt;
	}
}