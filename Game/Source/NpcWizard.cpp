#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "NpcWizard.h"

NpcWizard::NpcWizard(iPoint position, pugi::xml_node anim, int id):Npc(EntityType::NPC_WIZARD,position, id)
{
	//bounds = { 0,0, 16,32 };
	//type = EntityType::NPC_WIZARD;
	texture = app->tex->Load("Assets/Textures/Npc/Wizard.png");

	state = EntityState::WALKING_RIGHT;
	pugi::xml_node player = anim.child("npc_wizard").child("overworld");

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

NpcWizard::~NpcWizard()
{
}

bool NpcWizard::Load()
{
	
	
	return true;
}

bool NpcWizard::Update(float dt)
{
	if (state != EntityState::INACTIVE)Npc::Update(dt);		
	return true;
}

void NpcWizard::Draw(bool showColliders)
{
	if (state != EntityState::INACTIVE) {
		Npc::Draw(showColliders);
		if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
		SDL_Rect textureRect = { 14, 11, 46,49 };
		app->render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
	}

}

bool NpcWizard::UnLoad()
{
	app->tex->UnLoad(texture);
	return true;
}


bool NpcWizard::SaveState(pugi::xml_node& node)
{
	Npc::SaveState(node);
	pugi::xml_node auxiliar1;
	pugi::xml_node auxiliar2;
	auxiliar1 = node.append_child("NpcType");
	auxiliar2 = node.append_child("Dialoge");
	auxiliar2.append_attribute("idDialoge").set_value(dialogeId);
	auxiliar1.append_attribute("type").set_value("NPC_WIZARD");
	return true;
}