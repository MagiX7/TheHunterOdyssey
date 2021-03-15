#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Player.h"

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

	return ret;
}

void Player::Draw(bool showColliders)
{
	if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
}