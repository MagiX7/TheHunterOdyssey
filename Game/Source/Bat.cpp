#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"

#include "Bat.h"
#include "Player.h"


Bat::Bat(iPoint pos) : Enemy(EntityType::BAT)
{
	bounds = { pos.x, pos.y, 37, 33 };
	texture = app->tex->Load("Assets/Textures/Enemies/flying_eye.png");
	name = "Bat";

	battlePos = pos;
	health = 20;
	mana = 80;
	damage = 20;
	defense = 20;
	speed = 10;

	attack = false;

	currentState = EnemyState::NORMAL;

	font = new Font("Assets/Font/font3.xml", app->tex);
}

Bat::~Bat()
{
}

bool Bat::Load()
{


	return true;
}

bool Bat::Update(float dt)
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

bool Bat::CheckCollisions()
{
	return true;
}

void Bat::Draw(bool showColliders)
{
	if (showColliders == true)
	{
		app->render->DrawRectangle(bounds, 0, 0, 255, 255);
	}
		
	SDL_Rect rect = { 4,5,bounds.w,bounds.h };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);

	SDL_Color color = { 255,255,255,255 };
	app->render->DrawText(font, "BAT", bounds.x, bounds.y - 15, 15, 5, color);

	char tmp[32] = { 0 };

	sprintf_s(tmp, 32, "Health: %i", health);
	color = { 0,255,0,255 };
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y, 15, 5, color);

	sprintf_s(tmp, 32, "Mana: %i", mana);
	color = { 0,0,255,255 };
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y + 20, 15, 5, color);
}

bool Bat::UnLoad()
{
	app->tex->UnLoad(texture);
	font->UnLoad(app->tex);

	RELEASE(font);

	return true;
}

bool Bat::LoadState(pugi::xml_node& node)
{
	return true;
}

bool Bat::SaveState(pugi::xml_node& node)
{
	return true;
}

void Bat::GetDamage(int dmg)
{
	health -= dmg * dmg / (dmg + defense);

	if (health <= 0)
	{
		health = 0;
		currentAnim = &deathAnim;
	}
}

void Bat::Attack(Player* player)
{
	if (health > 0)
	{
		currentState = EnemyState::ATTACKING;
		target = player;
	}
}

void Bat::Travel(iPoint destination, float dt)
{
	if (bounds.x < destination.x) bounds.x += 200 * dt;
	if (bounds.x > destination.x) bounds.x -= 100 * dt;
	if (bounds.y > destination.y) bounds.y -= 100 * dt;
	if (bounds.y < destination.y) bounds.y += 100 * dt;
}