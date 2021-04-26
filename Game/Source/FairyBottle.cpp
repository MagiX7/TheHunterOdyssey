#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyBottle.h"

FairyBottle::FairyBottle() : Item(ItemType::FAIRY_BOTTLE)
{
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

void FairyBottle::Draw(bool showColliders, SDL_Rect bounds)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool FairyBottle::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}