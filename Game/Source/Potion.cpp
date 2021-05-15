#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "Potion.h"

Potion::Potion(iPoint pos, SDL_Texture* atlas, eastl::string map) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 64, 289, 32, 32};
	itemType = ItemType::POTION;
	mapName = map;
}

Potion::~Potion()
{
}

bool Potion::Load()
{
	healAmount = 500;

	return true;
}

bool Potion::Update(float dt)
{
	return true;
}

void Potion::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool Potion::UnLoad()
{
	return true;
}

bool Potion::SaveState(pugi::xml_node& n)
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

void Potion::UseItem(Player* player)
{
	player->GetHealed(healAmount);
}