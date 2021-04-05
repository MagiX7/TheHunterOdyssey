#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include"DialogueManager.h"

#include "Town.h"

Town::Town(iPoint position) :Npc(EntityType::TOWN, position)
{
	/*bounds = { 0,0, 16,32 };
	type = EntityType::TOWN;*/
	texture = app->tex->Load("Assets/Textures/Npc/Town.png");
	dialogeId = 0;
}

Town::~Town()
{
}

bool Town::Load()
{
	//scene->dialogueManager->current = scene->dialogueManager->LoadDialogue(0);
	return true;
}

bool Town::Update(float dt)
{
	Npc::Update(dt);

	return true;
}

void Town::Draw(bool showColliders)
{
	Npc::Draw(showColliders);
	if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
	SDL_Rect textureRect = { 446, 11, 43,49  };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &textureRect);
	
	
}

bool Town::UnLoad()
{
	return true;
}


bool Town::SaveState(pugi::xml_node& node)
{
	Npc::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("NpcType");

	auxiliar1.append_attribute("type").set_value("TOWN");
	return true;
}

