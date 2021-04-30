#pragma once

#include "Quest.h"

class VisitQuest : public Quest
{
public:
	VisitQuest(pugi::xml_node n);

	virtual ~VisitQuest();

	bool Update(EntityType t, SString n = "") override;

	void Draw(Font* font) override;
private:
	SString mapName;
};