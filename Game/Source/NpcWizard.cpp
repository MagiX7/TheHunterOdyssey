#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "NpcWizard.h"

NpcWizard::NpcWizard(iPoint position):Npc(EntityType::NPC_WIZARD,position)
{
	//bounds = { 0,0, 16,32 };
	//type = EntityType::NPC_WIZARD;
}

NpcWizard::~NpcWizard()
{
}

bool NpcWizard::Load()
{
	//texture = app->tex->Load("Assets/Textures/hunter_spritesheet.png");
	return true;
}

bool NpcWizard::Update(float dt)
{


	return true;
}

void NpcWizard::Draw(bool showColliders)
{
	if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
	SDL_Rect textureRect = { 23, 14, 51,81 };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &textureRect);
}

bool NpcWizard::UnLoad()
{
	return true;
}


bool NpcWizard::SaveState(pugi::xml_node& node)
{
	Npc::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("NpcType");

	auxiliar1.append_attribute("type").set_value("NPC_WIZARD");
	return true;
}