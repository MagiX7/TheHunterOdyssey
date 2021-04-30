#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "Inventory.h"

#include "Font.h"
#include "Item.h"

Inventory::Inventory(eastl::list<Player*> pls)
{
	players = pls;
}

Inventory::~Inventory()
{
}

bool Inventory::Load(Font* font)
{
	atlasTexture = app->tex->Load("Assets/Textures/Items/items_atlas_2.png");

	btnEquipment = new GuiButton(1, { 182,100,260,50 }, "Equipment", this, font);
	//btnEquipment->texture = 
	btnItems = new GuiButton(2, { 182,160,260,50 }, "Items", this, font);
	//btnEquipment->texture =
	btnWeapons = new GuiButton(3, { 182,220,260,50 }, "Weapons", this, font);
	//btnEquipment->texture =
	
	btnUse = new GuiButton(4, { 0,0,128,32 }, "USE", this, font);
	btnDelete = new GuiButton(5, { 0,0,128,32 }, "DEL", this, font);
	
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

	buttons.push_back(btnEquipment);
	buttons.push_back(btnItems);
	buttons.push_back(btnWeapons);

	// Initialize items;
	int offsetX = 0;
	int offsetY = 0;

	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		slots[i].bounds = { 466 + offsetX,369 + offsetY,40,40 };
		slots[i].id = i;
		slots[i].state = SlotState::NONE;

		offsetX += slots[i].bounds.w + 7;
		if (i == 7 || i == 15 || i == 23)
		{
			offsetY += slots[i].bounds.h + 7;
			offsetX = 0;
		}
	}

	slots->itemsAmount = 0;
	slots->filled = false;

	state = InventoryState::EQUIPMENT;
	grabbed = false;
	toGrabCount = 0.0f;
	currentSlotId = -1;
	originSlot = nullptr;
	isTextDisplayed = false;

	return true;
}

bool Inventory::Update(float dt)
{
	btnEquipment->Update(app->input, dt, -1);
	btnItems->Update(app->input, dt, -1);
	btnWeapons->Update(app->input, dt, -1);

	switch (state)
	{
	case InventoryState::NONE:
		// TODO
		break;

	case InventoryState::EQUIPMENT:
		// TODO
		break;

	case InventoryState::ITEMS:

		if (!grabbed)
		{
			for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
			{
				if ((slots[i].itemsAmount > 0) && (IsMouseInside(slots[i].bounds)))
				{
					if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
					{
						currentSlotId = i;
						slots[i].state = SlotState::SELECTED;
						isTextDisplayed = true;
						break;
					}
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
					{
						if (slots[i].filled && !isTextDisplayed)
						{
							originSlot = &slots[i];
							slots[i].filled = false;
							slots[i].item.isDragging = true;
							grabbed = true;
							slots[i].state = SlotState::NONE;
							break;
						}
					}
				}
			}
		}
		else
		{
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				for (int j = 0; j < MAX_INVENTORY_SLOTS; ++j)
				{
					if (IsMouseInside(slots[j].bounds) && !slots[j].filled)
					{
						slots[j].item = originSlot->item;
						slots[j].itemsAmount = originSlot->itemsAmount;
						slots[j].filled = true;
						slots[j].item.isDragging = false;
						slots[j].item.bounds = slots[j].bounds;
						originSlot = nullptr;
						slots[j].state = SlotState::UNSELECTED;

						grabbed = false;

						break;
					}
				}
			}
		}
		
		if (grabbed && originSlot != nullptr)
		{
			DragItem(slots[originSlot->id].item);
		}

		break;

	case InventoryState::WEAPONS:
		// TODO
		break;

	}

	if (currentSlotId > -1)
	{
		switch (slots[currentSlotId].state)
		{
		case SlotState::UNSELECTED:
			break;
		case SlotState::SELECTED:
			if (!IsMouseInside(tmpBounds) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				slots[currentSlotId].state = SlotState::UNSELECTED;
				isTextDisplayed = false;
			}
			btnUse->Update(app->input, dt, -1);
			btnDelete->Update(app->input, dt, -1);
			break;

		case SlotState::USE:
			btnHunter->Update(app->input, dt, -1);
			btnWizard->Update(app->input, dt, -1);
			btnThief->Update(app->input, dt, -1);
			btnWarrior->Update(app->input, dt, -1);

		case SlotState::DELETE:
			break;
		}
	}

	return true;
}

