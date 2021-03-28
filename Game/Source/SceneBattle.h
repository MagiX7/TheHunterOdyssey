#pragma once

#include "Scene.h"

class Player;

class SceneBattle : public Scene
{
public:
	SceneBattle(Player* p1, Player* p2);
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
	Player* player1;
	Player* player2;
};