#include "Render.h"
#include "GetItemQuest.h"

ItemQuest::ItemQuest(pugi::xml_node n) : Quest(QuestType::ITEM_QUEST)
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->description = n.attribute("description").as_string();
	this->nextQuestId = n.attribute("nextQuestId").as_int();
	this->type = (ItemType)n.attribute("item").as_int();
	this->reward = n.attribute("reward").as_int();
}

ItemQuest::~ItemQuest()
{
}

bool ItemQuest::Update(Item* item)
{
	if ((item != nullptr) && (type == item->itemType))
	{
		return true;
	}

	return false;
}

void ItemQuest::Draw(Render* render, Font* font, bool showMore)
{
	render->DrawText(font, name.c_str(), 15, 15, 24, 2, { 255, 255, 255 });
}

bool ItemQuest::SaveState(pugi::xml_node& node)
{
	node.append_attribute("type").set_value((int)qType);
	node.append_attribute("id").set_value(id);
	node.append_attribute("name").set_value(name.c_str());
	node.append_attribute("nextQuestId").set_value(nextQuestId);
	node.append_attribute("item").set_value((int)type);
	node.append_attribute("reward").set_value(reward);

    return true;
}