void Inventory::Draw(Font* font, bool showColliders)
{
	btnEquipment->Draw(app->render, showColliders);
	btnItems->Draw(app->render, showColliders);
	btnWeapons->Draw(app->render, showColliders);

	// Big rectangle in the background
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 150, true, false);

	// Shape of the inventory
	app->render->DrawRectangle({ 442, 100, 415, 500 }, 205, 205, 205, 200, true, false);

	// Shape of the current elements and the slots
	app->render->DrawRectangle({ 480, 350, 340, 220 }, 225, 225, 225, 200, true, false);

	// Shape where the stats should go
	SDL_Rect r = { 850, 100, 230, 500 };
	app->render->DrawRectangle(r, 240, 240, 240, 255, true, false);
	r.h = 100;
	app->render->DrawCenterText(font, "HUNTER", r, 30, 5, { 0,0,0,255 });
	r.h = 300;
	app->render->DrawCenterText(font, "WIZARD", r, 30, 5, { 0,0,0,255 });
	r.h = 500;
	app->render->DrawCenterText(font, "THIEF", r, 30, 5, { 0,0,0,255 });
	r.h = 700;
	app->render->DrawCenterText(font, "WARRIOR", r, 30, 5, { 0,0,0,255 });

	r = { 480, 350, 340, 220 };
	app->render->DrawCenterText(font, "Slots", r, 40, 5, { 0,0,0,255 });

	r = { 450, 100, 400, 250 };
	app->render->DrawCenterText(font, "Pj image", r, 40, 5, { 0,0,0,255 });


	std::string iQuantity;
	switch (state)
	{
	case InventoryState::EQUIPMENT:
		for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
		{
			r = { 163, 715, 40, 40 };
			app->render->DrawTexture(atlasTexture, slots[i].bounds.x, slots[i].bounds.y, &r, false);

			// Draw Equipment
		}

		break;
	case InventoryState::ITEMS:

		for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
		{
			r = { 163, 715, 40, 40 };
			app->render->DrawTexture(atlasTexture, slots[i].bounds.x, slots[i].bounds.y, &r, false);
			if (showColliders) app->render->DrawRectangle(slots[i].bounds, 255, 0, 0, 120, true, false);

			if (slots[i].itemsAmount > 0)
			{
				SDL_Rect textureRect = { 226,289,32,32 };
				iQuantity = std::to_string(slots[i].itemsAmount);

				// Draw Item
				if(!slots[i].item.isDragging)
				{
					app->render->DrawTexture(atlasTexture, slots[i].bounds.x + 4, slots[i].bounds.y + 4, &slots[i].item.atlasSection, false);

					app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 13, (slots[i].bounds.y + slots[i].bounds.h) - 25 + 2, 25, 2, { 0,0,0 });
					app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 15, slots[i].bounds.y + slots[i].bounds.h - 25, 25, 2, { 255,255,255 });
					if(showColliders) app->render->DrawRectangle(slots[i].item.bounds, 0, 0, 255, 120, true, false);
				}
			}
		}

		if(originSlot != nullptr && grabbed)
		{
			iQuantity = std::to_string(originSlot->itemsAmount);
			app->render->DrawTexture(atlasTexture, originSlot->item.bounds.x + 4, originSlot->item.bounds.y + 4, &originSlot->item.atlasSection, false);
			app->render->DrawText(font, iQuantity.c_str(), (originSlot->item.bounds.x + originSlot->item.bounds.w) - 4, (originSlot->item.bounds.y + originSlot->item.bounds.h) - 16 + 2, 25, 2, { 0,0,0 });
			app->render->DrawText(font, iQuantity.c_str(), (originSlot->item.bounds.x + originSlot->item.bounds.w) - 6, (originSlot->item.bounds.y + originSlot->item.bounds.h) - 16, 25, 2, { 255,255,255 });
			if (showColliders) app->render->DrawRectangle(originSlot->item.bounds, 0, 255, 0, 120, true, false);
		}

		if (currentSlotId > -1)
		{
			if(slots[currentSlotId].state == SlotState::SELECTED)
				DisplayText(slots[currentSlotId].bounds, showColliders);
			else if (slots[currentSlotId].state == SlotState::USE)
			{
				tmpBounds = slots[currentSlotId].bounds;
				SDL_Rect r = { tmpBounds.x, tmpBounds.y, 128, 95 };
				app->render->DrawRectangle(r, 0, 0, 0, 255, true, false);

				btnHunter->bounds.x = tmpBounds.x + 20;
				btnHunter->bounds.y = tmpBounds.y + 5;
				btnHunter->Draw(app->render, showColliders);

				btnWizard->bounds.x = tmpBounds.x + 70;
				btnWizard->bounds.y = tmpBounds.y + 5;
				btnWizard->Draw(app->render, showColliders);

				btnThief->bounds.x = tmpBounds.x + 20;
				btnThief->bounds.y = btnWizard->bounds.y + btnWizard->bounds.h + 5;
				btnThief->Draw(app->render, showColliders);

				btnWarrior->bounds.x = tmpBounds.x + 70;
				btnWarrior->bounds.y = btnWizard->bounds.y + btnWizard->bounds.h + 5;
				btnWarrior->Draw(app->render, showColliders);
			}
		}
		break;

	case InventoryState::WEAPONS:
		for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
		{
			r = { 163, 715, 40, 40 };
			app->render->DrawTexture(atlasTexture, slots[i].bounds.x, slots[i].bounds.y, &r, false);

			// Draw Weapons
		}
		break;
	}
}

