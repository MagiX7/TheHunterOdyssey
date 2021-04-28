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

		offsetX += slots[i].bounds.w + 7;
		if (i == 7 || i == 15 || i == 23)
		{
			offsetY += slots[i].bounds.h + 7;
			offsetX = 0;
		}
	}
	slots->itemsAmount = 0;
	slots->filled = false;

	return true;
}

bool Inventory::Update(float dt)
{
	btnEquipment->Update(app->input, dt, -1);
	btnItems->Update(app->input, dt, -1);
	btnWeapons->Update(app->input, dt, -1);

	//int offsetX = 0;
	//int offsetY = 0;
	//for (int i = 0; i < MAX_INVENTORY_SLOTS; i++)
	//{
	//	slots[i].bounds = { 466 - app->render->camera.x + offsetX,369 - app->render->camera.y + offsetY,40,40 };

	//	offsetX += slots[i].bounds.w + 7;
	//	if (i == 7 || i == 15 || i == 23)
	//	{
	//		offsetY += slots[i].bounds.h + 7;
	//		offsetX = 0;
	//	}
	//}

	switch (state)
	{
	case InventoryState::NONE:
		// TODO
		break;

	case InventoryState::EQUIPMENT:
		// TODO
		break;

	case InventoryState::ITEMS:

		for (int i = 0; i < MAX_INVENTORY_SLOTS; i++)
		{
			if ((slots[i].itemsAmount > 0) && (IsMouseInside(slots[i].bounds)))
			{
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				{
					currentSlotId = i;
					state = InventoryState::ITEM_SELECTED;
					break;
				}
			}
		}
		
		break;

	case InventoryState::WEAPONS:
		// TODO
		break;

	case InventoryState::ITEM_SELECTED:
		
		if (!IsMouseInside(tmpBounds) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			state = InventoryState::ITEMS;
		}
		btnUse->Update(app->input, dt, -1);
		btnDelete->Update(app->input, dt, -1);

		break;
	case InventoryState::USE_ITEM:
		btnHunter->Update(app->input, dt, -1);
		btnWizard->Update(app->input, dt, -1);
		btnThief->Update(app->input, dt, -1);
		btnWarrior->Update(app->input, dt, -1);

		break;

	default: break;
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

	// Equipment button
	/*SDL_Rect r = btnEquipment->bounds;
	r.x = btnEquipment->bounds.x - app->render->camera.x;
	r.y = btnEquipment->bounds.y - app->render->camera.y;
	
	app->render->DrawRectangle(r, 220, 220, 220);
	app->render->DrawCenterText(font, "Equipment", btnEquipment->bounds, 40, 5, { 0,0,0,255 });*/


	// Items button
	/*r.x = btnItems->bounds.x - app->render->camera.x;
	r.y = btnItems->bounds.y - app->render->camera.y;

	app->render->DrawRectangle(r, 220, 220, 220);
	app->render->DrawCenterText(font, "Items", btnItems->bounds, 40, 5, { 0,0,0,255 });*/


	// Weapons button
	/*r.x = btnWeapons->bounds.x - app->render->camera.x;
	r.y = btnWeapons->bounds.y - app->render->camera.y;
	
	app->render->DrawRectangle(r, 220, 220, 220);
	app->render->DrawCenterText(font, "Weapons", btnWeapons->bounds, 40, 5, { 0,0,0,255 });*/
	


	// Debug text
	/*r = { 850 , 100, 230, 500 };
	app->render->DrawCenterText(font, "Stats", r, 40, 5, { 0,0,0,255 });*/

	r = { 480, 350, 340, 220 };
	app->render->DrawCenterText(font, "Slots", r, 40, 5, { 0,0,0,255 });

	r = { 450, 100, 400, 250 };
	app->render->DrawCenterText(font, "Pj image", r, 40, 5, { 0,0,0,255 });

	int id = 0;
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		r = { 163, 715, 40, 40};
		app->render->DrawTexture(atlasTexture, slots[i].bounds.x, slots[i].bounds.y, &r, false);

		if (slots[i].itemsAmount > 0)
		{
			SDL_Rect textureRect = { 226,289,32,32 };
			std::string iQuantity = std::to_string(slots[i].itemsAmount);

			app->render->DrawTexture(atlasTexture, slots[i].bounds.x + 4, slots[i].bounds.y + 4, &slots[i].item.atlasSection, false);

			app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 13, (slots[i].bounds.y + slots[i].bounds.h) - 25 + 2, 25, 2, { 0,0,0 });
			app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 15, slots[i].bounds.y + slots[i].bounds.h - 25, 25, 2, { 255,255,255 });
	/*		switch (slots[i].item.iType)
			{
			case ItemType::ULTRA_POTION:
				app->render->DrawTexture(atlasTexture, slots[i].bounds.x + 4, slots[i].bounds.y + 4, &textureRect, false);

				app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 13, (slots[i].bounds.y + slots[i].bounds.h) - 25 + 2, 25, 2, { 0,0,0 });
				app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 15, slots[i].bounds.y + slots[i].bounds.h - 25, 25, 2, { 255,255,255 });

				break;
			case ItemType::POTION:
				app->render->DrawTexture(atlasTexture, slots[i].bounds.x, slots[i].bounds.y, &textureRect, false);

				app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 13, (slots[i].bounds.y + slots[i].bounds.h) - 25 + 2, 25, 2, { 0,0,0 });
				app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 15, slots[i].bounds.y + slots[i].bounds.h - 25, 25, 2, { 255,255,255 });

				break;
			default:

				break;
			}*/
		}
	}

	if(state == InventoryState::ITEM_SELECTED) DisplayText(slots[id].bounds, showColliders);
	else if (state == InventoryState::USE_ITEM)
	{
		tmpBounds = slots[id].bounds;
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
		else if (control->id == 2) state = InventoryState::ITEMS;
		else if (control->id == 3) state = InventoryState::WEAPONS;
		else if (control->id == 4) state = InventoryState::USE_ITEM;
		else if (control->id == 5) state = InventoryState::DELETE_ITEM;
		else if (control->id == 6)
		{
			currentPlayer = GetPlayer(PlayerType::HUNTER);

			slots[currentSlotId].item.UseItem(currentPlayer);
			--slots[currentSlotId].itemsAmount;

			if (slots[currentSlotId].itemsAmount > 0) state = InventoryState::ITEM_SELECTED;
			else state = InventoryState::ITEMS;
		}
		else if (control->id == 7)
		{
			currentPlayer = GetPlayer(PlayerType::WIZARD);

			slots[currentSlotId].item.UseItem(currentPlayer);
			--slots[currentSlotId].itemsAmount;

			if (slots[currentSlotId].itemsAmount > 0) state = InventoryState::ITEM_SELECTED;
			else state = InventoryState::ITEMS;
		}
		else if (control->id == 8)
		{
			currentPlayer = GetPlayer(PlayerType::THIEF);

			slots[currentSlotId].item.UseItem(currentPlayer);
			--slots[currentSlotId].itemsAmount;

			if (slots[currentSlotId].itemsAmount > 0) state = InventoryState::ITEM_SELECTED;
			else state = InventoryState::ITEMS;
		}
		else if (control->id == 9)
		{
			currentPlayer = GetPlayer(PlayerType::WARRIOR);

			slots[currentSlotId].item.UseItem(currentPlayer);
			--slots[currentSlotId].itemsAmount;

			if (slots[currentSlotId].itemsAmount > 0) state = InventoryState::ITEM_SELECTED;
			else state = InventoryState::ITEMS;
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
			break;
		}
		else if (!slots[i].filled)
		{
			slots[i].item = it;
			slots[i].filled = true;
			slots[i].itemsAmount = 1;
			break;
		}
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

Player* Inventory::GetPlayer(PlayerType type)
{
	eastl::list<Player*>::iterator it = players.begin();
	for (; it != players.end(); ++it)
	{
		if ((*it)->playerType == PlayerType::HUNTER) return (*it);
	}
	return nullptr;
}
