#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyTear.h"

FairyTear::FairyTear(iPoint pos, SDL_Texture* atlas) : Item(ItemType::FAIRY_TEAR, pos, atlas)
{
	atlasSection = { 96, 289, 32, 32 };
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