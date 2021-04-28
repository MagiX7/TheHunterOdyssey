#pragma once

#include "Menu.h"
#include "Item.h"

#include "GuiButton.h"

#define MAX_INVENTORY_SLOTS 32
#define MAX_INVENTORY_SLOTS_ITEMS 32

class Input;
class Player;
class SDL_Texture;

enum class InventoryState
{
	NONE = 0,
	EQUIPMENT,
	ITEMS,
	WEAPONS,
	ITEM_SELECTED,
	USE_ITEM,
	DELETE_ITEM,
};

struct InventorySlot
{
	SDL_Rect bounds;
	//Item* items[MAX_INVENTORY_SLOTS_ITEMS];
	Item item;
	int itemsAmount;
	//ItemType itemType;
};

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

	void AddItem(Item it);

	bool IsMouseInside(SDL_Rect r);

	void DisplayText(SDL_Rect bounds, bool showColliders);

private:
	Player* GetPlayer(PlayerType type);

public:
	eastl::list<Player*> players;

	GuiButton* btnEquipment; // Armors
	GuiButton* btnItems;     // Potions etc
	GuiButton* btnWeapons;   // Weapons

private:
	SDL_Texture* atlasTexture;
	SDL_Texture* playersTexture;

	InventoryState state;

	// There will be 32 slots
	InventorySlot slots[MAX_INVENTORY_SLOTS];
	int currentSlotId;
	Player* currentPlayer;

	//bool displayText;

	// Display Text buttons to select with player uses the item
	GuiButton* btnHunter;
	GuiButton* btnWizard;
	GuiButton* btnThief;
	GuiButton* btnWarrior;

	GuiButton* btnUse;       // Use item
	GuiButton* btnDelete;	 // Delete an item.


	SDL_Rect tmpBounds;

};