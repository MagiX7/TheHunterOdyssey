#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Npc.h"
#include "DialogueManager.h"
#include "Player.h"
#include "SceneGameplay.h"
#include<time.h>

#define SPEED_X 60.0f
#define SPEED_Y 60.0f
Npc::Npc(EntityType type, iPoint position, int id) : Entity(type), dialogeId(id)
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
	app->tex->UnLoad(texture);
	font->UnLoad(app->tex);
	delete font;
}

bool Npc::Load()
{
	return true;
}
void Npc::setInactive(){ state = EntityState::INACTIVE;}
void Npc::setActive() { state = getNewState(); }
EntityState Npc::getState() { return state; }
bool Npc::Update(float dt)
{
	
	lastPosition = bounds;
	currentAnim->speed = 7.0f * dt;
	if (stateTimer > stateMaxTimer)
	{
		state = getNewState();
	}
	stateTimer += dt;

	switch (state)
	{
	case EntityState::WALLKING_LEFT:
		bounds.x -= SPEED_X * dt;
		currentAnim = &walkLeft;
		break;
	case EntityState::WALLKING_RIGHT:
		bounds.x += SPEED_X * dt;
		currentAnim = &walkRight;
		break;
	case EntityState::WALLKING_UP:
		bounds.y -= SPEED_Y * dt;
		currentAnim = &walkUp;
		break;
	case EntityState::WALLKING_DOWN:
		bounds.y += SPEED_Y * dt;
		currentAnim = &walkDown;
		break;
	case EntityState::STOP_LEFT:
		currentAnim = &idleLeft;
		break;
	case EntityState::STOP_RIGHT:
		currentAnim = &idleRight;
		break;
	case EntityState::STOP_UP:
		currentAnim = &idleUp;
		break;
	case EntityState::STOP_DOWN:
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
	if (move == false)state = EntityState::STOP_DOWN;
}
void Npc::Draw(bool showColliders)
{
	if (showColliders)
		app->render->DrawRectangle(bounds, 255, 255, 255, 255);

	if (drawPtext) app->render->DrawText(font, "Press O to talk", (bounds.x - 30)- (app->render->camera.x * (-1)), (bounds.y - 30) - (app->render->camera.y*(-1)), 20, 5, { 255,255,255 });
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
void Npc::setDrawPtext(bool DrawPtext) {
	drawPtext=DrawPtext;
}
void Npc::setTalkStart(bool TalkStart) {
	talkStart=TalkStart;
}
int Npc::getDialogeId() 
{
	return dialogeId;
}
bool Npc::CheckCollision(Player* player)
{
	if (state != EntityState::INACTIVE) {
		if (player->bounds.x < bounds.x + ((bounds.w + FIND_RADIOUS) * 2) && player->bounds.x > bounds.x - (bounds.w + FIND_RADIOUS) && player->bounds.y< bounds.y + ((bounds.h + FIND_RADIOUS) * 2) && player->bounds.y > bounds.y - (bounds.h + FIND_RADIOUS))
		{
			drawPtext = true;

			if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN || app->input->pads->a)
			{
				return true;
				state = EntityState::TALKING;
			}

		}
		else if (state == EntityState::TALKING)
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
	}
	return false;
}

EntityState Npc::getNewState()
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
		return EntityState::WALLKING_UP;
		break;
	case 1:
		return EntityState::WALLKING_RIGHT;
		break;
	case 2:
		return EntityState::WALLKING_LEFT;
		break;
	case 3:
		return EntityState::WALLKING_DOWN;
		break;
	case 4:
		return EntityState::STOP_UP;
		break;
	case 5:
		return EntityState::STOP_RIGHT;
		break;
	case 6:
		return EntityState::STOP_LEFT;
		break;
	case 7:
		return EntityState::STOP_DOWN;
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
		case EntityState::WALLKING_LEFT:
			while (state == EntityState::WALLKING_LEFT || state == EntityState::STOP_DOWN || state == EntityState::STOP_LEFT || state == EntityState::STOP_RIGHT || state == EntityState::STOP_UP)state = getNewState();
			break;
		case EntityState::WALLKING_RIGHT:
			while (state == EntityState::WALLKING_RIGHT || state == EntityState::STOP_DOWN || state == EntityState::STOP_LEFT || state == EntityState::STOP_RIGHT || state == EntityState::STOP_UP)state = getNewState();
			break;
		case EntityState::WALLKING_UP:
			while (state == EntityState::WALLKING_UP || state == EntityState::STOP_DOWN || state == EntityState::STOP_LEFT || state == EntityState::STOP_RIGHT || state == EntityState::STOP_UP)state = getNewState();
			break;
		case EntityState::WALLKING_DOWN:
			while (state == EntityState::WALLKING_DOWN || state == EntityState::STOP_DOWN || state == EntityState::STOP_LEFT || state == EntityState::STOP_RIGHT || state == EntityState::STOP_UP)state = getNewState();
			break;
		default:
			break;
		}
	}

}