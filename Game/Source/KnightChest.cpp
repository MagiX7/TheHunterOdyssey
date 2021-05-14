#include "App.h"
#include "Render.h"

#include "KnightChest.h"

bool KnightChest::Load()
{
	atlasSection = { 164, 228, 26, 27 };

	return true;
}

void KnightChest::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}
