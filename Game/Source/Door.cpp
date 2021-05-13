#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Audio.h"
#include "Player.h"
#include "SceneGameplay.h"

#include "Door.h"

#include <time.h>

#define SPEED_X 180.0f
#define SPEED_Y 180.0f

Door::Door(EntityType type, iPoint position, Player* player) : Entity(type)
{
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 64;
	bounds.h = 56;
	state = EntityState::STOP_DOWN;
	tmpState = EntityState::STOP_DOWN;
	currPlayer = player;
}

Door::~Door()
{

}

bool Door::Load()
{
	texture = app->tex->Load("Assets/Textures/Objects/door_1.png");

	return true;
}

void Door::SetInactive()
{
	state = EntityState::INACTIVE;
}

void Door::SetActive()
{
	state = GetNewState();
}

bool Door::Update(float dt)
{
	if (CheckCollision(currPlayer)) currPlayer->bounds = lastPosition;

	if (!CheckCollision(currPlayer))
	{
		lastPosition = currPlayer->bounds;

		switch (state)
		{
		case EntityState::WALLKING_LEFT:
			bounds.x -= SPEED_X * dt;
			break;
		case EntityState::WALKING_RIGHT:
			bounds.x += SPEED_X * dt;
			break;
		case EntityState::WALLKING_UP:
			bounds.y -= SPEED_Y * dt;
			break;
		case EntityState::WALLKING_DOWN:
			bounds.y += SPEED_Y * dt;
			break;
		default:
			break;
		}
	}

	return true;
}

void Door::Draw(bool showColliders)
{
	if (bounds.x + bounds.w > (-app->render->camera.x) && bounds.x < (-app->render->camera.x) + app->render->camera.w && bounds.y + bounds.h >(-app->render->camera.y) && bounds.y < (-app->render->camera.y) + app->render->camera.h) 
	{
		SDL_Rect rect = { 0,0,bounds.w,bounds.h };
		app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);
		if (showColliders) app->render->DrawRectangle(bounds, 0, 0, 255, 100);
	}
}

bool Door::UnLoad()
{
	app->tex->UnLoad(texture);

	return true;
}

bool Door::SaveState(pugi::xml_node& node)
{
	pugi::xml_node auxiliar1 = node.append_child("bounds");
	auxiliar1.append_attribute("X").set_value(bounds.x);
	auxiliar1.append_attribute("Y").set_value(bounds.y);
	auxiliar1.append_attribute("W").set_value(bounds.w);
	auxiliar1.append_attribute("H").set_value(bounds.h);

	return true;
}

bool Door::CheckCollision(Player* player)
{
	//if (state != EntityState::INACTIVE)
	//{
	//	//PULL BLOCK
	//	if (player->bounds.x + player->bounds.w > bounds.x && player->bounds.x < bounds.x + bounds.w && player->bounds.y + player->bounds.h > bounds.y && player->bounds.y < bounds.y + bounds.h)
	//	{
	//		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && !isMoving)
	//		{
	//			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//			{
	//				state = EntityState::WALLKING_UP;
	//			}
	//			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//			{
	//				state = EntityState::WALLKING_DOWN;
	//			}
	//			else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//			{
	//				state = EntityState::WALLKING_LEFT;
	//			}
	//			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//			{
	//				state = EntityState::WALKING_RIGHT;
	//			}
	//		}
	//		return true;
	//	}
	//}
	return false;
}

EntityState Door::GetNewState()
{
	return EntityState::INACTIVE;
}

void Door::OnCollision()
{

}

void Door::SetBounds(SDL_Rect myBounds)
{
	bounds = myBounds;
}