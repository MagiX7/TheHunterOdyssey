#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "Inventory.h"

#include "Font.h"
#include "Item.h"
#include "UltraPotion.h"
#include "Potion.h"
#include "Orb.h"
#include "OrbFragment.h"
#include "KnightHelmet.h"

Inventory::Inventory(eastl::list<Player*> pls, SDL_Texture* atlas)
{
	players = pls;
	atlasTexture = atlas;
}

Inventory::~Inventory()
{
}

bool Inventory::Load(Font* font)
{
	guiTex = app->tex->Load("Assets/Textures/UI/gui_inventory.png");

	btnEquipment = new GuiButton(1, { 150,116,274,78 }, "Equipment", this, font);
	btnEquipment->alineation = 0;
	btnEquipment->texture = guiTex;
	btnEquipment->section = { 739,0,274,78 };

	btnItems = new GuiButton(2, { 150,204,274,78 }, "Items", this, font);
	btnItems->alineation = 0;
	btnItems->texture = guiTex;
	btnItems->section = { 739,0,274,78 };

	/*btnInfo = new GuiButton(3, { 150,292,274,78 }, "Stats", this, font);
	btnInfo->alineation = 0;
	btnInfo->texture = guiTex;
	btnInfo->section = { 739,0,274,78 };*/
	
	btnHunter = new GuiButton(6, { 0,0, 40,40 }, "", this, font);
	btnHunter->texture = atlasTexture;
	btnHunter->section = { 0,722,32,32 };

	btnWizard = new GuiButton(7, { 0,0, 40,40 }, "", this, font);
	btnWizard->texture = atlasTexture;
	btnWizard->section = { 35,722,32,32 };

	btnThief = new GuiButton(8, { 0,0, 40,40 }, "", this, font);
	btnThief->texture = atlasTexture;
	btnThief->section = { 71,722,32,32 };

	btnWarrior = new GuiButton(9, { 0,0, 40,40 }, "", this, font);
	btnWarrior->texture = atlasTexture;
	btnWarrior->section = { 103,722,25,32 };

	// Next and back character buttons
	btnNext = new GuiButton(10, { 645, 149, 46, 124 }, "", this, font);
	btnNext->texture = guiTex;
	btnNext->section = { 890, 219, 46, 124 };
	btnNext->sectionFocused = { 890,469,46,124 };

	btnPrev = new GuiButton(11, { 469, 149, 46, 124 }, "", this, font);
	btnPrev->texture = guiTex;
	btnPrev->section = { 890, 90, 46, 124 };
	btnPrev->sectionFocused = { 890, 345, 46, 124 };

	btnUse = new GuiButton(4, { 0,0,128,32 }, "USE", this, font);
	btnDelete = new GuiButton(5, { 0,0,128,32 }, "DEL", this, font);

	btnUnEquip = new GuiButton(12, { 0,0,128, 32 }, "UNEQUIP", this, font);
	btnUnEquip->alineation = 1;

	//btnNextStats = new GuiButton(13, {}, "", this, font);
	//btnPrevStats = new GuiButton(14, {}, "", this, font);

	buttons.push_back(btnEquipment);
	buttons.push_back(btnItems);
	//buttons.push_back(btnInfo);

	// Initialize items;
	int offsetX = 0;
	int offsetY = 0;

	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		slots[i].bounds = { 466 + offsetX,369 + offsetY,40,40 };
		slots[i].id = i;
		slots[i].state = SlotState::NONE;
		//slots[i].item = nullptr;

		armorSlots[i].bounds = { 466 + offsetX,369 + offsetY,40,40 };
		armorSlots[i].id = i;
		armorSlots[i].state = SlotState::NONE;

		offsetX += slots[i].bounds.w + 7;
		if (i == 7 || i == 15 || i == 23)
		{
			offsetY += slots[i].bounds.h + 7;
			offsetX = 0;
		}
	}

	currentPlayer = (*players.begin());
	for (int i = 0; i < MAX_EQUIPMENT_SLOTS; ++i)
	{
		equipment[i].bounds = {755, 136 + (i * 55) , 40, 40};
		equipment[i].filled = false;
		equipment[i].id = i;
		equipment[i].state = SlotState::NONE;
	}

	armorSlots->filled = false;
	armorSlots->itemsAmount = 0;

	slots->itemsAmount = 0;
	slots->filled = false;

	state = InventoryState::EQUIPMENT;
	grabbed = false;
	toGrabCount = 0.0f;
	currentSlotId = -1;
	originSlot = nullptr;
	isTextDisplayed = false;
	displayEquipmentMenu = false;

	usingItem = false;

	return true;
}

