#include "App.h"
#include "Input.h"
#include "Render.h"

#include "SceneGameplay.h"
#include "Inventory.h"

#include "Shop.h"

#include "Item.h"
#include "Armor.h"

#include "KnightHelmet.h"
#include "KnightChest.h"

#include "FairyBottle.h"
#include "FairyTear.h"
#include "FairyWing.h"
#include "OmniPotion.h"
#include "Potion.h"
#include "UltraPotion.h"

Shop::Shop(SceneGameplay* s, Inventory* inven, SDL_Texture* at)
{
	scene = s;
	inventory = inven;
	helmet = nullptr;
	back = nullptr;
	count = 0;

	atlas = at;
}

Shop::~Shop()
{
}

bool Shop::Load(Font* font)
{
	helmet = new GuiButton(1, { 150, 70, 100, 50 }, "HELMET", this, font);
	helmet->alineation = 1;

	chest = new GuiButton(2, { 150, 150, 100, 50 }, "CHEST", this, font);
	chest->alineation = 1;

	fairyBottle = new GuiButton(3, { 150, 230, 100, 50 }, "FAIRY BOTTLE", this, font);
	fairyBottle->alineation = 1;

	fairyTear = new GuiButton(4, { 150, 310, 100, 50 }, "FAIRY TEAR", this, font);
	fairyTear->alineation = 1;

	fairyWing = new GuiButton(5, { 150, 390, 100, 50 }, "FAIRY WING", this, font);
	fairyWing->alineation = 1;

	omniPotion = new GuiButton(6, { 150, 470, 100, 50 }, "OMNIPOTION", this, font);
	omniPotion->alineation = 1;

	potion = new GuiButton(7, { 150, 550, 100, 50 }, "POTION", this, font);
	potion->alineation = 1;

	ultraPotion = new GuiButton(8, { 150, 630, 100, 50 }, "ULTRAPOTION", this, font);
	ultraPotion->alineation = 1;

	back = new GuiButton(9, { 1030, 600, 100, 50 }, "BACK", this, font);
	back->alineation = 1;

	return true;
}

bool Shop::Update(float dt)
{
	helmet->Update(app->input, dt, -1);
	chest->Update(app->input, dt, -1);
	fairyBottle->Update(app->input, dt, -1);
	fairyTear->Update(app->input, dt, -1);
	fairyWing->Update(app->input, dt, -1);
	omniPotion->Update(app->input, dt, -1);
	potion->Update(app->input, dt, -1);
	ultraPotion->Update(app->input, dt, -1);
	back->Update(app->input, dt, -1);
	return true;
}

void Shop::Draw(Font* font, bool showColliders)
{
	app->render->DrawRectangle({ 140, 60, 1000, 600 }, 150, 150, 150);
	helmet->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	chest->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	fairyBottle->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	fairyTear->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	fairyWing->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	omniPotion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	potion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	ultraPotion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	back->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
}

bool Shop::UnLoad()
{
	return true;
}

bool Shop::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1)
		{
			inventory->AddItem(CreateItem(ArmorType::HELMET));
		}
		else if (control->id == 2)
		{
			inventory->AddItem(CreateItem(ArmorType::CHEST));
		}
		else if (control->id == 3)
		{
			inventory->AddItem(CreateItem(ItemType::FAIRY_BOTTLE));
		}
		else if (control->id == 4)
		{
			inventory->AddItem(CreateItem(ItemType::FAIRY_TEAR));
		}
		else if (control->id == 5)
		{
			inventory->AddItem(CreateItem(ItemType::FAIRY_WING));
		}
		else if (control->id == 6)
		{
			inventory->AddItem(CreateItem(ItemType::OMNI_POTION));
		}
		else if (control->id == 7)
		{
			inventory->AddItem(CreateItem(ItemType::POTION));
		}
		else if (control->id == 8)
		{
			inventory->AddItem(CreateItem(ItemType::ULTRA_POTION));
		}
		if (control->id == 9) scene->ChangeState(GameplayMenuState::NONE);
		break;
	}
	return false;
}

Item* Shop::CreateItem(ItemType type)
{
	Item* item = nullptr;
	iPoint pos;

	switch (type)
	{
	case ItemType::FAIRY_BOTTLE:
		pos = { 10, 10 };
		item = new FairyBottle(pos, atlas);
		break;
	case ItemType::FAIRY_TEAR:
		pos = { 10, 10 };
		item = new FairyTear(pos, atlas);
		break;
	case ItemType::FAIRY_WING:
		pos = { 10, 10 };
		item = new FairyWing(pos, atlas);
		break;
	case ItemType::OMNI_POTION:
		pos = { 10, 10 };
		item = new OmniPotion(pos, atlas);
		break;
	case ItemType::POTION:
		pos = { 10, 10 };
		item = new Potion(pos, atlas);
		break;
	case ItemType::ULTRA_POTION:
		pos = { 10, 10 };
		item = new UltraPotion(pos, atlas);
		break;
	}
	item->Load();

	return item;
}

Item* Shop::CreateItem(ArmorType type)
{
	Item* item = nullptr;
	iPoint pos;
	switch (type)
	{
	case ArmorType::HELMET:
		pos = { 10, 10 };
		item = new KnightHelmet({ pos.x, pos.y, 32, 32}, pos, atlas);
		break;
	case ArmorType::CHEST:
		pos = { 10, 10 };
		item = new KnightChest({ pos.x, pos.y, 32, 32 }, pos, atlas);
		break;
	}

	item->Load();

	return item;
}