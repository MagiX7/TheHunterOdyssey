#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"

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

	completedQuestFx = app->audio->LoadFx("Audio/Fx/quest_completed.wav");
	questTexture = app->tex->Load("Textures/UI/quest_texture.png");
	texture = app->tex->Load("Textures/UI/quests.png");
	questFinished = nullptr;
	questActive = nullptr;
	playFx = false;
	showMore = false;
	nextQuest = false;
	timer = 0.0f;
	questTimer = 0.0f;
	channel = app->audio->SetChannel();
}

QuestManager::~QuestManager()
{
}

bool QuestManager::Update(Input* input, float dt)
{
	if (questFinished != nullptr)
	{
		if (!playFx) app->audio->PlayFx(channel, completedQuestFx);
		playFx = true;
		questTimer += 2.0f * dt;
		if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			GetReward(questFinished->reward);
			finishedQuests.push_back(questFinished);
			questFinished = nullptr;
			nextQuest = true;
			questTimer = 0.0f;
		}
	}
	if (questActive != nullptr)
	{
		questTimer += 2.0f * dt;
		if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			activeQuests.push_back(questActive);
			questActive = nullptr;
			questTimer = 0.0f;
		}
	}

	if (!activeQuests.empty() && input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) showMore = !showMore;
	
	if (nextQuest)
	{
		timer += 2.0f * dt;
		if (timer >= 5.01f)
		{
			ActivateQuest((*finishedQuests.end().prev())->nextQuestId);
			nextQuest = false;
			timer = 0.0f;
		}
	}

	return true;
}

bool QuestManager::CheckQuests(Entity* entity)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();

	for (; it != itEnd; ++it)
	{
		if ((*it)->Update(entity) == true)
		{
			activeQuests.erase(it);
			playFx = false;
			questFinished = *it;
		}
	}

	return true;
}

bool QuestManager::CheckQuests(Item* item)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();

	for (; it != itEnd; ++it)
	{
		if ((*it)->Update(item) == true)
		{
			activeQuests.erase(it);
			playFx = false;
			questFinished = *it;
		}
	}
	return true;
}

bool QuestManager::CheckQuests(SString string)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();

	for (; it != itEnd; ++it)
	{
		if ((*it)->Update(string) == true)
		{
			activeQuests.erase(it);
			playFx = false;
			questFinished = *it;
		}
	}

	return true;
}

bool QuestManager::ActivateQuest(int id)
{
	if (id != -1)
	{
		eastl::list<Quest*>::iterator it = loadedQuests.begin();
		eastl::list<Quest*>::iterator itEnd = loadedQuests.end();
		for (; it != itEnd; ++it)
		{
			if ((*it)->id == id)
			{
				loadedQuests.erase(it);
				questActive = *it;
				break;
			}
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

void QuestManager::Draw(Render* render, Font* font)
{
	SDL_Rect r;
	if (showMore) r = {0, 0, 200, 150};
	else r = { 0, 0, 200, 50 };
	render->DrawTexture(questTexture, -render->camera.x, -render->camera.y, &r);
	
	if (activeQuests.empty()) 
		render->DrawText(font, "No active quests", 15, 15, 24, 2, { 255, 255, 255 });
	else
	{
		eastl::list<Quest*>::iterator it = activeQuests.begin();
		eastl::list<Quest*>::iterator itEnd = activeQuests.end();
		for (; it != itEnd; ++it)
		{
			(*it)->Draw(render, font, showMore);
		}
	}

	SDL_Rect rect = { 232, 152, 816, 435 };
	if (questFinished != nullptr)
	{
		render->DrawTexture(texture, -render->camera.x + 140, -render->camera.y + 60);
		if (questTimer < 5.0f)
		{
			render->DrawCenterText(font, "Quest Completed!", rect, 64, 5, { 255, 255, 255 });
		}
		else
		{
			render->DrawText(font, "Press ENTER to continue", 765, 560, 24, 3, { 255, 255, 255 });
		}
	}

	if (questActive != nullptr)
	{
		render->DrawTexture(texture, -render->camera.x + 140, -render->camera.y + 60);
		if (questTimer < 5.0f)
		{
			if (!finishedQuests.empty()) render->DrawCenterText(font, "New Quest!", rect, 64, 5, { 255, 255, 255 });
			else render->DrawCenterText(font, "This is your first quest!", rect, 64, 5, { 255, 255, 255 });
		}
		else
		{
			rect.y = 170;
			rect.h = 0;
			render->DrawCenterText(font, questActive->name.c_str(), rect, 50, 5, { 255, 255, 255 });
			render->DrawText(font, questActive->description.c_str(), 250, 270, 24, 3, { 255, 255, 255 }, 1050);
			render->DrawText(font, "Press ENTER to continue", 765, 560, 24, 3, { 255, 255, 255 });
		}
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

	app->tex->UnLoad(questTexture);
	app->tex->UnLoad(texture);
	app->audio->UnLoadFx(completedQuestFx);

	return true;
}

Player* QuestManager::SetPlayer(Player* player)
{
	currentPlayer = player;
	return currentPlayer;
}

void QuestManager::GetReward(int reward)
{
	currentPlayer->gold += reward;
}