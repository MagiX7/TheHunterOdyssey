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
	Item() {};
	Item(ItemType type, iPoint pos, SDL_Texture* atlas ,EntityType entType = EntityType::ITEM) : Entity(entType), iType(type), texture(atlas) ,isDragging(false)
	{
		bounds.x = pos.x;
		bounds.y = pos.y;
		bounds.w = 32;
		bounds.h = 32;
	}

	virtual ~Item() {}

	//virtual bool Load() override { return true; }
	//virtual bool Update(float dt) override;
	//virtual void Draw(bool showColliders) {}
	//virtual bool UnLoad() override;

	virtual void UseItem(Player* player) {};

	inline ItemType GetItemType() const { return iType; }

	bool operator==(Item& it) const
	{
		return this->iType == it.iType;
	}

public:
	ItemType iType;

	SDL_Texture* texture;
	SDL_Rect atlasSection;

	bool isDragging; // For inventory
};