#pragma once

#include "Entity.h"

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
	Npc(EntityType type);
	~Npc();

	bool Load() override;
	bool Update(float dt) override;
	void Draw(bool showColliders) override;
	bool UnLoad() override;

};