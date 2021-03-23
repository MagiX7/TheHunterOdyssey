#include "App.h"
#include "Render.h"

#include "Npc.h"

Npc::Npc(EntityType type, NpcType Type, iPoint position) : Entity(type)
{
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 32;
	bounds.h = 32;
	npcType = Type;
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
	pugi::xml_node auxilar = node.append_child("NPC");
	pugi::xml_node auxiliar1 = auxilar.append_child("bounds");
	auxiliar1.append_attribute("X").set_value(bounds.x);
	auxiliar1.append_attribute("Y").set_value(bounds.y);
	auxiliar1.append_attribute("W").set_value(bounds.w);
	auxiliar1.append_attribute("H").set_value(bounds.h);

	auxiliar1 = auxilar.append_child("NpcType");
	const char* typeName = "/";

	if (npcType == NpcType::TABERN) { typeName = "TABERN"; }
	else if (npcType == NpcType::TOWN) { typeName = "TOWN"; }
	else if (npcType == NpcType::WIZARD) { typeName = "WIZARD"; }
	else if (npcType == NpcType::NONE) { typeName = "NONE"; }

	auxiliar1.append_attribute("type").set_value(typeName);
	return true;
}