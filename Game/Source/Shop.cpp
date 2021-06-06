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
	btnHelmet = nullptr;
	btnFairyBottle = nullptr;
	btnFairyTear = nullptr;
	btnFairyWing = nullptr;
	btnOmniPotion = nullptr;
	btnPotion = nullptr;
	btnUltraPotion = nullptr;
	btnBack = nullptr;
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

	btnHelmet = new GuiButton(1, { 375, 200, 80, 80 }, "", this, font);
	btnHelmet->texture = texture;
	btnHelmet->section = { 512, 0, 80, 80 };
	btnHelmet->alineation = 1;

	btnChest = new GuiButton(2, { 525, 200, 80, 80 }, "", this, font);
	btnChest->texture = texture;
	btnChest->section = { 512, 0, 80, 80 };
	btnChest->alineation = 1;

	btnFairyBottle = new GuiButton(3, { 675, 200, 80, 80 }, "", this, font);
	btnFairyBottle->texture = texture;
	btnFairyBottle->section = { 512, 0, 80, 80 };
	btnFairyBottle->alineation = 1;

	btnFairyTear = new GuiButton(4, { 825, 200, 80, 80 }, "", this, font);
	btnFairyTear->texture = texture;
	btnFairyTear->section = { 512, 0, 80, 80 };
	btnFairyTear->alineation = 1;

	btnFairyWing = new GuiButton(5, { 375, 330, 80, 80 }, "", this, font);
	btnFairyWing->texture = texture;
	btnFairyWing->section = { 512, 0, 80, 80 };
	btnFairyWing->alineation = 1;

	btnOmniPotion = new GuiButton(6, { 525, 330, 80, 80 }, "", this, font);
	btnOmniPotion->texture = texture;
	btnOmniPotion->section = { 512, 0, 80, 80 };
	btnOmniPotion->alineation = 1;

	btnPotion = new GuiButton(7, { 675, 330, 80, 80 }, "", this, font);
	btnPotion->texture = texture;
	btnPotion->section = { 512, 0, 80, 80 };
	btnPotion->alineation = 1;

	btnUltraPotion = new GuiButton(8, { 825, 330, 80, 80 }, "", this, font);
	btnUltraPotion->texture = texture;
	btnUltraPotion->section = { 512, 0, 80, 80 };
	btnUltraPotion->alineation = 1;

	btnBack = new GuiButton(9, { 805, 508, 130, 64 }, "BACK", this, font);
	btnBack->texture = texture;
	btnBack->section = { 622, 0, 130, 64 };
	btnBack->alineation = 1;

	controls.push_back(btnHelmet);
	controls.push_back(btnChest);
	controls.push_back(btnFairyBottle);
	controls.push_back(btnFairyTear);
	controls.push_back(btnFairyWing);
	controls.push_back(btnOmniPotion);
	controls.push_back(btnPotion);
	controls.push_back(btnUltraPotion);
	controls.push_back(btnBack);

	btnHelmet->state = GuiControlState::FOCUSED;
	currentControl = btnHelmet;
	lastControl = nullptr;

	return true;
}

bool Shop::Update(float dt)
{
	UpdatingButtons(app->input);

	int id = -1;
	if (currentControl != nullptr)
	{
		if (currentControl->state != GuiControlState::FOCUSED) currentControl->state = GuiControlState::FOCUSED;
		id = currentControl->id;
	}

	btnHelmet->Update(app->input, dt, id);
	btnChest->Update(app->input, dt, id);
	btnFairyBottle->Update(app->input, dt, id);
	btnFairyTear->Update(app->input, dt, id);
	btnFairyWing->Update(app->input, dt, id);
	btnOmniPotion->Update(app->input, dt, id);
	btnPotion->Update(app->input, dt, id);
	btnUltraPotion->Update(app->input, dt, id);
	btnBack->Update(app->input, dt, id);

	if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_B) == KEY_UP)
		scene->ChangeState(GameplayMenuState::NONE);

	return true;
}

