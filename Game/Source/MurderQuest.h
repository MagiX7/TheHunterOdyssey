#pragma once

#include "Quest.h"

enum class EntityType;

class MurderQuest : public Quest
{
public:
	MurderQuest(pugi::xml_node n);

	virtual ~MurderQuest();

	bool Update(Enemy* enemy) override;

	void Draw(Render* render, Font* font, bool showMore) override;

	bool SaveState(pugi::xml_node& node) override;

public:
	EnemyType type;

	int total;
	int actual;
};