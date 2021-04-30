#include "App.h"
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

bool VisitQuest::Update(EntityType t, SString n)
{
	if (mapName == n) isCompleted = true;
	
	return isCompleted;
}

void VisitQuest::Draw(Font* font)
{
	app->render->DrawText(font, name.c_str(), 10, 10, 30, 4, { 255, 255, 255 });
}