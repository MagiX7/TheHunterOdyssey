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

bool QuestManager::Draw(Render* render, Font* font)
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

	return true;
}