#pragma once

#include "Entity.h"

enum class ItemType
{
	NONE = 0,
	POTION,
	ULTRA_POTION,
	ETER,
	ULTRA_ETER,
	OMNI_POTION,
	FAIRY_TEAR,
	FAIRY_WING,
	FAIRY_BOTTLE
};

class Item : public Entity
{
public:
	Item(ItemType type, iPoint pos, EntityType entType = EntityType::ITEM) : Entity(entType), iType(type)
	{
		bounds.x = pos.x;
		bounds.y = pos.y;
		bounds.w = 32;
		bounds.h = 32;
	}

	virtual ~Item() {}

	//virtual bool Load() override { return true; }
	//virtual bool Update(float dt) override;
	virtual void Draw(bool showColliders) {}
	//virtual bool UnLoad() override;

	inline ItemType GetItemType() const { return iType; }


private:
	ItemType iType;
};