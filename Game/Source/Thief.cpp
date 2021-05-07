#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

#include "Thief.h"
#include "Enemy.h"

Thief::Thief(iPoint position, pugi::xml_node anim, ParticlesManager* particles) : Player(PlayerType::THIEF, EntityType::THIEF,position)
{
	stance = PlayerStance::ROAMING;
	healthPoints = 1500;
	maxHealthPoints = healthPoints;
	defense = 10;
	manaPoints = 500;
	maxManaPoints = manaPoints;
	meleeDamage = 50;
	magicDamage = 15;
	isDefending = false;
	attack = false;
	name = "Thief";
	Particles = particles;
	isDead = false;
	pugi::xml_node player = anim.child("thief").child("overworld");

	for (pugi::xml_node n = player.child("walk_front").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->walkDown.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("walk_left").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->walkLeft.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("walk_right").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->walkRight.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("walk_up").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->walkUp.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	player = anim.child("thief").child("battlers");

	for (pugi::xml_node n = player.child("idle").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->idleBattle.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("death").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->death.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("right_arm_swinging").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->attackAnim.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	for (pugi::xml_node n = player.child("damage_taken").child("pushback"); n; n = n.next_sibling("pushback"))
	{
		this->damageTaken.PushBack({ n.attribute("x").as_int(), n.attribute("y").as_int(), n.attribute("w").as_int(), n.attribute("h").as_int() });
	}

	idleDown.PushBack(walkDown.frames[0]);
	idleLeft.PushBack(walkLeft.frames[0]);
	idleRight.PushBack(walkRight.frames[0]);
	idleUp.PushBack(walkUp.frames[0]);

	currentAnim = &idleDown;
}

Thief::~Thief()
{
}

bool Thief::Load()
{
	texture = app->tex->Load("Assets/Textures/Players/thief_overworld.png");
	battlerTexture = app->tex->Load("Assets/Textures/Players/battler_thief.png");

	footStepFx = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/footstep_thief.ogg");
	dieFx = app->audio->LoadFx("Assets/Audio/Fx/Battle/thief_die.wav");
	attackFx = app->audio->LoadFx("Assets/Audio/Fx/Battle/thief_attack.wav");
	hurtFx = app->audio->LoadFx("Assets/Audio/Fx/Battle/thief_hurt.ogg");

	return true;
}

bool Thief::Update(float dt)
{
	currentAnim->speed = 10.0f * dt;

	switch (stance)
	{
	case PlayerStance::BATTLE:
		if (healthPoints <= 0)
		{
			if (isDead == false)
			{
				app->audio->PlayFx(dieFx);
				isDead = true;
			}
			healthPoints = 0;
			currentAnim = &death;
		}
		else if (currentAnim == &death && healthPoints > 0)
		{
			idleBattle.Reset();
			currentAnim = &idleBattle;
		}
		if (currentAnim == &damageTaken && damageTaken.HasFinished())
		{
			app->audio->PlayFx(hurtFx);
			idleBattle.Reset();
			currentAnim = &idleBattle;
		}
		break;
	case PlayerStance::ATTACKING:
		if (attack == false && currentAnim == &idleBattle)
		{
			Travel(iPoint(target->bounds.x, target->bounds.y), dt);

			if (bounds.x == target->bounds.x && bounds.y == target->bounds.y)
			{
				app->audio->PlayFx(attackFx);
				attack = true;
				attackAnim.Reset();
				currentAnim = &attackAnim;
				target->GetDamage(meleeDamage);
			}
		}
		else if (attack && attackAnim.HasFinished())
		{
			currentAnim = &idleBattle;
			Travel(iPoint(battlePos.x, battlePos.y), dt);

			if (bounds.x == battlePos.x && bounds.y == battlePos.y)
			{
				stance = PlayerStance::ATTACK_FINISHED;
				attack = false;
			}
		}
	}

	HandleInput(dt);

	currentAnim->Update();

	return true;
}

void Thief::Draw(bool showColliders)
{
	SDL_Rect r = bounds;
	r.x += 2;
	r.y += 3;
	if (showColliders) app->render->DrawRectangle(r, 255, 0, 0, 150);
	if (stance == PlayerStance::ROAMING) app->render->DrawTexture(texture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
	else if (stance == PlayerStance::BATTLE || stance == PlayerStance::ATTACKING ||
		stance == PlayerStance::ATTACK_FINISHED || stance == PlayerStance::ABILITY_FINISHED)
	{
		app->render->DrawTexture(battlerTexture, bounds.x, bounds.y, &currentAnim->GetCurrentFrame());
	}
}

bool Thief::UnLoad()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(battlerTexture);
	app->audio->UnLoadFx(footStepFx);
	app->audio->UnLoadFx(attackFx);
	app->audio->UnLoadFx(hurtFx);
	app->audio->UnLoadFx(dieFx);

	return true;
}

void Thief::HandleInput(float dt)
{
	switch (stance)
	{
	case PlayerStance::ROAMING:
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || app->input->pad->l_y < -0.5)
		{
			if (app->frameCount % 15 == 0) app->audio->PlayFx(footStepFx);
			bounds.y -= SPEED_Y * dt;
			if (currentAnim != &walkUp)
			{
				walkUp.Reset();
				currentAnim = &walkUp;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || app->input->pad->l_y > 0.5)
		{
			if (app->frameCount % 15 == 0) app->audio->PlayFx(footStepFx);
			bounds.y += SPEED_Y * dt;
			if (currentAnim != &walkDown)
			{
				walkDown.Reset();
				currentAnim = &walkDown;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || app->input->pad->l_x < -0.5)
		{
			if (app->frameCount % 15 == 0) app->audio->PlayFx(footStepFx);
			bounds.x -= SPEED_X * dt;
			if (currentAnim != &walkLeft)
			{
				walkLeft.Reset();
				currentAnim = &walkLeft;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || app->input->pad->l_x > 0.5)
		{
			if (app->frameCount % 15 == 0) app->audio->PlayFx(footStepFx);
			bounds.x += SPEED_X * dt;
			if (currentAnim != &walkRight)
			{
				walkRight.Reset();
				currentAnim = &walkRight;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE &&
			app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE &&
			app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_IDLE &&
			app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_IDLE &&
			app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_IDLE &&
			app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_IDLE &&
			app->input->pad->l_y > -0.5 && app->input->pad->l_y < 0.5 &&
			app->input->pad->l_x > -0.5 && app->input->pad->l_x < 0.5)
		{
			if (currentAnim == &walkDown) currentAnim = &idleDown;
			else if (currentAnim == &walkLeft) currentAnim = &idleLeft;
			else if (currentAnim == &walkRight) currentAnim = &idleRight;
			else if (currentAnim == &walkUp) currentAnim = &idleUp;
		}
		break;
	}
}

bool Thief::SaveState(pugi::xml_node& node)
{
	Player::SaveState(node);
	pugi::xml_node auxiliar1;
	auxiliar1 = node.append_child("playerType");

	auxiliar1.append_attribute("type").set_value("THIEF");
	return true;
}

void Thief::Attack(Enemy* enemy)
{
	if (healthPoints > 0)
	{
		stance = PlayerStance::ATTACKING;
		target = enemy;
	}
}

void Thief::GetDamage(int dmg)
{
	if (isDefending == false)
	{
		healthPoints -= dmg * dmg / (dmg + defense);
	}
	else
	{
		healthPoints -= (dmg * dmg / (dmg + defense)) * 0.5;
		isDefending = false;
	}
	damageTaken.Reset();
	currentAnim = &damageTaken;
}

void Thief::GetHealed(int heal)
{
	healthPoints += heal;
	if (healthPoints > maxHealthPoints) healthPoints = maxHealthPoints;
}

void Thief::GetMana(int amount)
{
	manaPoints += amount;
	if (manaPoints > maxManaPoints) manaPoints = maxManaPoints;
}

void Thief::Ability(Enemy* enemy, int currentAbility)
{
	stance = PlayerStance::ABILITY;

	switch (currentAbility)
	{
	case 1:
		enemy->GetDamage(magicDamage + 1);
		LOG("Casting STEAL");
		break;
	case 2:
		enemy->GetDamage(magicDamage);
		LOG("Casting BOMB");
		break;
	case 3:
		enemy->GetDamage(magicDamage + rand() % 200);
		LOG("Casting USE STEALED");
		break;
	case 4:
		enemy->GetDamage(magicDamage + 200);
		LOG("Casting LUCKY");
		break;
	}
	stance = PlayerStance::ABILITY_FINISHED;
}

void Thief::UseObject(Player* player, int currentObject)
{
	switch (currentObject)
	{
	case 1:
		// TODO Object functionality
		player->GetHealed(200);
		LOG("USING POTION");
		break;
	case 2:
		// TODO Object functionality
		player->GetHealed(1000);
		LOG("USING ULTRA POTION");
		break;
	case 3:
		// TODO Object functionality
		player->GetMana(500);
		LOG("USING ETER");
		break;
	case 4:
		player->GetMana(1500);
		LOG("USING ULTRA ETER");
		break;
	}
}

void Thief::SetDefend(bool option)
{
	isDefending = option;
}

void Thief::Travel(iPoint destination, float dt)
{
	if (bounds.x < destination.x) bounds.x += 200 * dt;
	if (bounds.x > destination.x) bounds.x -= 200 * dt;
	if (bounds.y > destination.y) bounds.y -= 200 * dt;
	if (bounds.y < destination.y) bounds.y += 200 * dt;
}

