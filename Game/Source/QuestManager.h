#pragma once

#include "EASTL/list.h"
#include "PugiXml/src/pugixml.hpp"
#include "Quest.h"
#include "Input.h"

class Player;
enum class EntityType;

class QuestManager
{
public:
	QuestManager(const QuestManager&) = delete;
	void operator=(const QuestManager&) = delete;

	static QuestManager* GetInstance();

	virtual ~QuestManager();

	bool Update(Input* input, float dt);

	bool CheckQuests(Entity* entity, SString s = "");

	bool ActivateQuest(int id);

	bool QuestState();

	//bool CompleteQuest(int id);

	bool LoadQuests(pugi::xml_node& n);

	bool SaveQuests(pugi::xml_node& n);

	void Draw(Render* render, Font* font);

	bool UnLoad();

	Player* SetPlayer(Player* player);

	void GetReward(int reward);

private:
	QuestManager();

	static QuestManager* instance;

	eastl::list<Quest*> loadedQuests;
	eastl::list<Quest*> activeQuests;
	eastl::list<Quest*> finishedQuests;

	Quest* questFinished;
	Quest* questActive;

	int completedQuestFx;
	bool playFx;
	bool showMore;
	bool nextQuest;
	float timer;

	SDL_Texture* questTexture;
	Player* currentPlayer;
};