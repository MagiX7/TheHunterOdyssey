#include "App.h"
#include "Render.h"

#include "Npc.h"

Npc::Npc(EntityType type, iPoint position) : Entity(type)
{
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 32;
	bounds.h = 32;
}

Npc::~Npc()
{
}

bool Npc::Load()
{
	//bounds = { 100,100,32,32 };

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
bool Npc::SaveState(pugi::xml_node& node)
{

	pugi::xml_node auxiliar1 = node.append_child("bounds");
	auxiliar1.append_attribute("X").set_value(bounds.x);
	auxiliar1.append_attribute("Y").set_value(bounds.y);
	auxiliar1.append_attribute("W").set_value(bounds.w);
	auxiliar1.append_attribute("H").set_value(bounds.h);

	
	return true;
}