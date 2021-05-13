#include "App.h"
#include "Render.h"

#include "KnightHelmet.h"

bool KnightHelmet::Load()
{
	atlasSection = { 96, 225, 32, 32 };

	return true;
}

void KnightHelmet::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}
