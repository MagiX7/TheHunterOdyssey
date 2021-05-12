#include "App.h"
#include "Render.h"

#include "KnightHelmet.h"

bool KnightHelmet::Load()
{
	atlasSection = { 101, 227, 23, 28 };

	return true;
}

void KnightHelmet::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}
