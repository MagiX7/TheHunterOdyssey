#pragma once

#include "Quest.h"

enum class ItemType;

class ItemQuest : public Quest
{
public:
	ItemQuest(pugi::xml_node n);

	virtual ~ItemQuest();

	bool Update(Item* item) override;

	void Draw(Render* render, Font* font, bool showMore);

	bool SaveState(pugi::xml_node& node) override;

private:
	ItemType type;

	int itemQuantity;
	int totalItems;
};