#pragma once

#include "EASTL/string.h"
#include "PugiXml/src/pugixml.hpp"
#include "SString.h"

#include "Font.h"
#include "Entity.h"
#include "Item.h"

enum class QuestType
{
	ITEM_QUEST = 0,
	MURDER_QUEST,
	VISIT_QUEST,
	TALK_QUEST
};

class Render;

class Quest
{
public:
	Quest(QuestType type) : qType(type){}

	virtual ~Quest() {}

	virtual bool Update(Entity* entity) { return false; }

	virtual bool Update(Item* item) { return false; }
	
	virtual bool Update(SString string) { return false; }

	virtual void Draw(Render* render, Font* font, bool showMore) {}

	virtual bool LoadState(pugi::xml_node node) { return true; }

	virtual bool SaveState(pugi::xml_node& node) { return true; }

public:
	QuestType qType;
	int id;
	eastl::string name;
	eastl::string description;
	int nextQuestId;
	bool isCompleted;
	int reward;
};