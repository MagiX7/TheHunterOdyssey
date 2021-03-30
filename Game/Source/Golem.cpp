#include "App.h"
#include "Render.h"

#include "Golem.h"


Golem::Golem(iPoint pos)
{
	bounds = { pos.x, pos.y, 16, 32 };
}

Golem::~Golem()
{
}

bool Golem::Load()
{
	

	return true;
}

bool Golem::Update(float dt)
{
	return true;
}

bool Golem::CheckCollisions()
{
	return true;
}

void Golem::Draw(bool showColliders)
{
	app->render->DrawRectangle(bounds, 0, 0, 255, 255);
}

bool Golem::UnLoad()
{
	return true;
}

bool Golem::LoadState(pugi::xml_node& node)
{
	return true;
}

bool Golem::SaveState(pugi::xml_node& node)
{
	return true;
}
