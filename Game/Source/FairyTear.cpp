#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "FairyTear.h"

FairyTear::FairyTear(iPoint pos) : Item(ItemType::FAIRY_TEAR, pos)
{
}

FairyTear::~FairyTear()
{
}

bool FairyTear::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/fairy_tear.png");

	return true;
}

bool FairyTear::Update(float dt)
{
	return true;
}

void FairyTear::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool FairyTear::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}