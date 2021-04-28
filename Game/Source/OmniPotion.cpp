#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "OmniPotion.h"

OmniPotion::OmniPotion(iPoint pos) : Item(ItemType::OMNI_POTION, pos)
{
	atlasSection = { 325, 290, 24, 28 };
}

OmniPotion::~OmniPotion()
{
}

bool OmniPotion::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/omni_potion.png");

	return true;
}

bool OmniPotion::Update(float dt)
{
	return true;
}

void OmniPotion::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool OmniPotion::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}