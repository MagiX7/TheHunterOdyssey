#include "GetItemQuest.h"

ItemQuest::ItemQuest(pugi::xml_node n) : Quest()
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