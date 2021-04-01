#include "App.h"
#include "Render.h"

#include "Skull.h"


Skull::Skull(iPoint pos) : Enemy(EntityType::SKULL)
{
	bounds = { pos.x, pos.y, 16, 32 };
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
