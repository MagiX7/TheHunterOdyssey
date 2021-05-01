#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Player.h"

#include "Potion.h"

Potion::Potion(iPoint pos) : Item(ItemType::POTION, pos)
{
	atlasSection = { 64, 289, 32, 32};
}

Potion::~Potion()
{
}

bool Potion::Load()
{
	texture = app->tex->Load("Assets/Textures/Items/potion.png");

	healAmount = 500;

	return true;
}

bool Potion::Update(float dt)
{
	return true;
}

void Potion::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y);
}

bool Potion::UnLoad()
{
	app->tex->UnLoad(texture);

	return false;
}

void Potion::UseItem(Player* player)
{
	player->GetHealed(healAmount);
}