#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Npc.h"
#include "DialogueManager.h"
#include "Player.h"
#include "SceneGameplay.h"

Npc::Npc(EntityType type, iPoint position) : Entity(type)
{
	drawPtext=false;
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 32;
	bounds.h = 32;
	font = new Font("Assets/Font/font3.xml", app->tex);
	drawPtext = false;

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
	//Player* auxiliar = scene->getCurrentPlayer();

	//if (talkStart) 
	//{		
	//	scene->dialogueManager->Update(dt);
	//}
	
	

	return true;
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

		if(app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
			return true;
	}
	else
	{
		drawPtext = false;
		talkStart = false;
		return false;
	}
}