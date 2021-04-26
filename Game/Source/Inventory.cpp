#include "App.h"
#include "Textures.h"
#include "Render.h"

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
	atlasTexture = app->tex->Load("Assets/Textures/Items/items_atlas.png");

	btnEquipment = new GuiButton(1, { 182,100,260,50 }, "Equipment", this, font);
	btnItems = new GuiButton(2, { 182,160,260,50 }, "Items", this, font);
	btnWeapons = new GuiButton(3, { 182,220,260,50 }, "Weapons", this, font);

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

	//eastl::list<InventorySlot*>::iterator it = slots.begin();


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
		// TODO
		break;

	case InventoryState::WEAPONS:
		// TODO
		break;

	default: break;
	}



	return true;
}

void Inventory::Draw(Font* font, bool showColliders)
{
	// Big rectangle in the background
	app->render->DrawRectangle({ 0 - app->render->camera.x,0 - app->render->camera.y,1280,720 }, 0, 0, 0, 150);

	// Shape of the inventory
	app->render->DrawRectangle({ 442 - app->render->camera.x, 100 - app->render->camera.y, 415, 500 }, 205, 205, 205, 200);

	// Shape of the current elements and the slots
	app->render->DrawRectangle({ 480 - app->render->camera.x, 350 - app->render->camera.y, 340, 220 }, 225, 225, 225, 200);

	// Shape where the stats should go
	app->render->DrawRectangle({ 850 - app->render->camera.x, 100 - app->render->camera.y, 230, 500 }, 240, 240, 240);


	// Equipment button
	SDL_Rect r = btnEquipment->bounds;
	r.x = btnEquipment->bounds.x - app->render->camera.x;
	r.y = btnEquipment->bounds.y - app->render->camera.y;
	
	app->render->DrawRectangle(r, 220, 220, 220);
	app->render->DrawCenterText(font, "Equipment", btnEquipment->bounds, 40, 5, { 0,0,0,255 });


	// Items button
	r.x = btnItems->bounds.x - app->render->camera.x;
	r.y = btnItems->bounds.y - app->render->camera.y;

	app->render->DrawRectangle(r, 220, 220, 220);
	app->render->DrawCenterText(font, "Items", btnItems->bounds, 40, 5, { 0,0,0,255 });


	// Weapons button
	r.x = btnWeapons->bounds.x - app->render->camera.x;
	r.y = btnWeapons->bounds.y - app->render->camera.y;
	
	app->render->DrawRectangle(r, 220, 220, 220);
	app->render->DrawCenterText(font, "Weapons", btnWeapons->bounds, 40, 5, { 0,0,0,255 });
	


	// Debug text
	r = { 850 , 100, 230, 500 };
	app->render->DrawCenterText(font, "Stats", r, 40, 5, { 0,0,0,255 });

	r = { 480, 350, 340, 220 };
	app->render->DrawCenterText(font, "Slots", r, 40, 5, { 0,0,0,255 });

	r = { 450, 100, 400, 250 };
	app->render->DrawCenterText(font, "Pj image", r, 40, 5, { 0,0,0,255 });

	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		SDL_Rect textureRect = { 228,290,26,28 };
		std::string iQuantity = std::to_string(slots[i].itemsAmount);
		switch (slots[i].itemType)
		{
		case ItemType::ULTRA_POTION:
			app->render->DrawTexture(atlasTexture, slots[i].bounds.x, slots[i].bounds.y, &textureRect);
			
			app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 15 + 2, (slots[i].bounds.y + slots[i].bounds.h) - 25 + 2, 25, 2, { 0,0,0});
			app->render->DrawText(font, iQuantity.c_str(), (slots[i].bounds.x + slots[i].bounds.w) - 15, slots[i].bounds.y + slots[i].bounds.h - 25, 25, 2, { 255,255,255 });
			break;
		default:
			app->render->DrawRectangle(slots[i].bounds, 255, 0, 0);
			break;
		}
	}


	//btnEquipment->Draw(app->render, showColliders);
	//btnItems->Draw(app->render, showColliders);

}

bool Inventory::UnLoad()
{
	RELEASE(btnEquipment);
	RELEASE(btnItems);
	RELEASE(btnWeapons);
	app->tex->UnLoad(atlasTexture);
	RELEASE(atlasTexture);

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

		break;
	}

	return false;
}

void Inventory::UpdatingButtons(Input* input)
{
}

void Inventory::AddItem(ItemType type)
{
	for (int i = 0; i < MAX_INVENTORY_SLOTS; i++)
	{
		if (slots[i].itemType == type)
		{
			slots[i].itemsAmount++;
			break;
		}
		else
		{
			slots[i].itemType = type;
			slots[i].itemsAmount = 1;
			break;
		}
	}
}
