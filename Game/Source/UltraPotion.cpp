#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "UltraPotion.h"

UltraPotion::UltraPotion(iPoint pos) : Item(ItemType::ULTRA_POTION, pos)
{
	/*bounds.x = pos.x - 5;
	bounds.y = pos.y - 5;
	bounds.w = 40;
	bounds.h = 40;*/
	atlasSection = { 194, 289, 32, 32 };
}

UltraPotion::~UltraPotion()
{
}

bool UltraPotion::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/ultra_potion.png");

	healAmount = 1000;

	return true;
}

bool UltraPotion::Update(float dt)
{
	return true;
}

void UltraPotion::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
	if (showColliders)
	{
		app->render->DrawRectangle(bounds, 255, 0, 0);
	}
}

bool UltraPotion::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}

void UltraPotion::UseItem(Player* player)
{
	player->GetHealed(healAmount);
}