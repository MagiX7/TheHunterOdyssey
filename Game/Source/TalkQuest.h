#pragma once

#include "Quest.h"

class TalkQuest : public Quest
{
public:
	TalkQuest(pugi::xml_node n);

	virtual ~TalkQuest();

	bool Update(EntityType t, SString n = "") override;

	void Draw(Font* font) override;
private:
	EntityType type;
};