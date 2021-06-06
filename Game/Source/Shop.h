#pragma once

#include "Menu.h"
#include "GuiButton.h"

class SceneGameplay;
class Inventory;

enum class ItemType;
enum class ArmorType;

class Shop : public Menu
{
public:
	Shop(SceneGameplay* s, Inventory* inven, SDL_Texture* at, Player* current);

	virtual ~Shop();

	bool Load(Font* font) override;

	bool Update(float dt) override;

	void Draw(Font* font, bool showColliders);

	bool UnLoad();

	bool OnGuiMouseClickEvent(GuiControl* control);

	void UpdatingButtons(Input* input) override;

	Item* CreateItem(ItemType type);
	Item* CreateItem(ArmorType type);

private:
	SceneGameplay* scene;
	Inventory* inventory;
	SDL_Texture* atlas;
	SDL_Texture* texture;
	SDL_Texture* background;
	Player* player;

	GuiButton* btnHelmet;
	GuiButton* btnChest;
	GuiButton* btnFairyBottle;
	GuiButton* btnFairyTear;
	GuiButton* btnFairyWing;
	GuiButton* btnOmniPotion;
	GuiButton* btnPotion;
	GuiButton* btnUltraPotion;
	GuiButton* btnBack;
};