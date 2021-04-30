#pragma once

#include "EASTL/list.h"
#include "PugiXml/src/pugixml.hpp"
#include "Quest.h"

class Player;
enum class EntityType;

class QuestManager
{
public:
	QuestManager(const QuestManager&) = delete;
	void operator=(const QuestManager&) = delete;

	static QuestManager* GetInstance();

	virtual ~QuestManager();

	bool Update(Player* player);

	bool CheckQuests(EntityType t, SString s = "");

	bool ActivateQuest(int id);

	bool CompleteQuest(int id);

	bool Draw(Font* font);

private:
	QuestManager();

	static QuestManager* instance;

	eastl::list<Quest*> loadedQuests;
	eastl::list<Quest*> activeQuests;
	eastl::list<Quest*> finishedQuests;

	pugi::xml_document questFile;

	int quest;
};