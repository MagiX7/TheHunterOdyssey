#pragma once

#include "Quest.h"

enum class ItemType;

class ItemQuest : public Quest
{
public:
	ItemQuest(pugi::xml_node n);

	virtual ~ItemQuest();

	bool Update();

	void Draw(Font* font);

private:
	ItemType type;

	int itemQuantity;
	int totalItems;
};