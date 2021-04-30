#include "App.h"
#include "Render.h"
#include "Player.h"

#include "QuestManager.h"

Quest::Quest()
{
}

Quest::~Quest()
{
}

QuestManager* QuestManager::instance = nullptr;

QuestManager* QuestManager::GetInstance()
{
	if (instance == nullptr) instance = new QuestManager();
	
	return instance;
}

QuestManager::QuestManager()
{
	questFile.load_file("quests.xml");
	pugi::xml_node node = questFile.child("quests").child("quest");
	for (; node; node = node.next_sibling("quest"))
	{
		Quest* quest = new Quest();
		quest->id = node.attribute("id").as_int();
		quest->name = node.attribute("name").as_string();
		quest->requiredId = node.attribute("requiredId").as_int();
		quest->isCompleted = node.attribute("isCompleted").as_bool();

		loadedQuests.push_back(quest);
	}
}

QuestManager::~QuestManager()
{
}

bool QuestManager::Update(Player* player)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();

	for (; it != itEnd; ++it)
	{
		switch ((*it)->id)
		{
		case 0:
			if (player->GetMovements() == 10) CompleteQuest((*it)->id);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
	}
	return true;
}

bool QuestManager::ActivateQuest(int id)
{
	eastl::list<Quest*>::iterator it = loadedQuests.begin();
	eastl::list<Quest*>::iterator itEnd = loadedQuests.end();
	for (; it != itEnd; ++it)
	{
		if ((*it)->id == id)
		{
			activeQuests.push_back(*it);
			loadedQuests.erase(it);
			break;
		}
	}

	return true;
}

bool QuestManager::CompleteQuest(int id)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();
	for (; it != itEnd; ++it)
	{
		if ((*it)->id == id)
		{
			(*it)->isCompleted = true;
			finishedQuests.push_back(*it);
			activeQuests.erase(it);
			break;
		}
	}

	return true;
}

bool QuestManager::Draw()
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();
	for (; it != itEnd; ++it)
	{
		app->render->DrawRectangle({ 0,0,50,50 }, 0, 0, 0, 255);
	}

	return true;
}