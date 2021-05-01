#pragma once

#include "Quest.h"

class TalkQuest : public Quest
{
public:
	TalkQuest(pugi::xml_node n);

	virtual ~TalkQuest();

	bool Update(Entity* entity, SString name = "");

	void Draw(Render* render, Font* font, bool showMore);
private:
	int npcId;
};