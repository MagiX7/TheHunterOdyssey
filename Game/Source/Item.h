#pragma once

#include "Entity.h"

enum class ItemType
{
	NONE = 0,
	POTION,
};

class Item : public Entity
{
public:
	Item(ItemType type, EntityType entType = EntityType::ITEM) : Entity(entType) {}
	virtual ~Item() {}

	//virtual bool Load() override { return true; }
	//virtual bool Update(float dt) override;
	virtual void Draw(bool showColliders, SDL_Rect bounds) {}
	//virtual bool UnLoad() override;

};