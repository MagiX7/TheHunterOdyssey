#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Skull.h"


Skull::Skull(iPoint pos) : Enemy(EntityType::SKULL)
{
	bounds = { pos.x, pos.y, 29, 29 };
	texture = app->tex->Load("Assets/Textures/Enemies/floating_skull.png");
	name = "Skull";

	health = 100.0f;
	mana = 50.0f;
	damage = 20.0f;
	defense = 20.0f;
	speed = 10.0f;
}

Skull::~Skull()
{
}

bool Skull::Load()
{


	return true;
}

bool Skull::Update(float dt)
{
	return true;
}

bool Skull::CheckCollisions()
{
	return true;
}

void Skull::Draw(bool showColliders)
{
	app->render->DrawRectangle(bounds, 0, 0, 255, 255);

	SDL_Rect rect = { 5,7,bounds.w,bounds.h };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
}

bool Skull::UnLoad()
{
	return true;
}

bool Skull::LoadState(pugi::xml_node& node)
{
	return true;
}

bool Skull::SaveState(pugi::xml_node& node)
{
	return true;
}
