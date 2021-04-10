#pragma once
#include "Entity.h"
#include"Font.h"


#define FIND_RADIOUS 10
enum class NpcState
{
	WALLKING_LEFT,
	WALLKING_RIGHT,
	WALLKING_UP,
	WALLKING_DOWN,
	STOP_LEFT,
	STOP_RIGHT,
	STOP_UP,
	STOP_DOWN,
	TALKING
};
enum class NpcType
{
	NONE = 0,
	TABERN,
	WIZARD,
	TOWN,
};

class Npc : public Entity
{
public:
	Npc(EntityType type, iPoint position, int id);
	~Npc();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders) override;
	bool UnLoad() override;
	bool SaveState(pugi::xml_node& node);

	bool CheckCollision(Player* player);
	NpcState state;

	void NpcMove(bool move);

	NpcState getNewState();

	void onCollision();
public:
	SDL_Rect lastPosition;
	bool npcMove;
	float stateTimer;
	int stateMaxTimer;
	Animation idleDown;
	Animation idleLeft;
	Animation idleRight;
	Animation idleUp;
	Animation walkDown;
	Animation walkLeft;
	Animation walkRight;
	Animation walkUp;
	NpcType npcType;

	int dialogeId;
	bool talkStart;
	bool drawPtext;
	Font* font;
	SDL_Texture* texture;
};