bool Inventory::Update(float dt)
{
	btnEquipment->Update(app->input, dt, -1);
	btnItems->Update(app->input, dt, -1);
	//btnInfo->Update(app->input, dt, -1);

	// Next and Prev character buttons update
	btnNext->Update(app->input, dt, -1);
	btnPrev->Update(app->input, dt, -1);

	switch (state)
	{
	case InventoryState::NONE:
		// TODO
		break;

	case InventoryState::EQUIPMENT:
		HandleObjects(armorSlots);
		if (currentSlotId > -1) HandleSlot(armorSlots, dt);

		break;

	case InventoryState::ITEMS:
		HandleObjects(slots);
		if (currentSlotId > -1) HandleSlot(slots, dt);

		break;

	case InventoryState::STATS:
		// TODO
		break;

	}

	if (state != InventoryState::STATS) HandleEquipment(dt);

	return true;
}

void Inventory::Draw(Font* font, bool showColliders)
{
	// Big rectangle in the background
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 150, true, false);

	// Shape of the inventory
	SDL_Rect section;
	if (state != InventoryState::STATS)
	{
		section = { 0, 0, 739, 597 };
		app->render->DrawTexture(guiTex, 406, 53, &section, false);
	}
	else if (state == InventoryState::STATS)
	{
		section = { 1144, 0, 739, 597 };
		app->render->DrawTexture(guiTex, 406, 53, &section, false);
	}

	btnEquipment->Draw(app->render, showColliders, 36);
	btnItems->Draw(app->render, showColliders, 36);
	//btnInfo->Draw(app->render, showColliders, 36);

	std::string iQuantity;
	switch (state)
	{
	case InventoryState::EQUIPMENT:
		DrawObjects(armorSlots, font, showColliders);
		app->render->DrawRectangle(btnItems->bounds, 0, 0, 0, 150, true, false);
		break;
	case InventoryState::ITEMS:
		DrawObjects(slots, font, showColliders);
		app->render->DrawRectangle(btnEquipment->bounds, 0, 0, 0, 150, true, false);
		break;
	case InventoryState::STATS:
		break;
	}

	if (state != InventoryState::STATS)
	{
		// Shape where the stats should go
		SDL_Rect r;

		// Equipment equiped drawing
		for (int i = 0; i < MAX_EQUIPMENT_SLOTS; ++i)
		{
			r = { 163, 715, 40, 40 };
			app->render->DrawTexture(atlasTexture, equipment[i].bounds.x, equipment[i].bounds.y, &r, false);
			if (equipment[i].item != nullptr) app->render->DrawTexture(atlasTexture, equipment[i].bounds.x + 4, equipment[i].bounds.y + 4, &equipment[i].item->atlasSection, false);
		}

		r = { 890, 130, 196, 50 };
		app->render->DrawCenterText(font, "HUNTER", r, 30, 5, { 255,255,255,255 });
		r.y = 235;
		app->render->DrawCenterText(font, "WIZARD", r, 30, 5, { 255,255,255,255 });
		r.y = 340;
		app->render->DrawCenterText(font, "THIEF", r, 30, 5, { 255,255,255,255 });
		r.y = 445;
		app->render->DrawCenterText(font, "WARRIOR", r, 30, 5, { 255,255,255,255 });

		// Draw current player
		SDL_Rect rectan;
		SDL_Rect rectan2 = { 522,149,116,124 };
		SDL_Rect rectan3;
		app->render->DrawRectangle(rectan2, 205, 205, 205, 200, true, false);

		if (currentPlayer->playerType == PlayerType::HUNTER)
		{
			rectan = { 739,103,106,113 };
			app->render->DrawTexture(guiTex, 527, 154, &rectan, false);
			rectan3 = { 885, 120, 206, 110 };
			app->render->DrawRectangle(rectan3, 255, 255, 255, 255, false, false);
		}
		else if (currentPlayer->playerType == PlayerType::WIZARD)
		{
			rectan = { 739, 217,106,113 };
			app->render->DrawTexture(guiTex, 527, 154, &rectan, false);
			rectan3 = { 885, 225, 206, 110 };
			app->render->DrawRectangle(rectan3, 255, 255, 255, 255, false, false);
		}
		else if (currentPlayer->playerType == PlayerType::THIEF)
		{
			rectan = { 739,331,106,113 };
			app->render->DrawTexture(guiTex, 527, 154, &rectan, false);
			rectan3 = { 885, 330, 206, 110 };
			app->render->DrawRectangle(rectan3, 255, 255, 255, 255, false, false);
		}
		else if (currentPlayer->playerType == PlayerType::WARRIOR)
		{
			rectan = { 739,445,106,113 };
			app->render->DrawTexture(guiTex, 527, 154, &rectan, false);
			rectan3 = { 885, 435, 206, 110 };
			app->render->DrawRectangle(rectan3, 255, 255, 255, 255, false, false);
		}

		btnNext->Draw(app->render, showColliders);
		btnPrev->Draw(app->render, showColliders);

		// Stats drawing

		eastl::list<Player*>::iterator it = players.begin();

		for (int i = 0; i < 4; ++i, ++it)
		{
			Player* pl = (*it);

			// Color bars
			// Black bar
			SDL_Rect statsBar = { 225, 797, 196, 9 };
			for (int j = 0; j < 3; ++j)
			{
				app->render->DrawTexture(atlasTexture, 890, 173 + (j * 14) + (i * 105), &statsBar, false);
			}

			int currHealth = (*it)->GetHealthPoints();
			int maxHealth = (*it)->GetMaxHealthPoints();

			if (currHealth < (maxHealth / 4))
				statsBar = { 228, 750, (190 * currHealth) / maxHealth, 5 };
			else if (currHealth < (maxHealth / 2))
				statsBar = { 228, 737, (190 * currHealth) / maxHealth, 5 };
			else
				statsBar = { 228, 725, (190 * currHealth) / maxHealth, 5 };

			// Green Bar
			app->render->DrawTexture(atlasTexture, 893, 175 + (i * 105), &statsBar, false);

			// Blue Bar
			statsBar = { 228, 764, (190 * pl->GetManaPoints()) / (pl->GetMaxManaPoints()), 5 };
			app->render->DrawTexture(atlasTexture, 893, 189 + (i * 105), &statsBar, false);

			// Yellow Bar
			statsBar = { 228, 778, (190 * pl->GetArmorPoints()) / (pl->GetMaxArmorPoints()), 5 };
			app->render->DrawTexture(atlasTexture, 893, 203 + (i * 105), &statsBar, false);
		}
	}

	if (displayEquipmentMenu && state != InventoryState::STATS) DisplayMenuEquipment(showColliders);
}

