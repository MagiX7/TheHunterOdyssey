#pragma once

#include "Quest.h"

class VisitQuest : public Quest
{
public:
	VisitQuest(pugi::xml_node n);

	virtual ~VisitQuest();

	bool Update(Entity* entity, SString n = "") override;

	void Draw(Render* render, Font* font, bool showMore) override;
private:
	SString mapName;
};