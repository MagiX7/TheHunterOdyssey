#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "Point.h"
#include "Font.h"

#include "EASTL/list.h"

class Player;
class Enemy;
class CharacterManager;
class SceneBattle;
class EntityManager;
class Map;
class DialogueManager;
class PauseMenu;
class Render;
class Item;
class Inventory;

enum class PlayerType;
enum class EntityType;

enum class GameplayMenuState
{
	NONE = 0,
	CHARACTER_SWAP,
	INVENTORY,
	PAUSE,
};

enum class GameplayState
{
	NONE = 0,
	ROAMING,
	BATTLE,
	DIALOG,
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
	void GenerateBattle();
	void CameraFollow(Render* render);
	void Fading(float dt);

	//void GenerateInventory();

public:
	DialogueManager* dialogueManager;
	
private:

	// Player stuff
	Player* currentPlayer;
	eastl::list<Player*> playerList;


	// Menus
	CharacterManager* charManager;
	PauseMenu* pause;
	GameplayMenuState menuState;
	GameplayState gameState;

	// Scenes
	SceneBattle* sceneBattle;
	Map* map;

	// Managers
	EntityManager* entityManager;

	// Font
	Font* font;

	// Inventory
	Inventory* inventory;

	// Items
	Item* ultraPotion;

	iPoint tmpPosPlayer;

	float alpha;
	bool transition;
	bool fadeOut;
	
	eastl::list<Enemy*> enemyList;
	Enemy* tmp;
};

#endif //__SCENEGAMEPLAY_H__