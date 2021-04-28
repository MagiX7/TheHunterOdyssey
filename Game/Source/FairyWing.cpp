#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyWing.h"

FairyWing::FairyWing(iPoint pos) : Item(ItemType::FAIRY_WING, pos)
{
	atlasSection = { 225, 289, 32, 32 };
}

FairyWing::~FairyWing()
{
}

bool FairyWing::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/fairy_wing.png");

	return true;
}

bool FairyWing::Update(float dt)
{
	return true;
}

void FairyWing::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool FairyWing::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}