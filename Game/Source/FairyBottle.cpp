#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyBottle.h"

FairyBottle::FairyBottle(iPoint pos, SDL_Texture* atlas) : Item(ItemType::FAIRY_BOTTLE, pos, atlas)
{
	atlasSection = { 353, 289, 32, 32 };
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