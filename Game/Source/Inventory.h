#pragma once

#include "Menu.h"
#include "Item.h"

#include "GuiButton.h"

#include <iostream>

#define MAX_INVENTORY_SLOTS 32
#define MAX_EQUIPMENT_SLOTS 3
#define ITEM_STACK 32

class Input;
class Player;
class SDL_Texture;
class Orb;
class Easing;
enum class PlayerType;

enum class SlotState
{
	NONE = 0,
	UNSELECTED,
	SELECTED,
	FOCUSED,
	USE,
	DELETE
};

enum class InventoryState
{
	NONE = 0,
	EQUIPMENT,
	ITEMS,
	STATS,
};

struct InventorySlot
{
	SDL_Rect bounds;
	Item* item;
	int itemsAmount;
	bool filled;
	int id;
	SlotState state;

	bool operator==(InventorySlot& i) const
	{
		return (i.bounds.x == bounds.x && i.bounds.y == bounds.y && i.bounds.w == bounds.w && i.bounds.h == bounds.h &&
			i.item->objectType == item->objectType && i.itemsAmount == itemsAmount && i.filled == filled);
	}
	bool operator!=(InventorySlot& i) const
	{
		return (i.bounds.x != bounds.x || i.bounds.y != bounds.y || i.bounds.w != bounds.w || i.bounds.h != bounds.h ||
			i.item->objectType != item->objectType || i.itemsAmount != itemsAmount || i.filled != filled);
	}
};

class Inventory : public Menu
{
public:
	Inventory(eastl::list<Player*> pls, SDL_Texture* atlas);
	virtual ~Inventory();

	bool Load(Font* font) override;
	bool Update(float dt) override;
	void Draw(Font* font, bool showColliders) override;
	bool UnLoad() override;
	bool OnGuiMouseClickEvent(GuiControl* control) override;
	void UpdatingButtons(Input* input) override;

	void LoadState(pugi::xml_node& node);
	void SaveState(pugi::xml_node& node);

	void AddItem(Item *it);

	bool IsMouseInside(SDL_Rect r);

	void DisplayText(SDL_Rect bounds, bool showColliders);

	void DragItem(Item& item);

	void GetEquipment(Player* player);

	void HandleEquipment(float dt);

	void DisplayMenuEquipment(bool showColliders);

	void CompleteOrb(int index);

	int ObjectQuantity(ItemType itemType);

	void UseObject(ItemType itemType);

	inline void SetPlayersList(eastl::list<Player*> pl) 
	{
		players.clear();
		players = pl; 
		currentPlayer = (*players.begin());
	}

	void ResetStates();

private:
	Player* GetPlayer(PlayerType type);

	void HandleObjects(InventorySlot objects[]);

	void DrawObjects(InventorySlot objects[], Font* font, bool showColliders);

	void HandleSlot(InventorySlot objects[], float dt);

	void UseObject(InventorySlot objects[], Player* player);

	void HandleStatsInfo();

	void DrawStatsInfo(bool showColliders);

public:
	eastl::list<Player*> players;

	GuiButton* btnEquipment; // Armors
	GuiButton* btnItems;     // Potions etc

private:
	SDL_Texture* atlasTexture;
	SDL_Texture* guiTex;

	InventoryState state;

	// Armor and weapons equiped slots
	InventorySlot equipment[MAX_EQUIPMENT_SLOTS];
	bool inEquipment;

	// There will be 32 slots for items
	InventorySlot slots[MAX_INVENTORY_SLOTS];
	int currentSlotId;
	InventorySlot* originSlot;

	// There will be 32 slots for Armor
	InventorySlot armorSlots[MAX_INVENTORY_SLOTS];
	int currentArmorSlotId;
	InventorySlot* originArmorSlot;

	Player* currentPlayer;

	Orb* orb;

	// Display Text buttons to select with player uses the item
	/*GuiButton* btnHunter;
	GuiButton* btnWizard;
	GuiButton* btnThief;
	GuiButton* btnWarrior;*/

	GuiButton* btnUse;       // Use item
	GuiButton* btnDelete;	 // Delete an item
	//GuiButton* btnEquip;	// Equips an armor
	GuiButton* btnUnEquip;	// Unequip an armor

	// Use this buttons for going through characters
	GuiButton* btnNext;
	GuiButton* btnPrev;
	GuiButton* btnNextStats;
	GuiButton* btnPrevStats;

	SDL_Rect tmpBounds;
	SDL_Rect tmpUsingBounds;
	SDL_Rect tmpCharBounds;
	SDL_Rect tmpEquipMenuBounds;

	Easing* easing;

	bool grabbed;
	float toGrabCount;
	bool isTextDisplayed;
	bool usingItem;
	bool displayEquipmentMenu;
	int currentEquipmentId;
};