#pragma once

#include "Quest.h"

class VisitQuest : public Quest
{
public:
	VisitQuest(pugi::xml_node n);

	virtual ~VisitQuest();

	bool Update(SString string) override;

	void Draw(Render* render, Font* font, bool showMore) override;

	bool SaveState(pugi::xml_node& node) override;
private:
	SString mapName;
};