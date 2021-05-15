#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyTear.h"

FairyTear::FairyTear(iPoint pos, SDL_Texture* atlas, eastl::string map) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 96, 289, 32, 32 };
	itemType = ItemType::FAIRY_TEAR;
	mapName = map;
}

FairyTear::~FairyTear()
{
}

bool FairyTear::Load()
{
	return true;
}

bool FairyTear::Update(float dt)
{
	return true;
}

void FairyTear::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool FairyTear::UnLoad()
{
	return true;
}