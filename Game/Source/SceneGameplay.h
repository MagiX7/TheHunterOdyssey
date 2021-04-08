#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include"EASTL/list.h"
#include "Font.h"

class Player;
class CharacterManager;
class Npc;
class SceneBattle;
class EntityManager;
class Map;
class DialogueManager;
class PauseMenu;
class Render;
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
	BATTLE,
	DIALOG
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

	void CharacterSwap(PlayerType pType);

	bool CheckDialogue();

private:
	void HandleInput(float dt);
	bool CollisionMapEntity(SDL_Rect rect);
	bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2);
	void CameraFollow(Render* render);

public:
	Player* getCurrentPlayer();
	DialogueManager* dialogueManager;

private:
	SDL_Texture* bgDialog;

	Player* currentPlayer;
	
	eastl::list<Player*> playerList;

	CharacterManager* charManager;

	SceneBattle* sceneBattle;

	GameplayMenuState menuState;

	GameplayState gameState;

	EntityManager* entityManager;

	Map* map;

	PauseMenu* pause;

	Font* font;
};

#endif //__SCENEGAMEPLAY_H__