bool Inventory::UnLoad()
{
	RELEASE(btnEquipment);
	RELEASE(btnItems);
	//RELEASE(btnInfo);
	RELEASE(btnUse);
	RELEASE(btnDelete);
	RELEASE(btnHunter);
	RELEASE(btnWizard);
	RELEASE(btnThief);
	RELEASE(btnWarrior);
	RELEASE(btnNext);
	RELEASE(btnPrev);
	RELEASE(btnUnEquip);

	app->tex->UnLoad(atlasTexture);
	//RELEASE(atlasTexture);
	app->tex->UnLoad(guiTex);

	buttons.clear();
	players.clear();

	return true;
}

bool Inventory::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:

		if (control->id == 1) state = InventoryState::EQUIPMENT;
		else if (control->id == 2)
			state = InventoryState::ITEMS;
		else if (control->id == 3) state = InventoryState::STATS;
		else if (control->id == 4) // Use
		{
			if (state == InventoryState::ITEMS) slots[currentSlotId].state = SlotState::USE;
			else if (state == InventoryState::EQUIPMENT) armorSlots[currentSlotId].state = SlotState::USE;
			
		}
		else if (control->id == 5) // Delete
		{
			if (state == InventoryState::ITEMS) slots[currentSlotId].state = SlotState::DELETE;
			else if (state == InventoryState::EQUIPMENT) armorSlots[currentSlotId].state = SlotState::DELETE;
		}
		else if (control->id == 6) // Hunter
		{
			switch (state)
			{
			case InventoryState::ITEMS:
				UseObject(slots, GetPlayer(PlayerType::HUNTER));
				break;

			case InventoryState::EQUIPMENT:
				Player* tmpPlayer = GetPlayer(PlayerType::HUNTER);
				tmpPlayer->SetEquipment((Armor*)armorSlots[currentSlotId].item);
				
				if (currentPlayer == tmpPlayer) GetEquipment(tmpPlayer);
				
				UseObject(armorSlots, tmpPlayer);
				break;
			}
		}
		else if (control->id == 7)
		{
			switch (state)
			{
			case InventoryState::ITEMS:
				UseObject(slots, GetPlayer(PlayerType::WIZARD));
				break;

			case InventoryState::EQUIPMENT:
				Player* tmpPlayer = GetPlayer(PlayerType::WIZARD);
				tmpPlayer->SetEquipment((Armor*)armorSlots[currentSlotId].item);

				if (currentPlayer == tmpPlayer) GetEquipment(tmpPlayer);

				UseObject(armorSlots, tmpPlayer);
				break;
			}
		}
		else if (control->id == 8)
		{
			switch (state)
			{
			case InventoryState::ITEMS:
				UseObject(slots, GetPlayer(PlayerType::THIEF));
				break;

			case InventoryState::EQUIPMENT:
				Player* tmpPlayer = GetPlayer(PlayerType::THIEF);
				tmpPlayer->SetEquipment((Armor*)armorSlots[currentSlotId].item);

				if (currentPlayer == tmpPlayer) GetEquipment(tmpPlayer);

				UseObject(armorSlots, tmpPlayer);
				break;
			}
		}
		else if (control->id == 9)
		{
			switch (state)
			{
			case InventoryState::ITEMS:
				UseObject(slots, GetPlayer(PlayerType::WARRIOR));
				break;

			case InventoryState::EQUIPMENT:
				Player* tmpPlayer = GetPlayer(PlayerType::WARRIOR);
				tmpPlayer->SetEquipment((Armor*)armorSlots[currentSlotId].item);

				if (currentPlayer == tmpPlayer) GetEquipment(tmpPlayer);

				UseObject(armorSlots, tmpPlayer);
				break;
			}
		}
		else if (control->id == 10)
		{
			eastl::list<Player*>::iterator it = players.begin();
			eastl::list<Player*>::iterator itEnd = players.end().prev();
			for (; it != itEnd; ++it)
			{
				if ((*it) == currentPlayer && (*it.next()) != nullptr)
				{
					currentPlayer = *(it.next());
					GetEquipment(currentPlayer);
					break;
				}
			}
		}
		else if (control->id == 11)
		{
			eastl::list<Player*>::iterator it = players.end().prev();
			eastl::list<Player*>::iterator itBegin = players.begin();
			for (; it != itBegin; --it)
			{
				if ((*it) == currentPlayer && (*it.prev()) != nullptr)
				{
					currentPlayer = *(it.prev());
					GetEquipment(currentPlayer);
					break;
				}
			}
		}
		else if (control->id == 12)
		{
			AddItem(equipment[currentEquipmentId].item);
			currentPlayer->UnequipArmor((Armor*)equipment[currentEquipmentId].item);
			equipment[currentEquipmentId].item = nullptr;
			equipment[currentEquipmentId].itemsAmount--;
			equipment[currentEquipmentId].filled = false;
			displayEquipmentMenu = false;
		}
		break;
	}

	return false;
}

