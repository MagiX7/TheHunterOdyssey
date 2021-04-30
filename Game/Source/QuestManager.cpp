#include "App.h"
#include "Render.h"
#include "Player.h"
#include "Audio.h"

#include "QuestManager.h"

#include "GetItemQuest.h"
#include "MurderQuest.h"
#include "VisitQuest.h"

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
		Quest* quest = nullptr;
		switch (node.attribute("type").as_int())
		{
		case 1:
			quest = new ItemQuest(node);
			break;
		case 2:
			quest = new MurderQuest(node);
			break;
		case 3:
			quest = new VisitQuest(node);
		}

		if (quest != nullptr) loadedQuests.push_back(quest);
	}

	completedQuestFx = app->audio->LoadFx("Assets/Audio/Fx/quest_completed.wav");
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

bool QuestManager::CheckQuests(EntityType type, SString string)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();

	for (; it != itEnd; ++it)
	{
		if ((*it)->Update(type, string) == true)
		{
			activeQuests.erase(it);
			finishedQuests.push_back(*it);
			app->audio->PlayFx(completedQuestFx);
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

//bool QuestManager::CompleteQuest(int id)
//{
//	eastl::list<Quest*>::iterator it = activeQuests.begin();
//	eastl::list<Quest*>::iterator itEnd = activeQuests.end();
//	for (; it != itEnd; ++it)
//	{
//		if ((*it)->id == id)
//		{
//			(*it)->isCompleted = true;
//			finishedQuests.push_back(*it);
//			activeQuests.erase(it);
//			break;
//		}
//	}
//
//	return true;
//}

bool QuestManager::Draw(Font* font)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();
	for (; it != itEnd; ++it)
	{
		app->render->DrawRectangle({ 0,0,200,50 }, 0, 0, 0, 255, true, false);
		(*it)->Draw(font);
	}

	return true;
}