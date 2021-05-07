#include "GetItemQuest.h"

ItemQuest::ItemQuest(pugi::xml_node n) : Quest(QuestType::ITEM_QUEST)
{
    qType = QuestType::ITEM_QUEST;
}

ItemQuest::~ItemQuest()
{
}

bool ItemQuest::Update()
{
    
    return false;
}

void ItemQuest::Draw(Font* font)
{
}

bool ItemQuest::SaveState(pugi::xml_node& node)
{
    return true;
}