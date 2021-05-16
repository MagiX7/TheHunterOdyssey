#include "Render.h"
#include "TalkQuest.h"
#include "QuestManager.h"

#include "Entity.h"

TalkQuest::TalkQuest(pugi::xml_node n) : Quest(QuestType::TALK_QUEST)
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->description = n.attribute("description").as_string();
	this->nextQuestId = n.attribute("nextQuestId").as_int();
	this->npcId = n.attribute("npcId").as_int();
	this->type = (EntityType)n.attribute("entityType").as_int();
	this->reward = n.attribute("reward").as_int();
}

TalkQuest::~TalkQuest()
{
}


bool TalkQuest::Update(Entity* entity)
{
	if ((entity != nullptr) && (npcId == entity->GetDialogeId())) return true;
    
	return false;
}

void TalkQuest::Draw(Render* render, Font* font, bool showMore)
{
	render->DrawText(font, name.c_str(), 15, 15, 24, 2, { 255, 255, 255 });
}

bool TalkQuest::SaveState(pugi::xml_node& node)
{
	node.append_attribute("type").set_value((int)qType);
	node.append_attribute("id").set_value(id);
	node.append_attribute("name").set_value(name.c_str());
	node.append_attribute("description").set_value(description.c_str());
	node.append_attribute("nextQuestId").set_value(nextQuestId);
	node.append_attribute("npcId").set_value(npcId);
	node.append_attribute("reward").set_value(reward);

	return true;
}