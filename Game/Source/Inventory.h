#pragma once

#include "Menu.h"

#include "GuiButton.h"

#define MAX_INVENTORY_SLOTS 32
#define MAX_INVENTORY_SLOTS_ITEMS 32

class Input;
class Player;
class Item;

enum class InventoryState
{
	NONE = 0,
	EQUIPMENT,
	ITEMS,
	WEAPONS
};

//struct InventorySlot
//{
//	SDL_Rect bounds;
//	//Item* items[MAX_INVENTORY_SLOTS_ITEMS];
//	eastl::list<Item*> items;
//};

class Inventory : public Menu
{
public:
	Inventory(eastl::list<Player*> pls);
	virtual ~Inventory();

	bool Load(Font* font) override;
	bool Update(float dt) override;
	void Draw(Font* font, bool showColliders) override;
	bool UnLoad() override;
	bool OnGuiMouseClickEvent(GuiControl* control) override;
	void UpdatingButtons(Input* input) override;

public:
	eastl::list<Player*> players;

	GuiButton* btnEquipment; // Armors
	GuiButton* btnItems;     // Potions etc
	GuiButton* btnWeapons;   // Weapons


private:
	InventoryState state;

	// There will be 32 slots
	//InventorySlot slots[MAX_INVENTORY_SLOTS];
	eastl::list<Item*> slots;
};