#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyWing.h"

FairyWing::FairyWing(iPoint pos, SDL_Texture* atlas) : Item(ItemType::FAIRY_WING, pos, atlas)
{
	atlasSection = { 225, 289, 32, 32 };
}

FairyWing::~FairyWing()
{
}

bool FairyWing::Load()
{
	return true;
}

bool FairyWing::Update(float dt)
{
	return true;
}

void FairyWing::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool FairyWing::UnLoad()
{
	return true;
}