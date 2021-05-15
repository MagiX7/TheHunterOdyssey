#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "UltraPotion.h"

UltraPotion::UltraPotion(iPoint pos, SDL_Texture* atlas, eastl::string map) : Item(ObjectType::ITEM, pos, atlas)
{
	/*bounds.x = pos.x - 5;
	bounds.y = pos.y - 5;
	bounds.w = 40;
	bounds.h = 40;*/
	atlasSection = { 194, 289, 32, 32 };
	itemType = ItemType::ULTRA_POTION;
	mapName = map;
}

UltraPotion::~UltraPotion()
{
}

bool UltraPotion::Load()
{
	healAmount = 1000;

	return true;
}

bool UltraPotion::Update(float dt)
{
	return true;
}

void UltraPotion::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
	if (showColliders)
	{
		app->render->DrawRectangle(bounds, 255, 0, 0);
	}
}

bool UltraPotion::UnLoad()
{
	return false;
}

bool UltraPotion::SaveState(pugi::xml_node& n)
{
	pugi::xml_node node = n.append_child("position");
	node.append_attribute("x").set_value(bounds.x);
	node.append_attribute("y").set_value(bounds.y);

	node = n.append_child("object_type");
	node.append_attribute("value").set_value((int)objectType);

	node = n.append_child("item_type");
	node.append_attribute("value").set_value((int)itemType);

	node = n.append_child("map_name");
	node.append_attribute("value").set_value(mapName.c_str());

	return true;
}

void UltraPotion::UseItem(Player* player)
{
	player->GetHealed(healAmount);
}