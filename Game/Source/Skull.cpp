#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"

#include "Skull.h"
#include "Player.h"


Skull::Skull(iPoint pos, pugi::xml_node anim) : Enemy(EntityType::SKULL)
{
	bounds = { pos.x, pos.y, 29, 29 };
	texture = app->tex->Load("Assets/Textures/Enemies/floating_skull.png");
	name = "Skull";

	battlePos = pos;
	health = 50;
	mana = 100;
	damage = 20;
	defense = 20;
	speed = 10.0f;

	attack = false;
	currentState = EnemyState::NORMAL;

	font = new Font("Assets/Font/font3.xml", app->tex);
}

Skull::~Skull()
{
}

bool Skull::Load()
{

	return true;
}

bool Skull::Update(float dt)
{
	switch (currentState)
	{
	case EnemyState::NORMAL:

		break;
	case EnemyState::ATTACKING:
		if (attack == false)
		{
			Travel(iPoint(target->bounds.x, target->bounds.y), dt);
			if (bounds.x == target->bounds.x && bounds.y == target->bounds.y)
			{
				attack = true;
				target->GetDamage(damage);
			}
		}
		else
		{
			Travel(iPoint(battlePos.x, battlePos.y), dt);
			if (bounds.x == battlePos.x && bounds.y == battlePos.y)
			{
				currentState = EnemyState::ATTACK_FINISHED;
				attack = false;
			}
		}
		break;
	case EnemyState::ATTACK_FINISHED:

		break;
	}

	return true;
}

bool Skull::CheckCollisions()
{
	return true;
}

void Skull::Draw(bool showColliders)
{
	if (showColliders)
		app->render->DrawRectangle(bounds, 0, 0, 255, 255);

	SDL_Rect rect = { 5,7,bounds.w,bounds.h };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);

	SDL_Color color = { 255,255,255,255 };
	app->render->DrawText(font, "SKULL", bounds.x, bounds.y - 15, 15, 5, color);

	char tmp[32] = { 0 };

	sprintf_s(tmp, 32, "Health: %d", health);
	color = { 0,255,0,255 };
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y, 15, 5, color);

	sprintf_s(tmp, 32, "Mana: %d", mana);
	color = {0,0,255,255};
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y + 20, 15, 5, color);
}

bool Skull::UnLoad()
{
	app->tex->UnLoad(texture);
	
	font->UnLoad(app->tex);
	RELEASE(font);

	return true;
}

bool Skull::LoadState(pugi::xml_node& node)
{
	return true;
}

bool Skull::SaveState(pugi::xml_node& node)
{
	return true;
}

void Skull::GetDamage(int dmg)
{
	health -= dmg * dmg / (dmg + defense);

	if (health <= 0)
	{
		health = 0;
		currentAnim = &deathAnim;
	}
}

void Skull::Attack(Player* player)
{
	if (health > 0)
	{
		currentState = EnemyState::ATTACKING;
		target = player;
	}
}

void Skull::Travel(iPoint destination, float dt)
{
	if (bounds.x < destination.x) bounds.x += 200 * dt;
	if (bounds.x > destination.x) bounds.x -= 100 * dt;
	if (bounds.y > destination.y) bounds.y -= 100 * dt;
	if (bounds.y < destination.y) bounds.y += 100 * dt;
}