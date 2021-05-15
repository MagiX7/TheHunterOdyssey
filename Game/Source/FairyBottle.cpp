#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyBottle.h"

FairyBottle::FairyBottle(iPoint pos, SDL_Texture* atlas, eastl::string map) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 353, 289, 32, 32 };
	itemType = ItemType::FAIRY_BOTTLE;
	mapName = map;
}

FairyBottle::~FairyBottle()
{
}

bool FairyBottle::Load()
{
	return true;
}

bool FairyBottle::Update(float dt)
{
	return true;
}

void FairyBottle::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool FairyBottle::UnLoad()
{
	return true;
}