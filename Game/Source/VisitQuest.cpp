#include "Render.h"

#include "VisitQuest.h"

VisitQuest::VisitQuest(pugi::xml_node n)
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->requiredId = n.attribute("requiredId").as_int();
	this->isCompleted = n.attribute("isCompleted").as_bool();
	this->mapName = n.attribute("mapName").as_string();
}

VisitQuest::~VisitQuest()
{
}

bool VisitQuest::Update(Entity* entity, SString n)
{
	if ((n != nullptr) && (mapName == n)) isCompleted = true;
	
	return isCompleted;
}

void VisitQuest::Draw(Render* render, Font* font, bool showMore)
{
	render->DrawText(font, name.c_str(), 10, 10, 24, 2, { 255, 255, 255 });
}