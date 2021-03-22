#include "App.h"
#include "Render.h"

#include "Npc.h"

Npc::Npc(EntityType type) : Entity(type)
{
}

Npc::~Npc()
{
}

bool Npc::Load()
{
	bounds = { 100,100,32,32 };

	return true;
}

bool Npc::Update(float dt)
{

	return true;
}

void Npc::Draw(bool showColliders)
{
	if(showColliders)
		app->render->DrawRectangle(bounds, 255, 255, 255, 255);

}

bool Npc::UnLoad()
{
	return true;
}