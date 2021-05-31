#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "Inventory.h"

#include "Font.h"
#include "Item.h"
#include "UltraPotion.h"
#include "Potion.h"
#include "OmniPotion.h"
#include "FairyTear.h"
#include "Orb.h"
#include "OrbFragment.h"
#include "KnightHelmet.h"
#include "KnightChest.h"

#include "Easings.h"

Inventory::Inventory(eastl::list<Player*> pls, SDL_Texture* atlas)
{
	players = pls;
	atlasTexture = atlas;

	easing = new Easing(true, 0, -200, 350, 120);
}

Inventory::~Inventory()
{
}

bool Inventory::Load(Font* font)
{
	guiTex = app->tex->Load("Textures/UI/gui_inventory.png");

	btnEquipment = new GuiButton(1, { -200,116,274,78 }, "Equipment", this, font);
	btnEquipment->alineation = 0;
	btnEquipment->texture = guiTex;
	btnEquipment->section = { 739,0,274,78 };

	btnItems = new GuiButton(2, { -200,204,274,78 }, "Items", this, font);
	btnItems->alineation = 0;
	btnItems->texture = guiTex;
	btnItems->section = { 739,0,274,78 };

	/*btnInfo = new GuiButton(3, { 150,292,274,78 }, "Stats", this, font);
	btnInfo->alineation = 0;
	btnInfo->texture = guiTex;
	btnInfo->section = { 739,0,274,78 };*/
	
	/*btnHunter = new GuiButton(6, { 0,0, 40,40 }, "", this, font);
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
	btnWarrior->section = { 103,722,25,32 };*/

	// Next and back character buttons
	btnNext = new GuiButton(10, { 295, 149, 46, 124 }, "", this, font);
	btnNext->texture = guiTex;
	btnNext->section = { 890, 219, 46, 124 };
	btnNext->sectionFocused = { 890,469,46,124 };

	btnPrev = new GuiButton(11, { 119, 149, 46, 124 }, "", this, font);
	btnPrev->texture = guiTex;
	btnPrev->section = { 890, 90, 46, 124 };
	btnPrev->sectionFocused = { 890, 345, 46, 124 };

	btnUse = new GuiButton(4, { 0,0,128,32 }, "USE", this, font);
	btnUse->texture = nullptr;
	btnUse->alineation = 1;

	btnDelete = new GuiButton(5, { 0,0,128,32 }, "DEL", this, font);
	btnDelete->texture = nullptr;
	btnDelete->alineation = 1;

	btnUnEquip = new GuiButton(12, { 0,0,128, 32 }, "UNEQUIP", this, font);
	btnUnEquip->texture = nullptr;
	btnUnEquip->alineation = 1;

	//btnNextStats = new GuiButton(13, {}, "", this, font);
	//btnPrevStats = new GuiButton(14, {}, "", this, font);

	controls.push_back(btnEquipment);
	controls.push_back(btnItems);
	//buttons.push_back(btnInfo);

	// Initialize items;
	int offsetX = 0;
	int offsetY = 0;

	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		slots[i].bounds = { 466 + offsetX,369 + offsetY,40,40 };
		slots[i].id = i;
		slots[i].filled = false;
		slots[i].state = SlotState::NONE;
		slots[i].item = nullptr;
		slots[i].itemsAmount = 0;

		armorSlots[i].bounds = { 466 + offsetX,369 + offsetY,40,40 };
		armorSlots[i].id = i;
		armorSlots[i].filled = false;
		armorSlots[i].state = SlotState::NONE;
		armorSlots[i].item = nullptr;
		armorSlots[i].itemsAmount = 0;

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
		equipment[i].id = i;
		equipment[i].filled = false;
		equipment[i].state = SlotState::NONE;
		equipment[i].item = nullptr;
		equipment[i].itemsAmount = 0;
	}

	armorSlots->filled = false;
	armorSlots->itemsAmount = 0;

	slots->itemsAmount = 0;
	slots->filled = false;

	state = InventoryState::NONE;
	grabbed = false;
	toGrabCount = 0.0f;
	originSlot = nullptr;
	isTextDisplayed = false;
	displayEquipmentMenu = false;

	currentSlotId = 0;
	currentArmorSlotId = 2;
	inEquipment = false;

	usingItem = false;

	return true;
}

