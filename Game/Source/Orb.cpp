#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Orb.h"

Orb::Orb(iPoint pos, SDL_Texture* atlas, eastl::string map) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 381, 568, 32, 32 };
	itemType = ItemType::ORB;
	mapName = map;
}

Orb::Orb(SDL_Texture* atlas) : Item(ObjectType::ITEM, iPoint(0,0), atlas)
{
	atlasSection = { 381, 568, 32, 32 };
	itemType = ItemType::ORB;
}

Orb::~Orb()
{
}

bool Orb::Load()
{
	texture = app->tex->Load("Textures/Items/items_atlas.png");

	return true;
}

bool Orb::Update(float dt)
{
	return true;
}

void Orb::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool Orb::UnLoad()
{
	return true;
}

bool Orb::SaveState(pugi::xml_node& n)
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