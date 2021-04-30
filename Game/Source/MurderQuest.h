#pragma once

#include "Quest.h"

enum class EntityType;

class MurderQuest : public Quest
{
public:
	MurderQuest(pugi::xml_node n);

	virtual ~MurderQuest();

	bool Update(EntityType t, SString n = "") override;

	void Draw(Font* font) override;

public:
	EntityType type;

	int total;
	int actual;
};