#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiButton.h"

#include "Wizard.h"
#include "Enemy.h"

Wizard::Wizard(iPoint position, pugi::xml_node anim) : Player(PlayerType::WIZARD, EntityType::HUNTER, position)
{
	//bounds = { 0,0, 16,32 };
	stance = PlayerStance::ROAMING;
	healthPoints = 1000;
	defense = 5;
	manaPoints = 5000;
	meleeDamage = 1;
	magicDamage = 5;
	isDefending = false;
	name = "Wizard";

	pugi::xml_node player = anim.child("wizard").child("overworld");

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
	texture = app->tex->Load("Assets/textures/Players/wizard2.png");
}

Wizard::~Wizard()
{
}

bool Wizard::Load()
{
	// TODO implement textures for wizard. 
	return true;
}

bool Wizard::Update(float dt)
{
	currentAnim->speed = 10.0f * dt;

	GamePad& pad = app->input->pads[0];
	pad.enabled = true;
	HandleInput(dt, pad);

	currentAnim->Update();

	return true;
}

void Wizard::Draw(bool showColliders)
{
	//if (showColliders) app->render->DrawRectangle(bounds, 0, 255, 0);
	app->render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
}

bool Wizard::UnLoad()
{
	return true;
}

void Wizard::HandleInput(float dt, GamePad& pad)
{
	switch (stance)
	{
	case PlayerStance::ROAMING:
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || pad.up == true)
		{
			bounds.y -= SPEED_Y * dt;
			if (currentAnim != &walkUp)
			{
				walkUp.Reset();
				currentAnim = &walkUp;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || pad.down == true)
		{
			bounds.y += SPEED_Y * dt;
			if (currentAnim != &walkDown)
			{
				walkDown.Reset();
				currentAnim = &walkDown;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || pad.left == true)
		{
			bounds.x -= SPEED_X * dt;
			if (currentAnim != &walkLeft)
			{
				walkLeft.Reset();
				currentAnim = &walkLeft;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || pad.right == true)
		{
			bounds.x += SPEED_X * dt;
			if (currentAnim != &walkRight)
			{
				walkRight.Reset();
				currentAnim = &walkRight;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE &&
			app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		{
			if (currentAnim == &walkDown) currentAnim = &idleDown;
			else if (currentAnim == &walkLeft) currentAnim = &idleLeft;
			else if (currentAnim == &walkRight) currentAnim = &idleRight;
			else if (currentAnim == &walkUp) currentAnim = &idleUp;
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

void Wizard::Attack(Enemy* enemy)
{
	enemy->GetDamage(meleeDamage);
}

void Wizard::GetDamage(int dmg)
{
	if (isDefending == false)
	{
		healthPoints -= dmg * dmg / (dmg + defense);
	}
	else
	{
		healthPoints -= (dmg * dmg / (dmg + defense)) * 0.75;
		isDefending = false;
	}
}

void Wizard::Ability(Enemy* enemy, int currentAbility)
{
	switch (currentAbility)
	{
	case 1:
		enemy->GetDamage(magicDamage + 10);
		LOG("Casting FIRE");
		break;
	case 2:
		enemy->GetDamage(magicDamage);
		LOG("Casting WATER");
		break;
	case 3:
		enemy->GetDamage(magicDamage);
		LOG("Casting THUNDER");
		break;
	case 4:
		enemy->GetDamage(magicDamage);
		LOG("Casting GRAVITY");
		break;
	}
}

void Wizard::UseObject(Player* player, int currentObject)
{
	switch (currentObject)
	{
	case 1:
		// TODO Object functionality
		break;
	case 2:
		// TODO Object functionality
		break;
	case 3:
		// TODO Object functionality
		break;
	}
}

void Wizard::SetDefend(bool option)
{
	isDefending = option;
}
