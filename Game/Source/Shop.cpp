#include "App.h"
#include "Input.h"
#include "Render.h"

#include "SceneGameplay.h"
#include "Inventory.h"
#include "Player.h"

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

Shop::Shop(SceneGameplay* s, Inventory* inven, SDL_Texture* at, Player* current)
{
	scene = s;
	inventory = inven;
	helmet = nullptr;
	fairyBottle = nullptr;
	fairyTear = nullptr;
	fairyWing = nullptr;
	omniPotion = nullptr;
	potion = nullptr;
	ultraPotion = nullptr;
	back = nullptr;
	player = current;

	atlas = at;
}

Shop::~Shop()
{
}

bool Shop::Load(Font* font)
{
	texture = app->tex->Load("Textures/UI/items_shop.png");
	background = app->tex->Load("Textures/UI/shop_background.png");

	helmet = new GuiButton(1, { 375, 200, 80, 80 }, "", this, font);
	helmet->texture = texture;
	helmet->section = { 512, 0, 80, 80 };
	helmet->alineation = 1;

	chest = new GuiButton(2, { 525, 200, 80, 80 }, "", this, font);
	chest->texture = texture;
	chest->section = { 512, 0, 80, 80 };
	chest->alineation = 1;

	fairyBottle = new GuiButton(3, { 675, 200, 80, 80 }, "", this, font);
	fairyBottle->texture = texture;
	fairyBottle->section = { 512, 0, 80, 80 };
	fairyBottle->alineation = 1;

	fairyTear = new GuiButton(4, { 825, 200, 80, 80 }, "", this, font);
	fairyTear->texture = texture;
	fairyTear->section = { 512, 0, 80, 80 };
	fairyTear->alineation = 1;

	fairyWing = new GuiButton(5, { 375, 330, 80, 80 }, "", this, font);
	fairyWing->texture = texture;
	fairyWing->section = { 512, 0, 80, 80 };
	fairyWing->alineation = 1;

	omniPotion = new GuiButton(6, { 525, 330, 80, 80 }, "", this, font);
	omniPotion->texture = texture;
	omniPotion->section = { 512, 0, 80, 80 };
	omniPotion->alineation = 1;

	potion = new GuiButton(7, { 675, 330, 80, 80 }, "", this, font);
	potion->texture = texture;
	potion->section = { 512, 0, 80, 80 };
	potion->alineation = 1;

	ultraPotion = new GuiButton(8, { 825, 330, 80, 80 }, "", this, font);
	ultraPotion->texture = texture;
	ultraPotion->section = { 512, 0, 80, 80 };
	ultraPotion->alineation = 1;

	back = new GuiButton(9, { 805, 508, 130, 64 }, "BACK", this, font);
	back->texture = texture;
	back->section = { 622, 0, 130, 64 };
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
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120);
	app->render->DrawTexture(background, 233, 56);
	
	SDL_Rect itSection = { 0, 0, 64, 64 };
	SDL_Rect goldSection = { 592, 0, 30, 30 };
	helmet->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, helmet->bounds.x + 8, helmet->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, helmet->bounds.x + 3, helmet->bounds.y + helmet->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "250", helmet->bounds.x + 37, helmet->bounds.y + helmet->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 64, 0, 64, 64 };
	chest->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, chest->bounds.x + 8, chest->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, chest->bounds.x + 3, chest->bounds.y + chest->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "250", chest->bounds.x + 37, chest->bounds.y + chest->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 448, 0, 64, 64 };
	fairyBottle->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, fairyBottle->bounds.x + 8, fairyBottle->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, fairyBottle->bounds.x + 3, fairyBottle->bounds.y + fairyBottle->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "200", fairyBottle->bounds.x + 37, fairyBottle->bounds.y + fairyBottle->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 320, 0, 64, 64 };
	fairyTear->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, fairyTear->bounds.x + 8, fairyTear->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, fairyTear->bounds.x + 3, fairyTear->bounds.y + fairyTear->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "100", fairyTear->bounds.x + 37, fairyTear->bounds.y + fairyTear->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 384, 0, 64, 64 };
	fairyWing->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, fairyWing->bounds.x + 8, fairyWing->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, fairyWing->bounds.x, fairyWing->bounds.y + fairyWing->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "150", fairyWing->bounds.x + 37, fairyWing->bounds.y + fairyWing->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 128, 0, 64, 64 };
	omniPotion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, omniPotion->bounds.x + 8, omniPotion->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, omniPotion->bounds.x + 3, omniPotion->bounds.y + omniPotion->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "200", omniPotion->bounds.x + 37, omniPotion->bounds.y + omniPotion->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 256, 0, 64, 64 };
	potion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, potion->bounds.x + 8, potion->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, potion->bounds.x + 3, potion->bounds.y + potion->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "100", potion->bounds.x + 37, potion->bounds.y + potion->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 192, 0, 64, 64 };
	ultraPotion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, ultraPotion->bounds.x + 8, ultraPotion->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, ultraPotion->bounds.x + 3, ultraPotion->bounds.y + ultraPotion->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "150", ultraPotion->bounds.x + 37, ultraPotion->bounds.y + ultraPotion->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	back->Draw(app->render, showColliders, 40, { 0, 0, 0, 255 });
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
			if (player->gold >= 250)
			{
				inventory->AddItem(CreateItem(ArmorType::HELMET));
				player->gold -= 250;
			}
		}
		else if (control->id == 2)
		{
			if (player->gold >= 250)
			{
				inventory->AddItem(CreateItem(ArmorType::CHEST));
				player->gold -= 250;
			}
		}
		else if (control->id == 3)
		{
			if (player->gold >= 200)
			{
				inventory->AddItem(CreateItem(ItemType::FAIRY_BOTTLE));
				player->gold -= 200;
			}
		}
		else if (control->id == 4)
		{
			if (player->gold >= 100)
			{
				inventory->AddItem(CreateItem(ItemType::FAIRY_TEAR));
				player->gold -= 100;
			}
		}
		else if (control->id == 5)
		{
			if (player->gold >= 150)
			{
				inventory->AddItem(CreateItem(ItemType::FAIRY_WING));
				player->gold -= 150;
			}
		}
		else if (control->id == 6)
		{
			if (player->gold >= 200)
			{
				inventory->AddItem(CreateItem(ItemType::OMNI_POTION));
				player->gold -= 200;
			}
		}
		else if (control->id == 7)
		{
			if (player->gold >= 100)
			{
				inventory->AddItem(CreateItem(ItemType::POTION));
				player->gold -= 100;
			}
		}
		else if (control->id == 8)
		{
			if (player->gold >= 150)
			{
				inventory->AddItem(CreateItem(ItemType::ULTRA_POTION));
				player->gold -= 150;
			}
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