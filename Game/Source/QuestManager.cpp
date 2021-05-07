#include "App.h"
#include "Input.h"
#include "Render.h"

#include "Audio.h"

#include "QuestManager.h"

#include "GetItemQuest.h"
#include "MurderQuest.h"
#include "VisitQuest.h"
#include "TalkQuest.h"

QuestManager* QuestManager::instance = nullptr;

QuestManager* QuestManager::GetInstance()
{
	if (instance == nullptr) instance = new QuestManager();
	
	return instance;
}

QuestManager::QuestManager()
{
	pugi::xml_document questFile;
	questFile.load_file("quests.xml");
	pugi::xml_node node = questFile.child("quests").child("quest");
	for (; node; node = node.next_sibling("quest"))
	{
		Quest* quest;
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
			break;
		case 4:
			quest = new TalkQuest(node);
			break;
		}

		loadedQuests.push_back(quest);
	}

	completedQuestFx = app->audio->LoadFx("Assets/Audio/Fx/quest_completed.wav");
	questFinished = nullptr;
	questActive = nullptr;
	playFx = false;
	showMore = false;
}

QuestManager::~QuestManager()
{
}

bool QuestManager::Update(Input* input, float dt)
{
	if (questFinished != nullptr)
	{
		if (!playFx) app->audio->PlayFx(completedQuestFx);
		playFx = true;
		if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			finishedQuests.push_back(questFinished);
			questFinished = nullptr;
		}
	}
	if (questActive != nullptr)
	{
		if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			activeQuests.push_back(questActive);
			questActive = nullptr;
		}
	}

	if (!activeQuests.empty() && input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) showMore = !showMore;
	//eastl::list<Quest*>::iterator it = activeQuests.begin();
	//eastl::list<Quest*>::iterator itEnd = activeQuests.end();

	//for (; it != itEnd; ++it)
	//{
	//	switch ((*it)->id)
	//	{
	//	case 0:
	//		break;
	//	case 1:
	//		break;
	//	case 2:
	//		break;
	//	case 3:
	//		break;
	//	case 4:
	//		break;
	//	case 5:
	//		break;
	//	default:
	//		break;
	//	}
	//}
	return true;
}

bool QuestManager::CheckQuests(Entity* entity, SString string)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();

	for (; it != itEnd; ++it)
	{
		if ((*it)->Update(entity, string) == true)
		{
			activeQuests.erase(it);
			playFx = false;
			//finishedQuests.push_back(*it);
			questFinished = *it;
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
			loadedQuests.erase(it);
			questActive = *it;
			//activeQuests.push_back(*it);
			break;
		}
	}

	return true;
}

bool QuestManager::QuestState()
{
	if (questFinished == nullptr && questActive == nullptr) return false;
	
	return true;
}

bool QuestManager::LoadQuests(pugi::xml_node& n)
{
	pugi::xml_node generalNode = n.child("questmanager");
	pugi::xml_node node = generalNode.child("loaded_quests").child("quests");

	loadedQuests.clear();
	for (; node; node = node.next_sibling("quests"))
	{
		Quest* quest;
		switch ((QuestType)node.attribute("type").as_int())
		{
		case QuestType::ITEM_QUEST:
			quest = new ItemQuest(node);
			break;
		case QuestType::MURDER_QUEST:
			quest = new MurderQuest(node);
			break;
		case QuestType::VISIT_QUEST:
			quest = new VisitQuest(node);
			break;
		case QuestType::TALK_QUEST:
			quest = new TalkQuest(node);
			break;
		}
		loadedQuests.push_back(quest);
	}

	node = generalNode.child("active_quests").child("quests");

	activeQuests.clear();
	for (; node; node = node.next_sibling("quests"))
	{
		Quest* quest;
		switch ((QuestType)node.attribute("type").as_int())
		{
		case QuestType::ITEM_QUEST:
			quest = new ItemQuest(node);
			break;
		case QuestType::MURDER_QUEST:
			quest = new MurderQuest(node);
			break;
		case QuestType::VISIT_QUEST:
			quest = new VisitQuest(node);
			break;
		case QuestType::TALK_QUEST:
			quest = new TalkQuest(node);
			break;
		}
		activeQuests.push_back(quest);
	}

	node = generalNode.child("finished_quests").child("quests");

	finishedQuests.clear();
	for (; node; node = node.next_sibling("quests"))
	{
		Quest* quest;
		switch ((QuestType)node.attribute("type").as_int())
		{
		case QuestType::ITEM_QUEST:
			quest = new ItemQuest(node);
			break;
		case QuestType::MURDER_QUEST:
			quest = new MurderQuest(node);
			break;
		case QuestType::VISIT_QUEST:
			quest = new VisitQuest(node);
			break;
		case QuestType::TALK_QUEST:
			quest = new TalkQuest(node);
			break;
		}
		finishedQuests.push_back(quest);
	}

	return true;
}

