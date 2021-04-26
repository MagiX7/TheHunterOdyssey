#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Potion.h"

Potion::Potion() : Item(ItemType::POTION)
{
}

Potion::~Potion()
{
}

bool Potion::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/potion.png");

	return true;
}

bool Potion::Update(float dt)
{
	return true;
}

void Potion::Draw(bool showColliders, SDL_Rect bounds)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool Potion::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}