void Inventory::UpdatingButtons(Input* input)
{
}

void Inventory::AddItem(Item *it)
{
	switch ((*it).objectType)
	{
	case ObjectType::ARMOR:
		for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
		{
			if (armorSlots[i].item != nullptr && armorSlots[i].item == it && armorSlots[i].itemsAmount <= ITEM_STACK)
			{
				RELEASE(it);
				armorSlots[i].itemsAmount++;
				armorSlots[i].state = SlotState::UNSELECTED;
				armorSlots[i].item->bounds = armorSlots[i].bounds;
				break;
			}
			else if (armorSlots[i].item == nullptr)
			{
				armorSlots[i].item = it;
				armorSlots[i].item->isDragging = false;
				armorSlots[i].filled = true;
				armorSlots[i].itemsAmount = 1;
				armorSlots[i].state = SlotState::UNSELECTED;
				armorSlots[i].item->bounds = armorSlots[i].bounds;
				break;
			}
		}
		break;
	case ObjectType::ITEM:
		for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
		{
			if (slots[i].item != nullptr && slots[i].item->itemType == (it)->itemType && slots[i].itemsAmount <= ITEM_STACK)
			{
				slots[i].itemsAmount++;
				if (it->itemType == ItemType::ORB_FRAGMENT) CompleteOrb(i);
				RELEASE(it);
				slots[i].state = SlotState::UNSELECTED;
				slots[i].item->bounds = slots[i].bounds;
				break;
			}
			else if (slots[i].item == nullptr)
			{
				slots[i].item = it;
				slots[i].filled = true;
				slots[i].itemsAmount = 1;
				slots[i].state = SlotState::UNSELECTED;
				slots[i].item->bounds = slots[i].bounds;
				break;
			}
		}
		break;
	}
}

