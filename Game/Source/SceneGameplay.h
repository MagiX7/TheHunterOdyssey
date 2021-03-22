#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

class Player;
class CharacterManager;
class Npc;
class SceneBattle;

enum class PlayerType;

enum class GameplayMenuState
{
	NONE = 0,
	CHARACTER_SWAP,
	INVENTORY,
	PAUSE
};

enum class GameplayState
{
	NONE = 0,
	ROAMING,
	BATTLE
};

class SceneGameplay : public Scene
{
public:
	SceneGameplay();

	virtual ~SceneGameplay() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

	void ChangeState(GameplayMenuState type);

	void SetPlayer(Player* pl);

private:
	SDL_Texture* bg;

	Player* player;
	Npc* npc;

	CharacterManager* charManager;

	SceneBattle* sceneBattle;

	GameplayMenuState menuState;

	GameplayState gameState;
};

#endif //__SCENEGAMEPLAY_H__