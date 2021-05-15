#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "OrbFragment.h"

OrbFragment::OrbFragment(iPoint pos, SDL_Texture* atlas, eastl::string map) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 439, 570, 20, 28 };
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