bool Inventory::IsMouseInside(SDL_Rect r)
{
	int x, y;
	app->input->GetMousePosition(x, y);

	return (x > r.x) && (x < r.x + r.w) && (y > r.y) && (y < r.y + r.h);
}

void Inventory::DisplayText(SDL_Rect bounds, bool showColliders)
{
	tmpBounds = bounds;

	SDL_Rect r = { bounds.x + 20, bounds.y + 20, 128, 95 };
	app->render->DrawRectangle(r, 0, 0, 0, 255, true, false);
	
	// Draw buttons
	btnUse->bounds.x = r.x;
	btnUse->bounds.y = r.y + 10; // 10 stands for offset
	btnUse->Draw(app->render, showColliders, 32);

	btnDelete->bounds.x = r.x;
	btnDelete->bounds.y = btnUse->bounds.y + btnUse->bounds.h + 5; // 5 stands for offset
	btnDelete->Draw(app->render, showColliders, 32);

	tmpUsingBounds = r;
}

void Inventory::DragItem(Item& item)
{
	int x, y;
	app->input->GetMousePosition(x, y);

	item.bounds.x = x - 16;
	item.bounds.y = y - 16;
}

Player* Inventory::GetPlayer(PlayerType type)
{
	eastl::list<Player*>::iterator it = players.begin();
	for (; it != players.end(); ++it)
	{
		if ((*it)->playerType == type) return (*it);
	}
	return nullptr;
}

