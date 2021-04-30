#pragma once

#include "EASTL/string.h"
#include "PugiXml/src/pugixml.hpp"
#include "SString.h"

#include "Font.h"

enum class QuestType
{
	ITEM_QUEST = 0,
	MURDER_QUEST,
	VISIT_QUEST
};

enum class EntityType;

class Quest
{
public:
	Quest() {}

	virtual ~Quest() {}

	virtual bool Update(EntityType t, SString n = "") { return true; }

	virtual void Draw(Font* font) {}

public:
	QuestType qType;
	int id;
	eastl::string name;
	int requiredId;
	bool isCompleted;
};