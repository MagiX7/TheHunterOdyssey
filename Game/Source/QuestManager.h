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
	QuestManager();

	virtual ~QuestManager();

	bool Update(Input* input, float dt);

	bool CheckQuests(Entity* entity);
	bool CheckQuests(Enemy* enemy);
	bool CheckQuests(Item* item);
	bool CheckQuests(SString string);

	bool ActivateQuest(int id);

	bool QuestState();

	void DeleteAllQuests();

	bool LoadQuests(pugi::xml_node& n);

	bool SaveQuests(pugi::xml_node& n);

	void Draw(Render* render, Font* font);

	bool UnLoad();

	Player* SetPlayer(Player* player);

	void GetReward(int reward);

	inline eastl::list<Quest*>& GetFinishedList() { return finishedQuests; }
	
	inline eastl::list<Quest*>& GetActiveList() { return activeQuests; }

private:

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
	float questTimer;

	int channel;

	SDL_Texture* guiTex;
	Player* currentPlayer;
};