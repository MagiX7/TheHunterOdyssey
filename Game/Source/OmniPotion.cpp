#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "OmniPotion.h"

OmniPotion::OmniPotion(iPoint pos, SDL_Texture* atlas) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 325, 290, 24, 28 };
	itemType = ItemType::OMNI_POTION;
}

OmniPotion::~OmniPotion()
{
}

bool OmniPotion::Load()
{
	return true;
}

bool OmniPotion::Update(float dt)
{
	return true;
}

void OmniPotion::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool OmniPotion::UnLoad()
{
	return true;
}