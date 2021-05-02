#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "UltraPotion.h"

UltraPotion::UltraPotion(iPoint pos, SDL_Texture* atlas) : Item(ItemType::ULTRA_POTION, pos, atlas)
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
	healAmount = 1000;

	return true;
}

bool UltraPotion::Update(float dt)
{
	return true;
}

void UltraPotion::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
	if (showColliders)
	{
		app->render->DrawRectangle(bounds, 255, 0, 0);
	}
}

bool UltraPotion::UnLoad()
{
	return false;
}

void UltraPotion::UseItem(Player* player)
{
	player->GetHealed(healAmount);
}