bool QuestManager::SaveQuests(pugi::xml_node& n)
{
	pugi::xml_node generalNode = n.append_child("questmanager");
	pugi::xml_node node = generalNode.append_child("loaded_quests");

	eastl::list<Quest*>::iterator it;
	eastl::list<Quest*>::iterator itEnd;
	if (!loadedQuests.empty())
	{
		it = loadedQuests.begin();
		itEnd = loadedQuests.end();
		for (; it != itEnd; ++it)
		{
			(*it)->SaveState(node.append_child("quests"));
		}
	}

	node = generalNode.append_child("active_quests");
	if (!activeQuests.empty())
	{
		it = activeQuests.begin();
		itEnd = activeQuests.end();
		for (; it != itEnd; ++it)
		{
			(*it)->SaveState(node.append_child("quests"));
		}
	}

	node = generalNode.append_child("finished_quests");
	if (!finishedQuests.empty())
	{
		it = finishedQuests.begin();
		itEnd = finishedQuests.end();
		for (; it != itEnd; ++it)
		{
			(*it)->SaveState(node.append_child("quests"));
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

void QuestManager::Draw(Render* render, Font* font)
{
	SDL_Rect r;
	if (showMore) r = {0, 0, 200, 150};
	else r = { 0, 0, 200, 50 };
	render->DrawRectangle(r, 0, 0, 0, 150, true, false);
	
	if (activeQuests.empty()) 
		render->DrawText(font, "No active quests", 10, 10, 24, 2, { 255, 255, 255 });
	else
	{
		eastl::list<Quest*>::iterator it = activeQuests.begin();
		eastl::list<Quest*>::iterator itEnd = activeQuests.end();
		for (; it != itEnd; ++it)
		{
			(*it)->Draw(render, font, showMore);
		}
	}

	SDL_Rect rect = { 390, 260,500,200 };
	if (questFinished != nullptr)
	{
		render->DrawRectangle(rect, 0, 0, 0, 150, true, false);
		render->DrawCenterText(font, "Quest Completed!", rect, 36, 5, { 255, 255, 255 });
	}

	if (questActive != nullptr)
	{
		render->DrawRectangle(rect, 0, 0, 0, 150, true, false);
		render->DrawCenterText(font, "New Quest!", rect, 36, 5, { 255, 255, 255 });
	}
}

bool QuestManager::UnLoad()
{
	if (!loadedQuests.empty())
	{
		eastl::list<Quest*>::iterator it = loadedQuests.begin();
		eastl::list<Quest*>::iterator itEnd = loadedQuests.end();
		for (; it != itEnd; ++it)
		{
			RELEASE(*it);
			loadedQuests.erase(it);
		}
	}

	if (!activeQuests.empty())
	{
		eastl::list<Quest*>::iterator it = activeQuests.begin();
		eastl::list<Quest*>::iterator itEnd = activeQuests.end();
		for (; it != itEnd; ++it)
		{
			RELEASE(*it);
			activeQuests.erase(it);
		}
	}

	if (!finishedQuests.empty())
	{
		eastl::list<Quest*>::iterator it = finishedQuests.begin();
		eastl::list<Quest*>::iterator itEnd = finishedQuests.end();
		for (; it != itEnd; ++it)
		{
			RELEASE(*it);
			finishedQuests.erase(it);
		}
	}

	return true;
}