void Inventory::GetEquipment(Player* player)
{
	equipment[0].item = player->GetHelmet();
	equipment[1].item = player->GetChest();
	equipment[2].item = player->GetBoots();
	for (int i = 0; i < MAX_EQUIPMENT_SLOTS; ++i)
	{
		if (equipment[i].item != nullptr)
		{
			equipment[i].filled = true;
			equipment[i].itemsAmount = 1;
		}
		else
		{
			equipment[i].filled = false;
			equipment[i].itemsAmount = 0;
		}
	}
}

void Inventory::HandleEquipment(float dt)
{
	if (displayEquipmentMenu == false)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
		{
			for (int i = 0; i < MAX_EQUIPMENT_SLOTS; i++)
			{
				if (IsMouseInside(equipment[i].bounds) && equipment[i].filled == true)
				{
					displayEquipmentMenu = true;
					tmpEquipMenuBounds = { equipment[i].bounds.x + 20, equipment[i].bounds.y + 20, 128, 50 };
					currentEquipmentId = i;
					break;
				}
			}
		}
	}
	else
	{
		if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !IsMouseInside(tmpEquipMenuBounds)) 
			displayEquipmentMenu = false;

		btnUnEquip->Update(app->input, dt, 1);
	}
}

void Inventory::DisplayMenuEquipment(bool showColliders)
{
	app->render->DrawRectangle(tmpEquipMenuBounds, 0, 0, 0, 255, true, false);

	
	btnUnEquip->bounds.x = tmpEquipMenuBounds.x;
	btnUnEquip->bounds.y = tmpEquipMenuBounds.y + tmpEquipMenuBounds.h / 4;

	btnUnEquip->Draw(app->render, showColliders, 32);
}

void Inventory::CompleteOrb(int index)
{
	RELEASE(slots[index].item);
	orb = new Orb(atlasTexture);
	slots[index].item = orb;
	slots[index].itemsAmount = 1;
}

