#include "Render.h"

#include "MurderQuest.h"
#include "QuestManager.h"

MurderQuest::MurderQuest(pugi::xml_node n) : Quest(QuestType::MURDER_QUEST)
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->description = n.attribute("description").as_string();
	this->nextQuestId = n.attribute("nextQuestId").as_int();
	this->type = (EntityType)n.attribute("enemy").as_int();
	this->total = n.attribute("total").as_int();
	this->actual = n.attribute("actual").as_int();
	this->reward = n.attribute("reward").as_int();
}

MurderQuest::~MurderQuest()
{
}

bool MurderQuest::Update(Enemy* enemy)
{
	if ((enemy != nullptr) && (type == enemy->type))
	{
		return true;
	}

	return false;
}

void MurderQuest::Draw(Render* render, Font* font, bool showMore)
{
	render->DrawText(font, name.c_str(), 15, 15, 24, 2, { 255, 255, 255 });
}

bool MurderQuest::SaveState(pugi::xml_node& node)
{
	node.append_attribute("type").set_value((int)qType);
	node.append_attribute("id").set_value(id);
	node.append_attribute("name").set_value(name.c_str());
	node.append_attribute("description").set_value(name.c_str());
	node.append_attribute("nextQuestId").set_value(nextQuestId);
	node.append_attribute("enemy").set_value((int)type);
	node.append_attribute("actual").set_value(actual);
	node.append_attribute("total").set_value(total);
	node.append_attribute("reward").set_value(reward);

	return true;
}