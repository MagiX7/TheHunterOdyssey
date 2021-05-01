#include "Render.h"
#include "TalkQuest.h"

#include "Entity.h"

TalkQuest::TalkQuest(pugi::xml_node n)
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->requiredId = n.attribute("requiredId").as_int();
	this->isCompleted = n.attribute("isCompleted").as_bool();
	this->npcId = n.attribute("npcId").as_int();
	this->type = (EntityType)n.attribute("entityType").as_int();
}

TalkQuest::~TalkQuest()
{
}


bool TalkQuest::Update(Entity* entity, SString name)
{
	if (npcId == entity->GetDialogeId()) isCompleted = true;
    
	return isCompleted;
}

void TalkQuest::Draw(Render* render, Font* font, bool showMore)
{
	render->DrawText(font, name.c_str(), 10, 10, 24, 2, { 255, 255, 255 });
}