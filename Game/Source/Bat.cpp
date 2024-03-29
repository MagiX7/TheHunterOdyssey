#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"
#include "Audio.h"
#include "Bat.h"
#include "Player.h"


Bat::Bat(iPoint pos, pugi::xml_node anim, eastl::string map) : Enemy(EntityType::BAT)
{
	bounds = { pos.x, pos.y, 37, 33 };
	texture = app->tex->Load("Textures/Enemies/bat.png");
	attackFx = app->audio->LoadFx("Audio/Fx/Battle/bat_attack.wav");
	dieFx = app->audio->LoadFx("Audio/Fx/Battle/bat_die.wav");
	hurtFx = app->audio->LoadFx("Audio/Fx/Battle/bat_hurt.wav");
	name = "Bat";
	mapName = map;
	eType = EnemyType::BAT;

	battlePos = pos;
	health = 600;
	maxHealth = health;
	mana = 80;
	damage = 400;
	defense = 20;
	speed = 10;
	channel = app->audio->SetChannel();

	attack = false;

	pugi::xml_node bat = anim.child("bat");

	for (pugi::xml_node n = bat.child("attack").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->attackAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}
	this->attackAnim.loop = false;

	for (pugi::xml_node n = bat.child("flight").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->flightAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = bat.child("death").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->deathAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = bat.child("hit").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->hitAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	currentState = EnemyState::NORMAL;
	currentAnim = &flightAnim;

	font = new Font(app,"Font/font3.xml", app->tex);
}

Bat::~Bat()
{
}
bool Bat::SaveState(pugi::xml_node& node)
{
	Enemy::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("EnemyType");
	auxiliar1.append_attribute("type").set_value("BAT");
	return true;
}
bool Bat::Load()
{
	return true;
}

bool Bat::Update(float dt)
{
	currentAnim->speed = 5.0f * dt;
	currentAnim->Update();

	if (currentAnim == &hitAnim && hitAnim.HasFinished())
	{
		flightAnim.Reset();
		currentAnim = &flightAnim;
	}

	switch (currentState)
	{
	case EnemyState::ROAMING:
		currentAnim = &flightAnim;
		break;
	case EnemyState::NORMAL:

		break;
	case EnemyState::ATTACKING:
		if (attack == false && currentAnim == &flightAnim)
		{
			Travel(iPoint(target->bounds.x, target->bounds.y), dt);
			if (bounds.x < (target->bounds.x + (target->bounds.w / 2)) && bounds.y == target->bounds.y)
			{
				startPaarticles(iPoint(target->bounds.x, target->bounds.y),-1);
				//set generator state(start) 
				//particlesBattle->setGeneratorState(particleNum, GeneratorState::STARTING);
				//particlesBa->SetPosition({ target->bounds.x,target->bounds.y });
				//set geneartor position
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
			currentAnim = &flightAnim;
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
		app->render->DrawRectangle(bounds, 0, 0, 255, 255);
		
	app->render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
}

bool Bat::UnLoad()
{
	app->tex->UnLoad(texture);
	//app->audio->UnLoadFx(attackFx);
	//app->audio->UnLoadFx(hurtFx);
	//app->audio->UnLoadFx(dieFx);
	font->UnLoad(app->tex);

	RELEASE(font);

	return true;
}

bool Bat::LoadState(pugi::xml_node& node)
{
	return true;
}

void Bat::GetDamage(int dmg)
{
	health -= dmg * dmg / (dmg + defense);

	if (health <= 0)
	{
		health = 0;
		deathAnim.Reset();
		app->audio->PlayFx(channel, dieFx);
		currentAnim = &deathAnim;
	}
	else
	{
		app->audio->PlayFx(channel, hurtFx);
		hitAnim.Reset();
		currentAnim = &hitAnim;
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