void Shop::Draw(Font* font, bool showColliders)
{
	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 120);
	app->render->DrawTexture(background, 233, 56);
	
	SDL_Rect itSection = { 0, 0, 64, 64 };
	SDL_Rect goldSection = { 592, 0, 30, 30 };
	btnHelmet->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, btnHelmet->bounds.x + 8, btnHelmet->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, btnHelmet->bounds.x + 3, btnHelmet->bounds.y + btnHelmet->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "250", btnHelmet->bounds.x + 37, btnHelmet->bounds.y + btnHelmet->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 64, 0, 64, 64 };
	btnChest->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, btnChest->bounds.x + 8, btnChest->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, btnChest->bounds.x + 3, btnChest->bounds.y + btnChest->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "250", btnChest->bounds.x + 37, btnChest->bounds.y + btnChest->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 448, 0, 64, 64 };
	btnFairyBottle->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, btnFairyBottle->bounds.x + 8, btnFairyBottle->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, btnFairyBottle->bounds.x + 3, btnFairyBottle->bounds.y + btnFairyBottle->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "200", btnFairyBottle->bounds.x + 37, btnFairyBottle->bounds.y + btnFairyBottle->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 320, 0, 64, 64 };
	btnFairyTear->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, btnFairyTear->bounds.x + 8, btnFairyTear->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, btnFairyTear->bounds.x + 3, btnFairyTear->bounds.y + btnFairyTear->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "100", btnFairyTear->bounds.x + 37, btnFairyTear->bounds.y + btnFairyTear->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 384, 0, 64, 64 };
	btnFairyWing->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, btnFairyWing->bounds.x + 8, btnFairyWing->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, btnFairyWing->bounds.x, btnFairyWing->bounds.y + btnFairyWing->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "150", btnFairyWing->bounds.x + 37, btnFairyWing->bounds.y + btnFairyWing->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 128, 0, 64, 64 };
	btnOmniPotion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, btnOmniPotion->bounds.x + 8, btnOmniPotion->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, btnOmniPotion->bounds.x + 3, btnOmniPotion->bounds.y + btnOmniPotion->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "200", btnOmniPotion->bounds.x + 37, btnOmniPotion->bounds.y + btnOmniPotion->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 256, 0, 64, 64 };
	btnPotion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, btnPotion->bounds.x + 8, btnPotion->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, btnPotion->bounds.x + 3, btnPotion->bounds.y + btnPotion->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "100", btnPotion->bounds.x + 37, btnPotion->bounds.y + btnPotion->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	itSection = { 192, 0, 64, 64 };
	btnUltraPotion->Draw(app->render, showColliders, 40, { 255, 255, 255, 255 });
	app->render->DrawTexture(texture, btnUltraPotion->bounds.x + 8, btnUltraPotion->bounds.y + 8, &itSection);
	app->render->DrawTexture(texture, btnUltraPotion->bounds.x + 3, btnUltraPotion->bounds.y + btnUltraPotion->bounds.h + 3, &goldSection);
	app->render->DrawText(font, "150", btnUltraPotion->bounds.x + 37, btnUltraPotion->bounds.y + btnUltraPotion->bounds.h + 3, 30, 2, { 255, 255, 255, 255 });

	btnBack->Draw(app->render, showColliders, 40, { 0, 0, 0, 255 });
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

void Shop::UpdatingButtons(Input* input)
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

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_DOWN)
	{
		if (currentControl == nullptr)
		{
			currentControl = (*controls.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else if (currentControl->id >= 5 && currentControl->id < 9)
		{
			lastControl = currentControl;
			currentControl = btnBack;
		}
		else
		{
			eastl::list<GuiControl*>::iterator it = controls.begin();
			for (int i = 0; i < controls.size(); ++i, ++it)
			{
				if ((*it)->id == currentControl->id + 4)
				{
					currentControl = (*it);
					break;
				}
			}
		}
	}
	else if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_DOWN)
	{
		if (currentControl == nullptr)
		{
			currentControl = (*controls.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else if (currentControl->id == 9) // btnBack
		{
			currentControl = lastControl;
			lastControl = nullptr;
		}
		else
		{
			eastl::list<GuiControl*>::iterator it = controls.begin();
			for (int i = 0; i < controls.size(); ++i, ++it)
			{
				if ((*it)->id == currentControl->id - 4)
				{
					currentControl = (*it);
					break;
				}
			}
		}
	}
	else if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_DOWN)
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
	else if (input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_DOWN)
	{
		if (currentControl == nullptr)
		{
			currentControl = (*controls.begin());
			SDL_ShowCursor(SDL_DISABLE);
		}
		else if(currentControl->id + 1 < 9)
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