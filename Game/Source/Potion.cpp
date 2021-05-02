#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "Potion.h"

Potion::Potion(iPoint pos, SDL_Texture* atlas) : Item(ItemType::POTION, pos, atlas)
{
	atlasSection = { 64, 289, 32, 32};
}

Potion::~Potion()
{
}

bool Potion::Load()
{
	healAmount = 500;

	return true;
}

bool Potion::Update(float dt)
{
	return true;
}

void Potion::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool Potion::UnLoad()
{
	return true;
}

void Potion::UseItem(Player* player)
{
	player->GetHealed(healAmount);
}