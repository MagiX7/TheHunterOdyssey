#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "Point.h"
#include "ParticlesManager.h"

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
class QuestManager;
class QuestMenu;
class Shop;

class Font;

enum class PlayerType;
enum class EntityType;

enum class GameplayMenuState
{
	NONE = 0,
	CHARACTER_SWAP,
	INVENTORY,
	PAUSE,
	QUESTS,
	SHOP
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
	
	bool CollisionMapEntity(SDL_Rect rect, EntityType type);

private:
	void HandleInput(Input* input, float dt);
	
	bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2);
	void GenerateBattle();
	void CameraFollow(Render* render);
	void Transitioning(float dt);
	void ChangeBlockBounds(int bounds_x, int bounds_y);
	void SetCameraMovement(int target_x, int target_y, float dt);

	void ChangeMap(const char* mapName, iPoint newPos, int doorFx = -1);

	void LoadNpc(SString mapName);
	void LoadItems(pugi::xml_node& n);

private:
	Player* currentPlayer;
	DialogueManager* dialogueManager;
	SDL_Texture* goldTexture;
	eastl::list<Player*> playerList;

	// Menus
	CharacterManager* charManager;
	PauseMenu* pause;
	GameplayMenuState menuState;
	GameplayState gameState;

	EntityManager* entityManager;

	ParticlesManager* particles;

	QuestManager* questManager;

	// Scenes
	SceneBattle* sceneBattle;
	float temp;

	Map* map;

	// Font
	Font* font;

	// Inventory
	Inventory* inventory;

	// Quest Menu
	QuestMenu* quests;

	// Shop Menu
	Shop* shop;

	// Interruptor block
	Item* interruptorBlock;

	SDL_Texture* atlas;
	SDL_Texture* guiTex;

	eastl::list<Item*> items;

	iPoint tmpPosPlayer;

	float alpha;
	bool transition;
	bool fadeOut;
	bool loadObjects;
	bool deleteDoor;
	//Fx
	int doorOpenedFx;
	int doorClosedFx;

	int puzzleCompletedFx;
	bool canSound1;
	bool canSound2;
	bool canSound3;
	bool canSound4;
	
	int whereMove;
	int cameraCounter;

	eastl::list<Enemy*> enemyList;
	Enemy* tmp;

	SDL_Rect iceBlockRect;

	float firstQuest;
	bool firstQuestAdded;
};

#endif //__SCENEGAMEPLAY_H__