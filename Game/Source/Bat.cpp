#include "App.h"
#include "Render.h"

#include "Bat.h"


Bat::Bat(iPoint pos) : Enemy(EntityType::SKULL)
{
	bounds = { pos.x, pos.y, 16, 32 };
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
