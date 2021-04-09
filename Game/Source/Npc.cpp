#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Npc.h"
#include "DialogueManager.h"
#include "Player.h"
#include "SceneGameplay.h"
#include<time.h>

#define SPEED_X 100.0f
#define SPEED_Y 100.0f
Npc::Npc(EntityType type, iPoint position) : Entity(type)
{
	drawPtext = false;
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 30;
	bounds.h = 32;
	font = new Font("Assets/Font/font3.xml", app->tex);
	drawPtext = false;
	stateTimer = 0;
	srand(time(NULL));
	stateMaxTimer = 2;
	npcMove = true;

}

Npc::~Npc()
{
}

bool Npc::Load()
{
	return true;
}

bool Npc::Update(float dt)
{
	lastPosition = bounds;
	currentAnim->speed = 10.0f * dt;
	if (stateTimer > stateMaxTimer)
	{
		state = getNewState();
	}
	stateTimer += dt;

	switch (state)
	{
	case NpcState::WALLKING_LEFT:
		bounds.x -= SPEED_X * dt;
		currentAnim = &walkLeft;
		break;
	case NpcState::WALLKING_RIGHT:
		bounds.x += SPEED_X * dt;
		currentAnim = &walkRight;
		break;
	case NpcState::WALLKING_UP:
		bounds.y -= SPEED_Y * dt;
		currentAnim = &walkUp;
		break;
	case NpcState::WALLKING_DOWN:
		bounds.y += SPEED_Y * dt;
		currentAnim = &walkDown;
		break;
	case NpcState::STOP_LEFT:
		currentAnim = &idleLeft;
		break;
	case NpcState::STOP_RIGHT:
		currentAnim = &idleRight;
		break;
	case NpcState::STOP_UP:
		currentAnim = &idleUp;
		break;
	case NpcState::STOP_DOWN:
		currentAnim = &idleDown;
		break;
	default:
		break;
	}
	currentAnim->Update();
	return true;
}
void Npc::NpcMove(bool move)
{
	npcMove = move;
	if (move == false)state = NpcState::STOP_DOWN;
}
void Npc::Draw(bool showColliders)
{
	if (showColliders)
		app->render->DrawRectangle(bounds, 255, 255, 255, 255);

	if (drawPtext) app->render->DrawText(font, "Press O to talk", bounds.x - 30, bounds.y - 30, 20, 5, { 255,255,255 });
}

bool Npc::UnLoad()
{
	RELEASE(font);
	return true;
}

bool Npc::SaveState(pugi::xml_node& node)
{
	pugi::xml_node auxiliar1 = node.append_child("bounds");
	auxiliar1.append_attribute("X").set_value(bounds.x);
	auxiliar1.append_attribute("Y").set_value(bounds.y);
	auxiliar1.append_attribute("W").set_value(bounds.w);
	auxiliar1.append_attribute("H").set_value(bounds.h);
	
	return true;
}

bool Npc::CheckCollision(Player* player)
{
	if (player->bounds.x < bounds.x + ((bounds.w + FIND_RADIOUS) * 2) && player->bounds.x > bounds.x - (bounds.w + FIND_RADIOUS) && player->bounds.y< bounds.y + ((bounds.h + FIND_RADIOUS) * 2) && player->bounds.y > bounds.y - (bounds.h + FIND_RADIOUS))
	{
		drawPtext = true;

		if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		{
			return true;
			state = NpcState::TALKING;
		}

	}
	else if (state == NpcState::TALKING)
	{
		state = getNewState();
		drawPtext = false;
		talkStart = false;
		return false;
	}
	else
	{
		drawPtext = false;
		talkStart = false;
		return false;
	}
	return false;
}

NpcState Npc::getNewState()
{
	int randNum;
	stateMaxTimer = (rand() % 5) + 2;
	int arc;
	int cant;
	if (npcMove)
	{
		arc = 0;
		cant = 8;
	}
	else
	{
		cant = 4;
		arc = 4;
	}
	randNum = ((rand() % cant) + arc);
	stateTimer = 0;
	switch (randNum)
	{
	case 0:
		return NpcState::WALLKING_UP;
		break;
	case 1:
		return NpcState::WALLKING_RIGHT;
		break;
	case 2:
		return NpcState::WALLKING_LEFT;
		break;
	case 3:
		return NpcState::WALLKING_DOWN;
		break;
	case 4:
		return NpcState::STOP_UP;
		break;
	case 5:
		return NpcState::STOP_RIGHT;
		break;
	case 6:
		return NpcState::STOP_LEFT;
		break;
	case 7:
		return NpcState::STOP_DOWN;
		break;
	default:
		break;
	}
}

void Npc::onCollision()
{
	if (npcMove) {
		bounds = lastPosition;
		switch (state)
		{
		case NpcState::WALLKING_LEFT:
			while (state == NpcState::WALLKING_LEFT || state == NpcState::STOP_DOWN || state == NpcState::STOP_LEFT || state == NpcState::STOP_RIGHT || state == NpcState::STOP_UP)state = getNewState();
			break;
		case NpcState::WALLKING_RIGHT:
			while (state == NpcState::WALLKING_RIGHT || state == NpcState::STOP_DOWN || state == NpcState::STOP_LEFT || state == NpcState::STOP_RIGHT || state == NpcState::STOP_UP)state = getNewState();
			break;
		case NpcState::WALLKING_UP:
			while (state == NpcState::WALLKING_UP || state == NpcState::STOP_DOWN || state == NpcState::STOP_LEFT || state == NpcState::STOP_RIGHT || state == NpcState::STOP_UP)state = getNewState();
			break;
		case NpcState::WALLKING_DOWN:
			while (state == NpcState::WALLKING_DOWN || state == NpcState::STOP_DOWN || state == NpcState::STOP_LEFT || state == NpcState::STOP_RIGHT || state == NpcState::STOP_UP)state = getNewState();
			break;
		default:
			break;
		}
	}

}