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

class Font;

enum class PlayerType;
enum class EntityType;
class ParticlesManager;
class Generator;

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
	
	bool CollisionMapEntity(SDL_Rect rect, EntityType type);

private:
	void HandleInput(float dt);
	
	bool CheckCollision(SDL_Rect rec1, SDL_Rect rec2);
	void GenerateBattle();
	void CameraFollow(Render* render);
	void Transitioning(float dt);
	void ChangeBlockBounds(int bounds_x, int bounds_y);

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

	// Interruptor block
	Item* interruptorBlock;

	SDL_Texture* atlas;

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
	
	eastl::list<Enemy*> enemyList;
	Enemy* tmp;

	SDL_Rect iceBlockRect;

	float firstQuest;
	bool firstQuestAdded;
};

#endif //__SCENEGAMEPLAY_H__