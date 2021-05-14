#pragma once

#include "Entity.h"
#include "Point.h"

enum class ObjectType
{
	NONE = 0,
	ITEM,
	ARMOR
};

enum class ItemType
{
	POTION,
	ULTRA_POTION,
	ETER,
	ULTRA_ETER,
	OMNI_POTION,
	FAIRY_TEAR,
	FAIRY_WING,
	FAIRY_BOTTLE,
	ORB,
	ORB_FRAGMENT,
	INTERRUPTOR_BLOCK,
};

class Item : public Entity
{
public:
	Item() {};
	Item(ObjectType type, iPoint pos, SDL_Texture* atlas , EntityType entType = EntityType::ITEM) : Entity(entType), objectType(type), texture(atlas) ,isDragging(false)
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
	virtual void UseItem() {};

	inline ObjectType GetItemType() const { return objectType; }

	bool operator==(Item& it) const
	{
		return this->objectType == it.objectType;
	}

public:
	ObjectType objectType;
	ItemType itemType;

	SDL_Texture* texture;
	SDL_Rect atlasSection;

	bool isDragging; // For inventory
};