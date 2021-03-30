#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Wizard.h"

Wizard::Wizard(iPoint position) : Player(PlayerType::WIZARD, EntityType::HUNTER, position)
{
	//bounds = { 0,0, 16,32 };
	stance = PlayerStance::ROAMING;
	healthPoints = 1000;
	manaPoints = 5000;
	name = "Wizard";
}

Wizard::~Wizard()
{
}

bool Wizard::Load()
{
	// TODO implement textures for wizard. 
	//app->tex->Load();
	return true;
}

bool Wizard::Update(float dt)
{
	HandleInput(dt);

	return true;
}

void Wizard::Draw(bool showColliders)
{
	if (showColliders) app->render->DrawRectangle(bounds, 0, 255, 0);
}

bool Wizard::UnLoad()
{
	return true;
}

void Wizard::HandleInput(float dt)
{
	switch (stance)
	{
	case PlayerStance::ROAMING:
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			bounds.y -= SPEED_Y * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			bounds.y += SPEED_Y * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			bounds.x -= SPEED_X * dt;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			bounds.x += SPEED_X * dt;
		}
		break;
	}
}

bool Wizard::SaveState(pugi::xml_node& node)
{
	Player::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("playerType");

	auxiliar1.append_attribute("type").set_value("WIZARD");


	

	return true;
}

