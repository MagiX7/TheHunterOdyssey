#include "App.h"
#include "Render.h"
#include "TalkQuest.h"

#include "Entity.h"

TalkQuest::TalkQuest(pugi::xml_node n)
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->requiredId = n.attribute("requiredId").as_int();
	this->isCompleted = n.attribute("isCompleted").as_bool();
	this->type = (EntityType)n.attribute("entityType").as_int();
}

TalkQuest::~TalkQuest()
{
}

bool TalkQuest::Update(EntityType t, SString n)
{
	if (t == type) isCompleted = true;
	
	return isCompleted;
}

void TalkQuest::Draw(Font* font)
{
	app->render->DrawText(font, name.c_str(), 10, 10, 30, 4, { 255, 255, 255 });
}