void Inventory::HandleObjects(InventorySlot objects[])
{
	if (!grabbed)
	{
		for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
		{
			if ((objects[i].itemsAmount > 0) && (IsMouseInside(objects[i].bounds)))
			{
				// Pop the Use and Delete menu
				if (!isTextDisplayed && app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
				{
					currentSlotId = i;
					objects[i].state = SlotState::SELECTED;
					isTextDisplayed = true;
					usingItem = true;
					break;
				}
				// Drag Items
				if (usingItem == false && !isTextDisplayed && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				{
					if (objects[i].item != nullptr && objects[i].filled)
					{
						originSlot = &objects[i];
						objects[i].filled = false;
						objects[i].item->isDragging = true;
						grabbed = true;
						objects[i].state = SlotState::NONE;
						break;
					}
				}
			}
		}
	}
	else
	{
		// Drop the item to a new Inventory Slot
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			for (int j = 0; j < MAX_INVENTORY_SLOTS; ++j)
			{
				if (IsMouseInside(objects[j].bounds) && !objects[j].filled)
				{
					objects[j].item = originSlot->item;
					objects[j].itemsAmount = originSlot->itemsAmount;
					objects[j].filled = true;
					objects[j].item->isDragging = false;
					objects[j].item->bounds = objects[j].bounds;
					objects[j].state = SlotState::UNSELECTED;

					if (slots[j].id != originSlot->id)
					{
						objects[originSlot->id].item->isDragging = false;
						objects[originSlot->id].filled = false;
						objects[originSlot->id].item = nullptr;
					}

					originSlot = nullptr;

					grabbed = false;

					break;
				}
			}
		}
	}

	// Drag the item. This is done here because otherwise there is one frame that the item's position is not uploaded
	if (grabbed && originSlot != nullptr)
	{
		DragItem(*objects[originSlot->id].item);
	}
}

void Inventory::DrawObjects(InventorySlot objects[], Font *font, bool showColliders)
{
	SDL_Rect r{ 0,0,0,0 };

	std::string iQuantity;

	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		r = { 163, 715, 40, 40 };
		app->render->DrawTexture(atlasTexture, objects[i].bounds.x, objects[i].bounds.y, &r, false);
		if (showColliders) app->render->DrawRectangle(objects[i].bounds, 255, 0, 0, 120, true, false);

		if (objects[i].itemsAmount > 0)
		{
			SDL_Rect textureRect = { 226,289,32,32 };
			iQuantity = std::to_string(objects[i].itemsAmount);

			// Draw Item
			if (objects[i].item != nullptr && !objects[i].item->isDragging)
			{
				app->render->DrawTexture(atlasTexture, objects[i].bounds.x + 4, objects[i].bounds.y + 4, &objects[i].item->atlasSection, false);

				app->render->DrawText(font, iQuantity.c_str(), (objects[i].bounds.x + objects[i].bounds.w) - 13, (objects[i].bounds.y + objects[i].bounds.h) - 25 + 2, 25, 2, { 0,0,0 });
				app->render->DrawText(font, iQuantity.c_str(), (objects[i].bounds.x + objects[i].bounds.w) - 15, (objects[i].bounds.y + objects[i].bounds.h) - 25, 25, 2, { 255,255,255 });
				if (showColliders) app->render->DrawRectangle(objects[i].item->bounds, 0, 0, 255, 120, true, false);
			}

			if (IsMouseInside(objects[i].bounds) && !isTextDisplayed)
			{
				app->render->DrawRectangle(objects[i].bounds, 200, 200, 200, 50, true, false);
			}
		}
	}

	// Draw the grabbed object
	if (originSlot != nullptr && grabbed)
	{
		iQuantity = std::to_string(originSlot->itemsAmount);
		app->render->DrawTexture(atlasTexture, originSlot->item->bounds.x + 4, originSlot->item->bounds.y + 4, &originSlot->item->atlasSection, false);
		app->render->DrawText(font, iQuantity.c_str(), (originSlot->item->bounds.x + originSlot->item->bounds.w) - 13, (originSlot->item->bounds.y + originSlot->item->bounds.h) - 25 + 2, 25, 2, { 0,0,0 });
		app->render->DrawText(font, iQuantity.c_str(), (originSlot->item->bounds.x + originSlot->item->bounds.w) - 15, (originSlot->item->bounds.y + originSlot->item->bounds.h) - 25, 25, 2, { 255,255,255 });
		if (showColliders) app->render->DrawRectangle(originSlot->item->bounds, 0, 255, 0, 120, true, false);
	}

	if (currentSlotId > -1)
	{
		if (objects[currentSlotId].state == SlotState::SELECTED)
			DisplayText(objects[currentSlotId].bounds, showColliders);
		else if (objects[currentSlotId].state == SlotState::USE)
		{
			tmpBounds = objects[currentSlotId].bounds;
			SDL_Rect r = { tmpBounds.x + 20, tmpBounds.y + 20, 128, 95 };
			app->render->DrawRectangle(r, 0, 0, 0, 255, true, false);

			btnHunter->bounds.x = r.x + 20;
			btnHunter->bounds.y = r.y + 5;
			btnHunter->Draw(app->render, showColliders);

			btnWizard->bounds.x = r.x + 70;
			btnWizard->bounds.y = r.y + 5;
			btnWizard->Draw(app->render, showColliders);

			btnThief->bounds.x = r.x + 20;
			btnThief->bounds.y = btnWizard->bounds.y + btnWizard->bounds.h + 5;
			btnThief->Draw(app->render, showColliders);

			btnWarrior->bounds.x = r.x + 70;
			btnWarrior->bounds.y = btnWizard->bounds.y + btnWizard->bounds.h + 5;
			btnWarrior->Draw(app->render, showColliders);

			tmpCharBounds = r;
		}
	}
}

