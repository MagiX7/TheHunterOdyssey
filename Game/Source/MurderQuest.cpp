#include "Render.h"

#include "MurderQuest.h"

MurderQuest::MurderQuest(pugi::xml_node n) : Quest()
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->requiredId = n.attribute("requiredId").as_int();
	this->isCompleted = n.attribute("isCompleted").as_bool();
	this->type = (EntityType)n.attribute("enemy").as_int();
	this->total = n.attribute("total").as_int();
	this->actual = n.attribute("actual").as_int();
}

MurderQuest::~MurderQuest()
{
}

bool MurderQuest::Update(Entity* entity, SString n)
{
	if ((entity != nullptr) && (type == entity->type)) isCompleted = true;

	return isCompleted;
}

void MurderQuest::Draw(Render* render, Font* font, bool showMore)
{
	render->DrawText(font, name.c_str(), 10, 10, 24, 2, { 255, 255, 255 });
}