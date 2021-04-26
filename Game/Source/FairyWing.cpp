#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyWing.h"

FairyWing::FairyWing() : Item(ItemType::FAIRY_WING)
{
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

void FairyWing::Draw(bool showColliders, SDL_Rect bounds)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool FairyWing::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}