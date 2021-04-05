#pragma once
#define FIND_RADIOUS 10
#include "Entity.h"
#include"Font.h"
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

	NpcType npcType;
protected:
	int dialogeId;
	bool talkStart;
	bool drawPtext;
	Font* font;
	SDL_Texture* texture;
};