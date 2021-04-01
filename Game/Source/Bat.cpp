#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Bat.h"


Bat::Bat(iPoint pos) : Enemy(EntityType::BAT)
{
	bounds = { pos.x, pos.y, 37, 33 };
	texture = app->tex->Load("Assets/Textures/Enemies/flying_eye.png");
	name = "Bat";

	health = 100.0f;
	mana = 50.0f;
	damage = 20.0f;
	defense = 20.0f;
	speed = 10.0f;
}

Bat::~Bat()
{
}

bool Bat::Load()
{


	return true;
}

bool Bat::Update(float dt)
{
	return true;
}

bool Bat::CheckCollisions()
{
	return true;
}

void Bat::Draw(bool showColliders)
{
	app->render->DrawRectangle(bounds, 0, 0, 255, 255);
	SDL_Rect rect = { 4,5,bounds.w,bounds.h };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
}

bool Bat::UnLoad()
{
	return true;
}

bool Bat::LoadState(pugi::xml_node& node)
{
	return true;
}

bool Bat::SaveState(pugi::xml_node& node)
{
	return true;
}
