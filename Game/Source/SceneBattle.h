#pragma once

#include "Scene.h"

class Player;
class Enemy;
class BattleMenu;

class SceneBattle : public Scene
{
public:
	SceneBattle(eastl::list<Player*> list, int enemies);
	virtual ~SceneBattle();

	// Called before the first frame
	bool Load() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	void Draw(bool colliders);

	// Called before quitting
	bool UnLoad() override;

	/*bool LoadState(pugi::xml_node&) { return true; }

	bool SaveState(pugi::xml_node&) const { return true; }*/

private:
	int numEnemies;

	eastl::list<Enemy*> enemyList;
	eastl::list<Player*> playerList;

	// Menu
	BattleMenu* battleMenu;
};