#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include"DialogueManager.h"
#include "Tabern.h"

Tabern::Tabern(iPoint position, pugi::xml_node anim, int id) :Npc(EntityType::TABERN, position, id)
{
	/*bounds = { 0,0, 16,32 };
	type = EntityType::TABERN;*/
	texture = app->tex->Load("Assets/Textures/Npc/Tabern.png");

	state = NpcState::WALLKING_RIGHT;
	pugi::xml_node player = anim.child("tabern").child("overworld");

	for (pugi::xml_node n = player.child("walk_front").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		walkDown.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("walk_left").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		walkLeft.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("walk_right").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		walkRight.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("walk_up").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		walkUp.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	idleDown.PushBack(walkDown.frames[0]);
	idleLeft.PushBack(walkLeft.frames[0]);
	idleRight.PushBack(walkRight.frames[0]);
	idleUp.PushBack(walkUp.frames[0]);



	currentAnim = &idleDown;
}

Tabern::~Tabern()
{
}

bool Tabern::Load()
{
	//scene->dialogueManager->LoadDialogue(0);
	return true;
}

bool Tabern::Update(float dt)
{
	Npc::Update(dt);

	return true;
}

void Tabern::Draw(bool showColliders)
{
	Npc::Draw(showColliders);
	if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
	SDL_Rect textureRect = { 449, 5, 46,55 };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
}

bool Tabern::UnLoad()
{
	return true;
}


bool Tabern::SaveState(pugi::xml_node& node)
{
	Npc::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("NpcType");

	auxiliar1.append_attribute("type").set_value("TABERN");
	return true;
}