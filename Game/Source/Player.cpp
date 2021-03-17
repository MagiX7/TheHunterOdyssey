#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Player.h"

#define SPEED_X 200.0f
#define SPEED_Y 200.0f

Player::Player() : Entity(EntityType::PLAYER)
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
	if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
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