void Inventory::HandleSlot(InventorySlot objects[], float dt)
{
	switch (objects[currentSlotId].state)
	{
	case SlotState::UNSELECTED:
		break;
	case SlotState::SELECTED:
		if (!IsMouseInside(tmpCharBounds) && !IsMouseInside(tmpUsingBounds) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			objects[currentSlotId].state = SlotState::UNSELECTED;
			isTextDisplayed = false;
			usingItem = false;
		}
		btnUse->Update(app->input, dt, -1);
		btnDelete->Update(app->input, dt, -1);
		break;

	case SlotState::USE:
		if (!IsMouseInside(tmpCharBounds) && !IsMouseInside(tmpUsingBounds) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			objects[currentSlotId].state = SlotState::UNSELECTED;
			isTextDisplayed = false;
			usingItem = false;
		}
		btnHunter->Update(app->input, dt, -1);
		btnWizard->Update(app->input, dt, -1);
		btnThief->Update(app->input, dt, -1);
		btnWarrior->Update(app->input, dt, -1);
		break;

	case SlotState::DELETE:
		objects[currentSlotId].itemsAmount--;
		objects[currentSlotId].state = SlotState::UNSELECTED;
		isTextDisplayed = false;
		usingItem = false;
		break;
	}
}

void Inventory::UseObject(InventorySlot objects[], Player* player)
{
	objects[currentSlotId].item->UseItem(player);
	--objects[currentSlotId].itemsAmount;

	if (objects[currentSlotId].itemsAmount > 0)
	{
		objects[currentSlotId].state = SlotState::SELECTED;
		isTextDisplayed = true;
	}
	else
	{
		objects[currentSlotId].state = SlotState::UNSELECTED;
		objects[currentSlotId].filled = false;
		isTextDisplayed = false;
		if (objects[currentSlotId].item->objectType == ObjectType::ARMOR)
		{
			objects[currentSlotId].item = nullptr;
		}
	}
}

void Inventory::UseObject(ItemType itemType)
{
	int aux;
	int aux2;
	int counter = 0;
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		if (slots[i].item != nullptr && slots[i].item->itemType == itemType)
		{
			aux = ObjectQuantity(itemType) / ITEM_STACK;
			aux2 = ObjectQuantity(itemType) - (aux * ITEM_STACK);

			if (aux == 0 && aux2 == slots[i].itemsAmount)
			{
				slots[i].itemsAmount--;

				if (slots[i].itemsAmount <= 0)
				{
					slots[i].state = SlotState::UNSELECTED;
					slots[i].filled = false;
					isTextDisplayed = false;
					slots[i].item = nullptr;

					break;
				}
			}
			else if (aux > 0 && slots[i].itemsAmount == aux2)
			{
				slots[i].itemsAmount--;

				if (slots[i].itemsAmount <= 0)
				{
					slots[i].state = SlotState::UNSELECTED;
					slots[i].filled = false;
					isTextDisplayed = false;
					slots[i].item = nullptr;

					break;
				}
			}
			else if (aux > 0 && slots[i].itemsAmount == ITEM_STACK)
			{
				counter++;

				if (aux == counter && aux2 == 0)
				{
					slots[i].itemsAmount--;

					if (slots[i].itemsAmount <= 0)
					{
						slots[i].state = SlotState::UNSELECTED;
						slots[i].filled = false;
						isTextDisplayed = false;
						slots[i].item = nullptr;

						break;
					}
				}
			}

		}
	}
}

int Inventory::ObjectQuantity(ItemType itemType)
{
	int counter = 0;
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		if (slots[i].item != nullptr && slots[i].item->itemType == itemType)
		{
			counter++;
		}
		else if (armorSlots[i].item != nullptr && armorSlots[i].item->itemType == itemType)
		{
			counter++;
		}
	}
	return counter;
}

void Inventory::HandleStatsInfo()
{
}

void Inventory::DrawStatsInfo(bool showColliders)
{
}