bool Inventory::Update(float dt)
{
	UpdatingButtons(app->input);

	int id = -1;
	if (currentControl != nullptr) id = currentControl->id;

	btnEquipment->Update(app->input, dt, id);
	btnItems->Update(app->input, dt, id);

	// Next and Prev character buttons update
	btnNext->Update(app->input, dt, id);
	btnPrev->Update(app->input, dt, id);

	if (easing->easingsActivated)
	{
		btnEquipment->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		btnItems->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		btnItems->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
		btnNext->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos + 495, easing->deltaPos, easing->totalIterations);
		btnPrev->bounds.x = easing->backEaseOut(easing->currentIteration, easing->initialPos + 319, easing->deltaPos, easing->totalIterations);

		if (easing->currentIteration < easing->totalIterations)
		{
			easing->currentIteration++;
		}
		else
		{
			easing->currentIteration = 0;
			easing->easingsActivated = false;
		}
	}

	switch (state)
	{
	case InventoryState::NONE:
		// TODO
		break;

	case InventoryState::EQUIPMENT:
		
		if (!inEquipment && armorSlots[currentSlotId].state != SlotState::SELECTED && app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_UP)
		{
			InventoryState tmp = state;
			state = InventoryState::NONE;

			currentSlotId = 0;
			armorSlots[0].state = SlotState::NONE;

			currentArmorSlotId = 2;
			equipment[2].state = SlotState::NONE;

			controls.clear();
			controls.push_back(btnEquipment);
			controls.push_back(btnItems);

			if (tmp == InventoryState::EQUIPMENT) currentControl = btnEquipment;
			else if (tmp == InventoryState::ITEMS) currentControl = btnItems;

			currentControl->state = GuiControlState::FOCUSED;
			lastControl = nullptr;
		}
		else if (inEquipment && equipment[currentArmorSlotId].state != SlotState::SELECTED && app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_UP)
		{
			InventoryState tmp = state;
			state = InventoryState::NONE;

			currentArmorSlotId = 2;
			equipment[2].state = SlotState::NONE;

			currentSlotId = 0;
			armorSlots[0].state = SlotState::NONE;

			controls.clear();
			controls.push_back(btnEquipment);
			controls.push_back(btnItems);

			if (tmp == InventoryState::EQUIPMENT) currentControl = btnEquipment;
			else if (tmp == InventoryState::ITEMS) currentControl = btnItems;

			currentControl->state = GuiControlState::FOCUSED;
			lastControl = nullptr;
		}
		
		
		HandleObjects(armorSlots);
		if (currentSlotId > -1) HandleSlot(armorSlots, dt);

		btnUse->text = "EQUIP";

		break;

	case InventoryState::ITEMS:
		
		if (!inEquipment && slots[currentSlotId].state != SlotState::SELECTED && app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_UP)
		{
			InventoryState tmp = state;
			state = InventoryState::NONE;
			
			currentSlotId = 0;
			slots[0].state = SlotState::NONE;

			controls.clear();
			controls.push_back(btnEquipment);
			controls.push_back(btnItems);

			if (tmp == InventoryState::EQUIPMENT) currentControl = btnEquipment;
			else if (tmp == InventoryState::ITEMS) currentControl = btnItems;

			currentControl->state = GuiControlState::FOCUSED;
			lastControl = nullptr;
		}

		HandleObjects(slots);
		if (currentSlotId > -1) HandleSlot(slots, dt);
		
		btnUse->text = "USE";

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
		app->render->DrawTexture(guiTex, btnEquipment->bounds.x + 250, 53, &section, false);
	}

	else if (state == InventoryState::STATS)
	{
		section = { 1144, 0, 739, 597 };
		app->render->DrawTexture(guiTex, btnEquipment->bounds.x + 250, 53, &section, false);
	}

	btnEquipment->Draw(app->render, showColliders, 36);
	btnItems->Draw(app->render, showColliders, 36);

	if (state != InventoryState::STATS)
	{
		// Shape where the stats should go
		SDL_Rect r = { 163, 715, 40, 40 };

		// Equipment equiped drawing
		for (int i = 0; i < MAX_EQUIPMENT_SLOTS; ++i)
		{
			app->render->DrawTexture(atlasTexture, btnEquipment->bounds.x + 600, equipment[i].bounds.y, &r, false);
			if (equipment[i].item != nullptr && &equipment[i].item->atlasSection != NULL)
				app->render->DrawTexture(atlasTexture, btnEquipment->bounds.x + 603, equipment[i].bounds.y + 4, &equipment[i].item->atlasSection, false);

			if (equipment[currentArmorSlotId].state == SlotState::FOCUSED)
			{
				app->render->DrawRectangle(equipment[currentArmorSlotId].bounds, 200, 200, 200, 50, true, false);
			}
		}

		r = { btnEquipment->bounds.x + 740, 130, 196, 50 };
		app->render->DrawCenterText(font, "HUNTER", r, 30, 5, { 255,255,255,255 });
		r.y = 235;
		app->render->DrawCenterText(font, "WIZARD", r, 30, 5, { 255,255,255,255 });
		r.y = 340;
		app->render->DrawCenterText(font, "THIEF", r, 30, 5, { 255,255,255,255 });
		r.y = 445;
		app->render->DrawCenterText(font, "WARRIOR", r, 30, 5, { 255,255,255,255 });

		// Draw current player
		SDL_Rect rectan;
		SDL_Rect rectan2 = { btnEquipment->bounds.x + 373,149,116,124 };
		SDL_Rect rectan3;
		app->render->DrawRectangle(rectan2, 205, 205, 205, 200, true, false);

		if (currentPlayer->playerType == PlayerType::HUNTER)
		{
			rectan = { 739,103,106,113 };
			rectan3 = { btnEquipment->bounds.x + 735, 120, 206, 110 };
		}
		else if (currentPlayer->playerType == PlayerType::WIZARD)
		{
			rectan = { 739, 217,106,113 };
			rectan3 = { btnEquipment->bounds.x + 735, 225, 206, 110 };
		}
		else if (currentPlayer->playerType == PlayerType::THIEF)
		{
			rectan = { 739,331,106,113 };
			rectan3 = { btnEquipment->bounds.x + 735, 330, 206, 110 };
		}
		else if (currentPlayer->playerType == PlayerType::WARRIOR)
		{
			rectan = { 739,445,106,113 };
			rectan3 = { btnEquipment->bounds.x + 735, 435, 206, 110 };
		}
		app->render->DrawTexture(guiTex, btnEquipment->bounds.x + 377, 154, &rectan, false);
		app->render->DrawRectangle(rectan3, 255, 255, 255, 255, false, false);

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
				app->render->DrawTexture(atlasTexture, btnEquipment->bounds.x + 740, 173 + (j * 14) + (i * 105), &statsBar, false);
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
			app->render->DrawTexture(atlasTexture, btnEquipment->bounds.x + 743, 175 + (i * 105), &statsBar, false);

			// Blue Bar
			statsBar = { 228, 764, (190 * pl->GetManaPoints()) / (pl->GetMaxManaPoints()), 5 };
			app->render->DrawTexture(atlasTexture, btnEquipment->bounds.x + 743, 189 + (i * 105), &statsBar, false);

			// Yellow Bar
			statsBar = { 228, 778, (190 * pl->GetArmorPoints()) / (pl->GetMaxArmorPoints()), 5 };
			app->render->DrawTexture(atlasTexture, btnEquipment->bounds.x + 743, 203 + (i * 105), &statsBar, false);
		}
	}


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
	//if (displayEquipmentMenu && state != InventoryState::STATS) DisplayMenuEquipment(showColliders);
}

