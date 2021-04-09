#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Hunter.h"
#include "Enemy.h"

Hunter::Hunter(iPoint position, pugi::xml_node anim) : Player(PlayerType::HUNTER, EntityType::HUNTER, position)
{
	//bounds = { 0,0, 16,32 };
	stance = PlayerStance::ROAMING;
	healthPoints = 2500;
	manaPoints = 1000;
	defense = 15;
	meleeDamage = 75;
	magicDamage = 10;
	isDefending = false;
	name = "Hunter";

	pugi::xml_node player = anim.child("hunter").child("overworld");

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
	texture = app->tex->Load("Assets/Textures/Players/hunter2.png");
}

Hunter::~Hunter()
{
}

bool Hunter::Load()
{

	return true;
}

bool Hunter::Update(float dt)
{
	currentAnim->speed = 10.0f * dt;

	HandleInput(dt);

	currentAnim->Update();

	return true;
}

void Hunter::Draw(bool showColliders)
{
	if (showColliders) app->render->DrawRectangle(bounds, 255, 0, 0);
	app->render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
}

bool Hunter::UnLoad()
{
	return true;
}

void Hunter::HandleInput(float dt)
{
	switch (stance)
	{
	case PlayerStance::ROAMING:
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			bounds.y -= SPEED_Y * dt;
			if (currentAnim != &walkUp)
			{
				walkUp.Reset();
				currentAnim = &walkUp;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			bounds.y += SPEED_Y * dt;
			if (currentAnim != &walkDown)
			{
				walkDown.Reset();
				currentAnim = &walkDown;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			bounds.x -= SPEED_X * dt;
			if (currentAnim != &walkLeft)
			{
				walkLeft.Reset();
				currentAnim = &walkLeft;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
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

bool Hunter::SaveState(pugi::xml_node& node)
{
	Player::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("playerType");

	auxiliar1.append_attribute("type").set_value("HUNTER");
	return true;
}

void Hunter::Attack(Enemy* enemy)
{
	enemy->GetDamage(meleeDamage);
}

void Hunter::GetDamage(int dmg)
{
	if (isDefending == false)
	{
		healthPoints -= dmg * dmg / (dmg + defense);
	}
	else
	{
		healthPoints -= (dmg * dmg / (dmg + defense)) * 0.99;
	}
}

void Hunter::Ability(Enemy* enemy, int currentAbility)
{
	switch (currentAbility)
	{
	case 1:
		enemy->GetDamage(magicDamage + 10);
		LOG("Casting STEADY SHOT");
		break;
	case 2:
		enemy->GetDamage(magicDamage);
		LOG("Casting SERPENT SHOT");
		break;
	case 3:
		enemy->GetDamage(magicDamage + meleeDamage + 50);
		LOG("Casting EXPLOSIVE SHOT");
		break;
	case 4:
		enemy->GetDamage(magicDamage);
		LOG("Casting BOLA");
		break;
	}
}

void Hunter::UseObject(Player* player, int currentObject)
{
	switch (currentObject)
	{
	case 1:
		// TODO Object functionality
		player->GetHealed(500);
		LOG("USING ULTRA POTION");
		break;
	case 2:
		// TODO Object functionality
		break;
	case 3:
		// TODO Object functionality
		break;
	}
}

void Hunter::GetHealed(int heal)
{
	healthPoints += heal;
}

void Hunter::SetDefend(bool option)
{
	isDefending = option;
}
