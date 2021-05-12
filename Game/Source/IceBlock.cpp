#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Player.h"

#include "IceBlock.h"

#include <time.h>

#define SPEED_X 60.0f
#define SPEED_Y 60.0f

IceBlock::IceBlock(EntityType type, iPoint position) : Entity(type)
{
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 64;
	bounds.h = 64;
}

IceBlock::~IceBlock()
{

}

bool IceBlock::Load()
{
	texture = app->tex->Load("Assets/Textures/Objects/block.png");

	return true;
}

void IceBlock::SetInactive()
{
	state = EntityState::INACTIVE;
}

void IceBlock::SetActive()
{
	state = GetNewState();
}

bool IceBlock::Update(float dt)
{
	lastPosition = bounds;

	/*bounds.x += SPEED_X * dt;
	bounds.y += SPEED_Y * dt;*/

	return true;
}

void IceBlock::Draw(bool showColliders)
{
	if (bounds.x + bounds.w > (-app->render->camera.x) && bounds.x < (-app->render->camera.x) + app->render->camera.w && bounds.y + bounds.h >(-app->render->camera.y) && bounds.y < (-app->render->camera.y) + app->render->camera.h) 
	{
		if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
		SDL_Rect rect = { 0,0,bounds.w,bounds.h };
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
	}
}

bool IceBlock::UnLoad()
{
	app->tex->UnLoad(texture);

	return true;
}

bool IceBlock::SaveState(pugi::xml_node& node)
{
	pugi::xml_node auxiliar1 = node.append_child("bounds");
	auxiliar1.append_attribute("X").set_value(bounds.x);
	auxiliar1.append_attribute("Y").set_value(bounds.y);
	auxiliar1.append_attribute("W").set_value(bounds.w);
	auxiliar1.append_attribute("H").set_value(bounds.h);

	return true;
}

bool IceBlock::CheckCollision(Player* player)
{
	if (state != EntityState::INACTIVE)
	{
		if (player->bounds.x < bounds.x + ((bounds.w + FIND_RADIOUS) * 2) && player->bounds.x > bounds.x - (bounds.w + FIND_RADIOUS) && player->bounds.y< bounds.y + ((bounds.h + FIND_RADIOUS) * 2) && player->bounds.y > bounds.y - (bounds.h + FIND_RADIOUS))
		{

		}
	}
	return false;
}

EntityState IceBlock::GetNewState()
{
	return EntityState::INACTIVE;
}

void IceBlock::OnCollision()
{

}