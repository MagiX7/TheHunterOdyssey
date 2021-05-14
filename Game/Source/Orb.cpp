#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Orb.h"

Orb::Orb(iPoint pos, SDL_Texture* atlas) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 383, 570, 28, 28 };
	itemType = ItemType::ORB;
}

Orb::Orb(SDL_Texture* atlas) : Item(ObjectType::ITEM, iPoint(0,0), atlas)
{
	atlasSection = { 383, 570, 28, 28 };
	itemType = ItemType::ORB;
}

Orb::~Orb()
{
}

bool Orb::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/items_atlas.png");

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