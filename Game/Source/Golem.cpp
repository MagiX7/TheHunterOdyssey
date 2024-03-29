#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"
#include "Audio.h"

#include "Golem.h"
#include "Player.h"


Golem::Golem(iPoint pos, pugi::xml_node anim, eastl::string map) : Enemy(EntityType::GOLEM)
{
	bounds = { pos.x, pos.y, 70, 69 };
	texture = app->tex->Load("Textures/Enemies/golem.png");
	attackFx = app->audio->LoadFx("Audio/Fx/Battle/golem_attack.wav");
	dieFx = app->audio->LoadFx("Audio/Fx/Battle/golem_die.wav");
	hurtFx = app->audio->LoadFx("Audio/Fx/Battle/golem_hurt.wav");
	name = "Golem";
	mapName = map;
	eType = EnemyType::GOLEM;
	
	battlePos = pos;
	health = 1000;
	maxHealth = health;
	mana = 50;
	damage = 1000;
	defense = 20;
	speed = 10;
	channel = app->audio->SetChannel();

	pugi::xml_node golem = anim.child("golem");

	for (pugi::xml_node n = golem.child("idle").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->idleAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = golem.child("attack").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->attackAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = golem.child("death").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->deathAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = golem.child("hit").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->hitAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	attack = false;

	currentState = EnemyState::NORMAL;
	currentAnim = &idleAnim;

	font = new Font(app, "Font/font3.xml", app->tex);
}

Golem::~Golem()
{
}

bool Golem::Load()
{
	return true;
}

bool Golem::Update(float dt)
{
	currentAnim->speed = 5.0f * dt;
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
			if (bounds.x <= target->bounds.x + 5 && bounds.y <= target->bounds.y + 5)
			{
				startPaarticles(iPoint(target->bounds.x, target->bounds.y), -1);
				//particlesBattle->setGeneratorState(particleNum, GeneratorState::STARTING);
				//particlesBa->SetPosition({ target->bounds.x,target->bounds.y });
				attack = true;
				app->audio->PlayFx(channel, attackFx);
				idleAnim.Reset();
				attackAnim.Reset();
				currentAnim = &attackAnim;
				target->GetDamage(damage);
			}
		}
		else if (attack && attackAnim.HasFinished())
		{
			currentAnim = &idleAnim;
			Travel(iPoint(battlePos.x, battlePos.y), dt);
			if (bounds.x == battlePos.x && bounds.y == battlePos.y)
			{
				currentState = EnemyState::ATTACK_FINISHED;
				idleAnim.Reset();
				currentAnim = &idleAnim;
				attack = false;
			}
		}
		break;
	case EnemyState::ATTACK_FINISHED:

		break;
	}

	return true;
}

bool Golem::CheckCollisions()
{
	return true;
}

void Golem::Draw(bool showColliders)
{
	if (showColliders) 	
		app->render->DrawRectangle(bounds, 0, 0, 255, 255);

	SDL_Rect rect = { 2,3,bounds.w,bounds.h };
	app->render->DrawTexture(texture, bounds.x, bounds.y + 5, &currentAnim->GetCurrentFrame());
}

bool Golem::UnLoad()
{
	app->tex->UnLoad(texture);
	//app->audio->UnLoadFx(attackFx);
	//app->audio->UnLoadFx(hurtFx);
	//app->audio->UnLoadFx(dieFx);

	font->UnLoad(app->tex);
	RELEASE(font);

	return true;
}

bool Golem::LoadState(pugi::xml_node& node)
{
	return true;
}

bool Golem::SaveState(pugi::xml_node& node)
{
	Enemy::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("EnemyType");
	auxiliar1.append_attribute("type").set_value("GOLEM");
	return true;
}

void Golem::GetDamage(int dmg)
{
	health -= dmg * dmg / (dmg + defense);

	if (health <= 0)
	{
		health = 0;
		app->audio->PlayFx(channel, dieFx);
		deathAnim.Reset();
		currentAnim = &deathAnim;
	}
	else
	{
		app->audio->PlayFx(channel, hurtFx);
		hitAnim.Reset();
		currentAnim = &hitAnim;
	}
}

void Golem::Attack(Player* player)
{
	if (health > 0)
	{
		currentState = EnemyState::ATTACKING;
		target = player;
	}
}

void Golem::Travel(iPoint destination, float dt)
{
	if (bounds.x < destination.x) bounds.x += 200 * dt;
	if (bounds.x > destination.x) bounds.x -= 100 * dt;
	if (bounds.y > destination.y) bounds.y -= 100 * dt;
	if (bounds.y < destination.y) bounds.y += 100 * dt;
}