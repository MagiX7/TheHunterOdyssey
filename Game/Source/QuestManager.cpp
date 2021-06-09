#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"

#include "Audio.h"

#include "QuestManager.h"
#include"AssetsManager.h"
#include "GetItemQuest.h"
#include "MurderQuest.h"
#include "VisitQuest.h"
#include "TalkQuest.h"

QuestManager::QuestManager()
{
	pugi::xml_document questFile;
	int size = app->assetsManager->MakeLoad("Xml/quests.xml");
	pugi::xml_parse_result result = questFile.load_buffer(app->assetsManager->GetLastBuffer(), size);
	app->assetsManager->DeleteBuffer();
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
	guiTex = app->tex->Load("Textures/UI/gui_quests.png");
	questFinished = nullptr;
	questActive = nullptr;
	currentPlayer = nullptr;
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
		if (questTimer >= 3.0f && (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP))
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
		if (questTimer >= 3.0f && (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP))
		{
			activeQuests.push_back(questActive);
			questActive = nullptr;
			questTimer = 0.0f;
		}
	}
	
	if (nextQuest)
	{
		timer += 2.0f * dt;
		if (timer >= 3.01f)
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
		if (entity != nullptr && (*it)->Update(entity) == true)
		{
			activeQuests.erase(it);
			playFx = false;
			questFinished = *it;
		}
	}

	return true;
}

bool QuestManager::CheckQuests(Enemy* enemy)
{
	eastl::list<Quest*>::iterator it = activeQuests.begin();
	eastl::list<Quest*>::iterator itEnd = activeQuests.end();

	for (; it != itEnd; ++it)
	{
		if (enemy != nullptr && (*it)->Update(enemy) == true)
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

void QuestManager::DeleteAllQuests()
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
	loadedQuests.clear();

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
	activeQuests.clear();

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
	finishedQuests.clear();
}

bool QuestManager::LoadQuests(pugi::xml_node& n)
{
	pugi::xml_node generalNode = n.child("questmanager");
	pugi::xml_node node = generalNode.child("loaded_quests").child("quests");

	DeleteAllQuests();
	
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
	SDL_Rect r = { 0, 600, 275, 50 };
	render->DrawTexture(guiTex, 0, 0, &r, false);
	
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

	SDL_Rect section = { 0,0,1000,600 };
	SDL_Rect rect = { 232, 152, 816, 435 };

	if (questFinished != nullptr)
	{
		render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 120, true, false);
		render->DrawTexture(guiTex, 140, 60, &section, false);
		if (questTimer < 3.0f)
		{
			render->DrawCenterText(font, "Quest Completed!", rect, 64, 5, { 255, 255, 255 });
		}
		else
		{
			render->DrawCenterText(font, "Quest Completed!", rect, 64, 5, { 255, 255, 255 });
			render->DrawText(font, "Press A to continue", 765, 560, 24, 3, { 255, 255, 255 });
		}
	}

	if (questActive != nullptr)
	{
		render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 120, true, false);
		render->DrawTexture(guiTex, 140, 60, &section, false);
		if (questTimer < 3.0f)
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
			render->DrawText(font, "Press A to continue", 800, 560, 24, 3, { 255, 255, 255 });
		}
	}
}

bool QuestManager::UnLoad()
{
	DeleteAllQuests();

	app->tex->UnLoad(guiTex);
	//app->audio->UnLoadFx(completedQuestFx);

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