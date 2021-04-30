#pragma once

#include "EASTL/list.h"
#include "EASTL/string.h"
#include "PugiXml/src/pugixml.hpp"

class Player;

class Quest
{
public:
	Quest();

	virtual ~Quest();

	int id;
	eastl::string name;
	int requiredId;
	bool isCompleted;
};

class QuestManager
{
public:
	QuestManager(const QuestManager&) = delete;
	void operator=(const QuestManager&) = delete;

	static QuestManager* GetInstance();

	virtual ~QuestManager();

	bool Update(Player* player);

	bool ActivateQuest(int id);

	bool CompleteQuest(int id);

	inline int ActiveListSize() { return activeQuests.size(); }

	bool Draw();

private:
	QuestManager();

	static QuestManager* instance;

	eastl::list<Quest*> loadedQuests;
	eastl::list<Quest*> activeQuests;
	eastl::list<Quest*> finishedQuests;

	pugi::xml_document questFile;

	int quest;
};