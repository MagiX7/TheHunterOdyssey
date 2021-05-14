#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Statue.h"
#include "Player.h"

Statue::Statue(EntityType type, iPoint position, Player* player)
{
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 64;
	bounds.h = 56;
	state = EntityState::STOP_DOWN;
	tmpState = EntityState::STOP_DOWN;
	currPlayer = player;
}

Statue::~Statue()
{
}

bool Statue::Load()
{
	texture = app->tex->Load("Assets/Textures/Objects/statue_1");

	return true;
}

bool Statue::Update(float dt)
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

void Statue::Draw(bool showColliders)
{
}

bool Statue::UnLoad()
{
	return true;
}

bool Statue::SaveState(pugi::xml_node& node)
{
	pugi::xml_node auxiliar1 = node.append_child("bounds");
	auxiliar1.append_attribute("X").set_value(bounds.x);
	auxiliar1.append_attribute("Y").set_value(bounds.y);
	auxiliar1.append_attribute("W").set_value(bounds.w);
	auxiliar1.append_attribute("H").set_value(bounds.h);

	return true;
}

bool Statue::CheckCollision(Player* player)
{
	if (state != EntityState::INACTIVE)
{
	//PULL BLOCK
	if (player->bounds.x + player->bounds.w > bounds.x && player->bounds.x < bounds.x + bounds.w && player->bounds.y + player->bounds.h > bounds.y && player->bounds.y < bounds.y + bounds.h)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			
		}
		return true;
	}
}
	return true;
}

void Statue::SetInactive()
{
	state = EntityState::INACTIVE;
}

void Statue::SetActive()
{
	state = GetNewState();
}

void Statue::SetBounds(SDL_Rect myBounds)
{
	bounds = myBounds;
}

EntityState Statue::GetNewState()
{
	return EntityState();
}

void Statue::OnCollision()
{
}