bool Inventory::UnLoad()
{
	RELEASE(btnEquipment);
	RELEASE(btnItems);
	RELEASE(btnUse);
	RELEASE(btnDelete);
	/*RELEASE(btnHunter);
	RELEASE(btnWizard);
	RELEASE(btnThief);
	RELEASE(btnWarrior);*/
	RELEASE(btnUnEquip);
	RELEASE(btnNext);
	RELEASE(btnPrev);

	RELEASE(easing);

	app->tex->UnLoad(atlasTexture);
	//RELEASE(atlasTexture);
	app->tex->UnLoad(guiTex);

	controls.clear();
	players.clear();

	return true;
}

bool Inventory::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:

		if (control->id == 1)
		{
			state = InventoryState::EQUIPMENT;

			controls.clear();
			lastControl = currentControl;
			currentControl = nullptr;
		}
		else if (control->id == 2) // Items
		{
			state = InventoryState::ITEMS;

			controls.clear();
			lastControl = currentControl;
			currentControl = nullptr;
		}
		else if (control->id == 3) state = InventoryState::STATS;
		else if (control->id == 4) // Use
		{
			if (state == InventoryState::ITEMS)
			{
				//slots[currentSlotId].state = SlotState::USE;
				if (!inEquipment)
				{
					UseObject(slots, currentPlayer);
				}
				else
				{
					AddItem(equipment[currentArmorSlotId].item);
					currentPlayer->UnequipArmor((Armor*)equipment[currentArmorSlotId].item);
					equipment[currentArmorSlotId].item = nullptr;
					equipment[currentArmorSlotId].itemsAmount--;
					equipment[currentArmorSlotId].filled = false;
					equipment[currentArmorSlotId].state = SlotState::NONE;
					displayEquipmentMenu = false;
					isTextDisplayed = false;
				}
			}
			else if (state == InventoryState::EQUIPMENT)
			{
				if (!inEquipment)
				{
					currentPlayer->SetEquipment((Armor*)armorSlots[currentSlotId].item);
					GetEquipment(currentPlayer);
					UseObject(armorSlots, currentPlayer);
				}
				else
				{
					AddItem(equipment[currentArmorSlotId].item);
					currentPlayer->UnequipArmor((Armor*)equipment[currentArmorSlotId].item);
					equipment[currentArmorSlotId].item = nullptr;
					equipment[currentArmorSlotId].itemsAmount--;
					equipment[currentArmorSlotId].filled = false;
					equipment[currentArmorSlotId].state = SlotState::NONE;
					displayEquipmentMenu = false;
					isTextDisplayed = false;
				}
			}
			
		}
		else if (control->id == 5) // Delete
		{
			if (state == InventoryState::ITEMS)
			{
				//slots[currentSlotId].state = SlotState::DELETE;
				slots[currentSlotId].itemsAmount--;
				slots[currentSlotId].state = SlotState::UNSELECTED;
				isTextDisplayed = false;
				usingItem = false;
			}
			else if (state == InventoryState::EQUIPMENT)
			{
				//armorSlots[currentSlotId].state = SlotState::DELETE;
				armorSlots[currentSlotId].itemsAmount--;
				armorSlots[currentSlotId].state = SlotState::UNSELECTED;
				isTextDisplayed = false;
				usingItem = false;
			}
		}
		else if (control->id == 6) // Equip
		{
			switch (state)
			{
			case InventoryState::ITEMS:			
				UseObject(slots, currentPlayer);
				break;

			case InventoryState::EQUIPMENT:
				currentPlayer->SetEquipment((Armor*)armorSlots[currentSlotId].item);
				GetEquipment(currentPlayer);
				UseObject(armorSlots, currentPlayer);

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
	int prevX = xMouse;
	int prevY = yMouse;
	input->GetMousePosition(xMouse, yMouse);
	if (prevX != xMouse || prevY != yMouse)
	{
		lastUserInput = 1;
		SDL_ShowCursor(SDL_ENABLE);
	}
	else
	{
		lastUserInput = 0;
	}

	if (state == InventoryState::EQUIPMENT || state == InventoryState::ITEMS)
	{
		if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_REPEAT)
		{
			currentControl = btnPrev;
			btnPrev->state = GuiControlState::PRESSED;
		}
		else if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_REPEAT)
		{
			currentControl = btnNext;
			btnNext->state = GuiControlState::PRESSED;
		}

		if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == KEY_UP)
		{
			btnPrev->state = GuiControlState::NORMAL;
			app->audio->PlayFx(btnPrev->channel, btnPrev->clickFx);
			btnPrev->isPlayable = true;
			btnPrev->NotifyObserver();
			/*currentControl = btnPrev;*/
		}
		else if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == KEY_UP)
		{
			btnNext->state = GuiControlState::NORMAL;
			app->audio->PlayFx(btnNext->channel, btnNext->clickFx);
			btnNext->isPlayable = true;
			btnNext->NotifyObserver();
			/*currentControl = btnNext;*/
		}

	}

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
	{
		if (currentControl != btnPrev && currentControl != btnNext)
		{
			if (currentControl == nullptr)
			{
				currentControl = (*controls.begin());
				SDL_ShowCursor(SDL_DISABLE);
			}
			else
			{
				eastl::list<GuiControl*>::iterator it = controls.begin();
				for (int i = 0; i < controls.size(); ++i, ++it)
				{
					if ((*it)->id == currentControl->id + 1)
					{
						currentControl = (*it);
						break;
					}
				}
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
	{
		if (currentControl != btnPrev && currentControl != btnNext)
		{
			if (currentControl == nullptr)
			{
				currentControl = (*controls.begin());
				SDL_ShowCursor(SDL_DISABLE);
			}
			else
			{
				eastl::list<GuiControl*>::iterator it = controls.begin();
				for (int i = 0; i < controls.size(); ++i, ++it)
				{
					if ((*it)->id == currentControl->id - 1)
					{
						currentControl = (*it);
						break;
					}
				}
			}
		}
	}
}

void Inventory::LoadState(pugi::xml_node& node)
{
	pugi::xml_node n = node.child("inventory").child("items");
	int i;
	for (i = 0, n = n.child("slot"); n && i < MAX_INVENTORY_SLOTS; n = n.next_sibling("slot"), ++i)
	{
		Item* item = nullptr;
		pugi::xml_node it = n.child("item");
		if ((ObjectType)it.child("object_type").attribute("value").as_int() == ObjectType::ITEM)
		{
			switch ((ItemType)it.child("item_type").attribute("value").as_int())
			{
			case ItemType::POTION:
				item = new Potion(iPoint(it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int()), atlasTexture, it.attribute("map_name").as_string());
				break;
			case ItemType::ULTRA_POTION:
				item = new UltraPotion(iPoint(it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int()), atlasTexture, it.attribute("map_name").as_string());
				break;
			case ItemType::OMNI_POTION:
				item = new OmniPotion(iPoint(it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int()), atlasTexture, it.attribute("map_name").as_string());
				break;
			case ItemType::FAIRY_TEAR:
				item = new FairyTear(iPoint(it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int()), atlasTexture, it.attribute("map_name").as_string());
				break;
			}
		}

		if (slots[i].item != nullptr)
		{
			slots[i].item->UnLoad();
			RELEASE(slots[i].item);
			slots[i].itemsAmount = 0;
			slots[i].filled = false;
			slots[i].state = SlotState::NONE;
		}
		if (item != nullptr)
		{
			item->Load();
			slots[i].item = item;
			slots[i].id = i;
			slots[i].itemsAmount = n.attribute("amount").as_int();
			slots[i].filled = n.attribute("filled").as_bool();
			slots[i].state = (SlotState)n.attribute("state").as_int();
		}
	}

	n = node.child("inventory").child("armor");
	i = 0;
	for (n = n.child("slot"); n && i < MAX_INVENTORY_SLOTS; n = n.next_sibling("slot"), ++i)
	{
		Item* item = nullptr;
		SDL_Rect r;
		pugi::xml_node it = n.child("item");
		if ((ObjectType)it.child("object_type").attribute("value").as_int() == ObjectType::ARMOR)
		{
			switch ((ArmorType)it.child("armor_type").attribute("value").as_int())
			{
			case ArmorType::HELMET:
				r = { it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int(), it.child("position").attribute("w").as_int(), it.child("position").attribute("h").as_int() };
				item = new KnightHelmet(r, iPoint(r.x, r.y), atlasTexture, it.attribute("map_name").as_string());
				break;
			case ArmorType::CHEST:
				r = { it.child("position").attribute("x").as_int(), it.child("position").attribute("y").as_int(), it.child("position").attribute("w").as_int(), it.child("position").attribute("h").as_int() };
				item = new KnightChest(r, iPoint(r.x, r.y), atlasTexture, it.attribute("map_name").as_string());
				break;
			}
		}

		if (armorSlots[i].item != nullptr)
		{
			armorSlots[i].item->UnLoad();
			RELEASE(slots[i].item);
			armorSlots[i].itemsAmount = 0;
			armorSlots[i].filled = false;
			armorSlots[i].state = SlotState::NONE;
		}
		if (item != nullptr)
		{
			item->Load();
			armorSlots[i].item = item;
			armorSlots[i].id = i;
			armorSlots[i].itemsAmount = n.attribute("amount").as_int();
			armorSlots[i].filled = n.attribute("filled").as_bool();
			armorSlots[i].state = (SlotState)n.attribute("state").as_int();
		}
	}
}

void Inventory::SaveState(pugi::xml_node& node)
{
	pugi::xml_node n = node.append_child("inventory");
	pugi::xml_node it = n.append_child("items");
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		// Items
		pugi::xml_node item = it.append_child("slot");
		if (slots[i].filled)
		{
			item.append_attribute("amount").set_value(slots[i].itemsAmount);
			item.append_attribute("filled").set_value(slots[i].filled);
			item.append_attribute("id").set_value(slots[i].id);
			item.append_attribute("state").set_value((int)slots[i].state);
			pugi::xml_node it = item.append_child("item");
			if(slots[i].item != nullptr)
				slots[i].item->SaveState(it);
		}
	}

	it = n.append_child("armor");
	for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
	{
		// Armor
		pugi::xml_node item = it.append_child("slot");
		if (armorSlots[i].filled)
		{
			item.append_attribute("amount").set_value(armorSlots[i].itemsAmount);
			item.append_attribute("filled").set_value(armorSlots[i].filled);
			item.append_attribute("id").set_value(armorSlots[i].id);
			item.append_attribute("state").set_value((int)armorSlots[i].state);
			pugi::xml_node it = item.append_child("item");
			armorSlots[i].item->SaveState(it);
		}
	}
}

void Inventory::AddItem(Item *it)
{
	switch ((*it).objectType)
	{
	case ObjectType::ARMOR:
		for (int i = 0; i < MAX_INVENTORY_SLOTS; ++i)
		{
			// Already an armor into the slot
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
	btnDelete->bounds.y = r.y + r.h / 2 + 5; // 5 stands for offset
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

void Inventory::ResetStates()
{
	easing->easingsActivated = true;
	state = InventoryState::NONE;

	controls.clear();
	controls.push_back(btnEquipment);
	controls.push_back(btnItems);
	currentControl = btnEquipment;
	lastControl = nullptr;
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

			//if (IsMouseInside(objects[i].bounds) && !isTextDisplayed)
			/*if(objects[currentSlotId].state == SlotState::FOCUSED)
			{
				app->render->DrawRectangle(objects[currentSlotId].bounds, 200, 200, 200, 50, true, false);
			}*/
		}
	}

	if (objects[currentSlotId].state == SlotState::FOCUSED)
	{
		app->render->DrawRectangle(objects[currentSlotId].bounds, 200, 200, 200, 50, true, false);
	}
	else if (equipment[currentArmorSlotId].state == SlotState::FOCUSED)
	{
		app->render->DrawRectangle(equipment[currentArmorSlotId].bounds, 200, 200, 200, 50, true, false);
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

	if (currentSlotId > -1 || currentArmorSlotId > -1)
	{
		if (objects[currentSlotId].state == SlotState::SELECTED)
			DisplayText(objects[currentSlotId].bounds, showColliders);

		if (equipment[currentArmorSlotId].state == SlotState::SELECTED)
			DisplayText(equipment[currentArmorSlotId].bounds, showColliders);
	}
}

void Inventory::HandleSlot(InventorySlot objects[], float dt)
{
	//objects[0].state = SlotState::FOCUSED;
	if (objects[currentSlotId].state != SlotState::SELECTED)
	{
		if (!inEquipment)
		{
			if ((app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_UP) && currentSlotId - 1 >= 0)
			{
				objects[currentSlotId].state = SlotState::NONE;
				--currentSlotId;
				objects[currentSlotId].state = SlotState::FOCUSED;
			}
			else if ((app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_UP) && currentSlotId + 1 <= MAX_INVENTORY_SLOTS - 1)
			{
				objects[currentSlotId].state = SlotState::NONE;
				++currentSlotId;
				objects[currentSlotId].state = SlotState::FOCUSED;
			}
			else if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_UP && currentSlotId + 8 <= MAX_INVENTORY_SLOTS - 1)
			{
				objects[currentSlotId].state = SlotState::NONE;
				currentSlotId += 8;
				objects[currentSlotId].state = SlotState::FOCUSED;
			}
			else if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_UP)
			{
				if (currentSlotId >= 0 && currentSlotId <= 7)
				{
					inEquipment = true;
					objects[currentSlotId].state = SlotState::NONE;
					equipment[currentArmorSlotId].state = SlotState::FOCUSED;
					/*equipment[currentArmorSlotId].state = SlotState::NONE;
					--currentArmorSlotId;
					equipment[currentArmorSlotId].state = SlotState::FOCUSED;*/
				}
				else if (currentSlotId - 8 >= 0)
				{
					objects[currentSlotId].state = SlotState::NONE;
					currentSlotId -= 8;
					objects[currentSlotId].state = SlotState::FOCUSED;
				}
			}
		}
		else
		{
			if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_UP) && !isTextDisplayed)
			{
				if (currentArmorSlotId >= 2)
				{
					equipment[currentArmorSlotId].state = SlotState::NONE;
					objects[currentSlotId].state = SlotState::FOCUSED;
					inEquipment = false;
				}
				else
				{
					equipment[currentArmorSlotId].state = SlotState::NONE;
					++currentArmorSlotId;
					equipment[currentArmorSlotId].state = SlotState::FOCUSED;
				}
			}
			else if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_UP || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_UP) && currentArmorSlotId > 0)
			{
				equipment[currentArmorSlotId].state = SlotState::NONE;
				--currentArmorSlotId;
				equipment[currentArmorSlotId].state = SlotState::FOCUSED;
			
			}
		}
	}

	int id = -1;
	if (currentControl != nullptr) id = currentControl->id;

	if (!inEquipment)
	{
		switch (objects[currentSlotId].state)
		{
		case SlotState::UNSELECTED:
			break;

		case SlotState::SELECTED:
			if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_UP)
			{
				controls.clear();
				currentControl = nullptr;
				lastControl = nullptr;
				objects[currentSlotId].state = SlotState::FOCUSED;
				isTextDisplayed = false;
				usingItem = false;
			}
			btnUse->Update(app->input, dt, id);
			btnDelete->Update(app->input, dt, id);
			break;

		case SlotState::FOCUSED:
			if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP && objects[currentSlotId].filled)
			{
				objects[currentSlotId].state = SlotState::SELECTED;

				controls.clear();
				controls.push_back(btnUse);
				controls.push_back(btnDelete);
				btnUse->text = "USE";
				currentControl = (*controls.begin());
				btnUse->state = GuiControlState::FOCUSED;
				lastControl = nullptr;
			}
			/*btnUse->Update(app->input, dt, -1);
			btnDelete->Update(app->input, dt, -1);*/

			break;

		case SlotState::USE:
			break;

		case SlotState::DELETE:
			break;
		}
	}
	else
	{
		switch (equipment[currentArmorSlotId].state)
		{
		case SlotState::UNSELECTED:
			break;

		case SlotState::SELECTED:
			if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_UP)
			{
				controls.clear();
				currentControl = nullptr;
				lastControl = nullptr;
				equipment[currentArmorSlotId].state = SlotState::FOCUSED;
				isTextDisplayed = false;
				usingItem = false;
			}
			btnUse->Update(app->input, dt, id);
			btnDelete->Update(app->input, dt, id);
			break;

		case SlotState::FOCUSED:
			if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP && equipment[currentArmorSlotId].filled)
			{
				equipment[currentArmorSlotId].state = SlotState::SELECTED;
				isTextDisplayed = true;

				controls.clear();
				controls.push_back(btnUse);
				controls.push_back(btnDelete);
				btnUse->text = "UNEQUIP";
				currentControl = (*controls.begin());
				btnUse->state = GuiControlState::FOCUSED;
				lastControl = nullptr;
			}

			break;
		case SlotState::USE:
			break;

		case SlotState::DELETE:
			break;
		}
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
				++counter;

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
			++counter;
		}
		else if (armorSlots[i].item != nullptr && armorSlots[i].item->itemType == itemType)
		{
			++counter;
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