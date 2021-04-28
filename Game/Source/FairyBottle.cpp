#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyBottle.h"

FairyBottle::FairyBottle(iPoint pos) : Item(ItemType::FAIRY_BOTTLE, pos)
{
	atlasSection = { 353, 289, 32, 32 };
}

FairyBottle::~FairyBottle()
{
}

bool FairyBottle::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/fairy_bottle.png");

	return true;
}

bool FairyBottle::Update(float dt)
{
	return true;
}

void FairyBottle::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool FairyBottle::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}