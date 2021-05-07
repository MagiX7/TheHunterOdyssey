#include "Render.h"

#include "VisitQuest.h"

VisitQuest::VisitQuest(pugi::xml_node n) : Quest(QuestType::VISIT_QUEST)
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->nextQuestId = n.attribute("nextQuestId").as_int();
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

bool VisitQuest::SaveState(pugi::xml_node& node)
{
	node.append_attribute("type").set_value((int)qType);
	node.append_attribute("id").set_value(id);
	node.append_attribute("name").set_value(name.c_str());
	node.append_attribute("nextQuestId").set_value(nextQuestId);
	node.append_attribute("isCompleted").set_value(isCompleted);
	node.append_attribute("mapName").set_value(mapName.GetString());

	return true;
}