#pragma once

#include "Quest.h"

class TalkQuest : public Quest
{
public:
	TalkQuest(pugi::xml_node n);

	virtual ~TalkQuest();

	bool Update(Entity* entity) override;

	void Draw(Render* render, Font* font, bool showMore);

	bool SaveState(pugi::xml_node& node) override;
private:
	int npcId;
	EntityType type;
};