bool Inventory::UnLoad()
{
	RELEASE(btnEquipment);
	RELEASE(btnItems);
	RELEASE(btnWeapons);
	RELEASE(btnUse);
	RELEASE(btnDelete);
	RELEASE(btnHunter);
	RELEASE(btnWizard);
	RELEASE(btnThief);
	RELEASE(btnWarrior);

	app->tex->UnLoad(atlasTexture);
	RELEASE(atlasTexture);

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
		else if (control->id == 3) state = InventoryState::WEAPONS;
		else if (control->id == 4)
		{
			//state = InventoryState::USE_ITEM;
			slots[currentSlotId].state = SlotState::USE;
		}
		else if (control->id == 5)
		{
			//state = InventoryState::DELETE_ITEM;
			slots[currentSlotId].state = SlotState::DELETE;
		}
		else if (control->id == 6)
		{
			currentPlayer = GetPlayer(PlayerType::HUNTER);

			slots[currentSlotId].item.UseItem(currentPlayer);
			--slots[currentSlotId].itemsAmount;

			if (slots[currentSlotId].itemsAmount > 0) slots[currentSlotId].state = SlotState::SELECTED;
			else slots[currentSlotId].state = SlotState::UNSELECTED;

			//currentSlotId = -1;
		}
		else if (control->id == 7)
		{
			currentPlayer = GetPlayer(PlayerType::WIZARD);

			slots[currentSlotId].item.UseItem(currentPlayer);
			--slots[currentSlotId].itemsAmount;

			if (slots[currentSlotId].itemsAmount > 0) slots[currentSlotId].state = SlotState::SELECTED;
			else slots[currentSlotId].state = SlotState::UNSELECTED;

			//currentSlotId = -1;
		}
		else if (control->id == 8)
		{
			currentPlayer = GetPlayer(PlayerType::THIEF);

			slots[currentSlotId].item.UseItem(currentPlayer);
			--slots[currentSlotId].itemsAmount;

			if (slots[currentSlotId].itemsAmount > 0) slots[currentSlotId].state = SlotState::SELECTED;
			else slots[currentSlotId].state = SlotState::UNSELECTED;

			//currentSlotId = -1;
		}
		else if (control->id == 9)
		{
			currentPlayer = GetPlayer(PlayerType::WARRIOR);

			slots[currentSlotId].item.UseItem(currentPlayer);
			--slots[currentSlotId].itemsAmount;

			if (slots[currentSlotId].itemsAmount > 0) slots[currentSlotId].state = SlotState::SELECTED;
			else slots[currentSlotId].state = SlotState::UNSELECTED;
			
			//currentSlotId = -1;
		}

		break;
	}

	return false;
}

void Inventory::UpdatingButtons(Input* input)
{
}

void Inventory::AddItem(Item it)
{
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		if (slots[i].item == it)
		{
			slots[i].itemsAmount++;
			slots[i].state = SlotState::UNSELECTED;
			break;
		}
		else if (!slots[i].filled)
		{
			slots[i].item = it;
			slots[i].filled = true;
			slots[i].itemsAmount = 1;
			slots[i].state = SlotState::UNSELECTED;
			break;
		}
		slots[i].item.bounds = slots[i].bounds;
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
	SDL_Rect r = { bounds.x, bounds.y, 128, 95 };
	app->render->DrawRectangle(r, 0, 0, 0, 255, true, false);
	
	// Draw buttons
	btnUse->bounds.x = bounds.x;
	btnUse->bounds.y = bounds.y + 10; // 10 stands for offset
	btnUse->Draw(app->render, showColliders, 32);

	btnDelete->bounds.x = bounds.x;
	btnDelete->bounds.y = btnUse->bounds.y + btnUse->bounds.h + 5; // 5 stands for offset
	btnDelete->Draw(app->render, showColliders, 32);
}

void Inventory::DragItem(Item& item)
{
	int x, y;
	app->input->GetMousePosition(x, y);

	item.bounds.x = x - 16;
	item.bounds.y = y - 16;

	LOG("  =================== ITEM POS: %i    %i  =============== ", item.bounds.x, item.bounds.y);
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