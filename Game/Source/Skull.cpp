#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"
#include "Audio.h"

#include "Skull.h"
#include "Player.h"

Skull::Skull(iPoint pos, pugi::xml_node anim, eastl::string map) : Enemy(EntityType::SKULL)
{
	bounds = { pos.x, pos.y, 74, 74 };
	texture = app->tex->Load("Textures/Enemies/floating_skull.png");
	attackFx = app->audio->LoadFx("Audio/Fx/Battle/skull_attack.wav");
	dieFx = app->audio->LoadFx("Audio/Fx/Battle/skull_die.wav");
	hurtFx = app->audio->LoadFx("Audio/Fx/Battle/skull_hurt.wav");
	name = "Skull";
	mapName = map;
	eType = EnemyType::SKULL;

	battlePos = pos;
	health = 750;
	maxHealth = health;
	mana = 100;
	damage = 200;
	defense = 20;
	speed = 10.0f;
	channel = app->audio->SetChannel();

	attack = false;

	pugi::xml_node skull = anim.child("skull");

	for (pugi::xml_node n = skull.child("attack").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->attackAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}
	this->attackAnim.loop = false;

	for (pugi::xml_node n = skull.child("death").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->deathAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}
	this->deathAnim.loop = false;

	for (pugi::xml_node n = skull.child("idle").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->idleAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}
	this->idleAnim.loop = true;

	currentState = EnemyState::NORMAL;

	currentAnim = &idleAnim;

	font = new Font(app, "Font/font3.xml", app->tex);
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
	currentAnim->speed = 9.0f * dt;
	currentAnim->Update();

	if (currentAnim == &hitAnim && hitAnim.HasFinished())
	{
		idleAnim.Reset();
		currentAnim = &idleAnim;
	}

	switch (currentState)
	{
	case EnemyState::ROAMING:
		currentAnim = &idleAnim;
		break;
	case EnemyState::NORMAL:

		break;
	case EnemyState::ATTACKING:
		if (attack == false && currentAnim == &idleAnim)
		{
			Travel(iPoint(target->bounds.x, target->bounds.y), dt);
			if (bounds.x == target->bounds.x && bounds.y == target->bounds.y)
			{
				startPaarticles(iPoint(target->bounds.x, target->bounds.y), -1);
				//particlesBattle->setGeneratorState(particleNum, GeneratorState::STARTING);
				//particlesBa->SetPosition({ target->bounds.x,target->bounds.y });
				attack = true;
				app->audio->PlayFx(channel, attackFx);
				target->GetDamage(damage);
				attackAnim.Reset();
				currentAnim = &attackAnim;
				idleAnim.Reset();
			}
		}
		else if (attack && attackAnim.HasFinished())
		{
			Travel(iPoint(battlePos.x, battlePos.y), dt);
			currentAnim = &idleAnim;

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

	app->render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
}

bool Skull::UnLoad()
{
	app->tex->UnLoad(texture);
	//app->audio->UnLoadFx(attackFx);
	//app->audio->UnLoadFx(hurtFx);
	//app->audio->UnLoadFx(dieFx);

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
	Enemy::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("EnemyType");
	auxiliar1.append_attribute("type").set_value("SKULL");
	return true;
}

void Skull::GetDamage(int dmg)
{
	health -= dmg * dmg / (dmg + defense);
	if (health > 0) app->audio->PlayFx(channel, hurtFx);
	if (health <= 0)
	{
		health = 0;
		app->audio->PlayFx(channel, dieFx);
		deathAnim.Reset();
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