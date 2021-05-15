#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "OrbFragment.h"

OrbFragment::OrbFragment(iPoint pos, SDL_Texture* atlas, eastl::string map) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 433, 568, 32, 32 };
	itemType = ItemType::ORB_FRAGMENT;
	mapName = map;
}

OrbFragment::~OrbFragment()
{
}

bool OrbFragment::Load()
{
	texture = app->tex->Load("Textures/Items/items_atlas.png");

	return true;
}

bool OrbFragment::Update(float dt)
{
	return true;
}

void OrbFragment::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool OrbFragment::UnLoad()
{
	return true;
}

bool OrbFragment::SaveState(pugi::xml_node& n)
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