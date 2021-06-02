#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Merchant.h"

Merchant::Merchant(iPoint position, pugi::xml_node anim, int id) : Npc(EntityType::MERCHANT, position, id)
{
	texture = app->tex->Load("Maps/map_tileset.png");

	dialogeId = id;
	pugi::xml_node player = anim.child("ray").child("overworld");

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

Merchant::~Merchant()
{
}

bool Merchant::Load()
{
	return true;
}

bool Merchant::Update(float dt)
{
	
	return true;
}

void Merchant::Draw(bool showColliders)
{
	if (state != EntityState::INACTIVE)
	{
		if (bounds.x + bounds.w > (-app->render->camera.x) && bounds.x < (-app->render->camera.x) + app->render->camera.w && bounds.y + bounds.h >(-app->render->camera.y) && bounds.y < (-app->render->camera.y) + app->render->camera.h) 
		{
			Npc::Draw(showColliders);
			if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);

			SDL_Rect textureRect = { 14, 11, 46,49 };
			app->render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
		}
	}
}

bool Merchant::UnLoad()
{
	return true;
}