#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "InterruptorBlock.h"

InterruptorBlock::InterruptorBlock(iPoint pos, SDL_Texture* atlas, eastl::string map) : Item(ObjectType::ITEM, pos, atlas)
{
	atlasSection = { 435, 615, 28, 26 };
	itemType = ItemType::INTERRUPTOR_BLOCK;
	isDropped = false;
	mapName = map;
}

InterruptorBlock::~InterruptorBlock()
{
}

bool InterruptorBlock::Load()
{
	texture = app->tex->Load("Textures/Items/items_atlas.png");
	return true;
}

bool InterruptorBlock::Update(float dt)
{
	return true;
}

void InterruptorBlock::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool InterruptorBlock::UnLoad()
{
	return true;
}