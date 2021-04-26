#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "UltraPotion.h"

UltraPotion::UltraPotion() : Item(ItemType::ULTRA_POTION)
{
}

UltraPotion::~UltraPotion()
{
}

bool UltraPotion::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/ultra_potion.png");

	return true;
}

bool UltraPotion::Update(float dt)
{
	return true;
}

void UltraPotion::Draw(bool showColliders, SDL_Rect bounds)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool UltraPotion::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}