#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "EASTL/list.h"
#include "Font.h"
#include "Point.h"

class Player;
class CharacterManager;
class SceneBattle;
class EntityManager;
class Map;
class DialogueManager;
class PauseMenu;
class Render;
enum class PlayerType;
enum class EntityType;

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
	
	bool CollisionMapEntity(SDL_Rect rect, EntityType type);

private:
	void HandleInput(float dt);
	
	bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2);
	void CameraFollow(Render* render);
	void Fading(float dt);


public:
	DialogueManager* dialogueManager;
	
private:
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

	iPoint tmpPosPlayer;

	float alpha;
	bool transition;
	bool fadeOut;
};

#endif //__SCENEGAMEPLAY_H__