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
	bounds.w = 26;
	bounds.h = 32;
	currentAbilitySelected = 0;
	gold = 500;
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

	HandleInput(dt);

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

void Player::GetMana(int amount)
{
}

void Player::SetDefend(bool option)
{
}

void Player::HandleInput(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || app->input->pad->l_y < -0.5)
	{
		bounds.y -= SPEED_Y * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || app->input->pad->l_y > 0.5)
	{
		bounds.y += SPEED_Y * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || app->input->pad->l_x < -0.5)
	{
		bounds.x -= SPEED_X * dt;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || app->input->pad->l_x > 0.5)
	{
		bounds.x += SPEED_X * dt;
	}
}