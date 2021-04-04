#include "App.h"
#include "Render.h"
#include"Input.h"
#include "Npc.h"
#include"DialogueManager.h"
#include"Player.h"
#include"SceneGameplay.h"
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
	Player*auxiliar=scene->getCurrentPlayer();
	if (talkStart) 
	{		
		scene->dialogueManager->Update(dt);
		
	}
	
	if (auxiliar->bounds.x< bounds.x + ((bounds.w + FIND_RADIOUS) * 2) && auxiliar->bounds.x > bounds.x - (bounds.w + FIND_RADIOUS) && auxiliar->bounds.y< bounds.y + ((bounds.h + FIND_RADIOUS) * 2) && auxiliar->bounds.y > bounds.y - (bounds.h + FIND_RADIOUS))
	{
		drawPtext = true;
		if (app->input->GetKey(SDL_SCANCODE_O) == KeyState::KEY_DOWN)
		{
			scene->dialogueManager->printText = true;
			drawPtext = false;
			talkStart = true;
		}
	}else
	{ 
		
		drawPtext = false; 
		talkStart = false;
	}

	

	return true;
}

void Npc::Draw(bool showColliders)
{
	if (talkStart) 
	{
		scene->dialogueManager->Draw();
	}
	else 
	{
		if (showColliders)
			app->render->DrawRectangle(bounds, 255, 255, 255, 255);
		if (drawPtext)app->render->DrawText(font, "Press O to talk", bounds.x - 30, bounds.y - 30, 20, 5, { 255,255,255 });
	}
}

bool Npc::UnLoad()
{
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