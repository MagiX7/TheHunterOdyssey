#pragma once

#include "Quest.h"

enum class EntityType;

class MurderQuest : public Quest
{
public:
	MurderQuest(pugi::xml_node n);

	virtual ~MurderQuest();

	bool Update(Entity* entity, SString n = "") override;

	void Draw(Render* render, Font* font, bool showMore) override;

public:
	EntityType type;

	int total;
	int actual;
};