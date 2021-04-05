#pragma once
#include "Entity.h"
#include"Font.h"

#define FIND_RADIOUS 10

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
	Npc(EntityType type, iPoint position);
	~Npc();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders) override;
	bool UnLoad() override;
	bool SaveState(pugi::xml_node& node);

	bool CheckCollision(Player* player);

public:
	NpcType npcType;

	int dialogeId;
	bool talkStart;
	bool drawPtext;
	Font* font;
	SDL_Texture* texture;
};