#pragma once

#include "Item.h"
#include "Point.h"

enum class ArmorType
{
	NONE = 0,
	HELMET,
	CHEST,
	BOOTS
};

class Armor : public Item
{
public:
	Armor(iPoint pos, SDL_Texture* atlas) : Item(ObjectType::ARMOR, pos, atlas) {}
	virtual~Armor(){}

protected:
	ArmorType armorType;

	int durability;
	int defenseAmount;

};