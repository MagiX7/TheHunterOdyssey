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
	VISIT_QUEST
};

class Render;

class Quest
{
public:
	Quest() {}

	virtual ~Quest() {}

	virtual bool Update(Entity* entity, SString n = "") { return true; }

	virtual void Draw(Render* render, Font* font, bool showMore) {}

public:
	QuestType qType;
	int id;
	eastl::string name;
	int requiredId;
	bool isCompleted;
};