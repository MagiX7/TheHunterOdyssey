#pragma once

#include "EASTL/string.h"
#include "PugiXml/src/pugixml.hpp"
#include "SString.h"

#include "Font.h"
#include "Entity.h"

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

	virtual bool Update(Entity* entity, SString n = "") { return true; }

	virtual void Draw(Render* render, Font* font, bool showMore) {}

	virtual bool LoadState(pugi::xml_node node) { return true; }

	virtual bool SaveState(pugi::xml_node& node) { return true; }

public:
	QuestType qType;
	int id;
	eastl::string name;
	int nextQuestId;
	bool isCompleted;
	int reward;
};