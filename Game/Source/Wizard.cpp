#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

#include "Wizard.h"
#include "Enemy.h"

Wizard::Wizard(iPoint position, pugi::xml_node anim, ParticlesManager* particles) : Player(PlayerType::WIZARD, EntityType::HUNTER, position)
{
	//bounds = { 0,0, 16,32 };
	stance = PlayerStance::ROAMING;
	healthPoints = 2000;
	maxHealthPoints = healthPoints;
	armorPoints = 5;
	maxArmorPoints = armorPoints;
	manaPoints = 5000;
	maxManaPoints = manaPoints;
	meleeDamage = 10;
	magicDamage = 50;
	isDefending = false;
	attack = false;
	name = "Wizard";
	Particles = particles;
	isDead = false;
	pugi::xml_node player = anim.child("wizard").child("overworld");

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

	player = anim.child("wizard").child("battlers");

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

	this->abilityName[0] = "Fire ball";
	this->abilityName[1] = "Water ball";
	this->abilityName[2] = "Thunder";
	this->abilityName[3] = "Gravity";

	this->abilityCost[0] = 200;
	this->abilityCost[1] = 150;
	this->abilityCost[2] = 250;
	this->abilityCost[3] = 400;
}

Wizard::~Wizard()
{
}

bool Wizard::Load()
{
	// TODO implement textures for wizard. 
	texture = app->tex->Load("Assets/Textures/Players/wizard2.png");
	battlerTexture = app->tex->Load("Assets/Textures/Players/battler_wizard.png");

	footStepFx = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/footstep_wizard.ogg");
	dieFx = app->audio->LoadFx("Assets/Audio/Fx/Battle/wizard_die.wav");
	attackFx = app->audio->LoadFx("Assets/Audio/Fx/Battle/wizard_attack.wav");
	hurtFx = app->audio->LoadFx("Assets/Audio/Fx/Battle/wizard_hurt.ogg");

	return true;
}

bool Wizard::Update(float dt)
{
	currentAnim->speed = 10.0f * dt;

	switch (stance)
	{
	case PlayerStance::BATTLE:
		if (healthPoints <= 0)
		{
			if (isDead == false)
			{
				app->audio->PlayFx(channel, dieFx);
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
			app->audio->PlayFx(channel, hurtFx);
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
				app->audio->PlayFx(channel, attackFx);
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
		break;
	}

	HandleInput(dt);

	currentAnim->Update();

	return true;
}

void Wizard::Draw(bool showColliders)
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

bool Wizard::UnLoad()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(battlerTexture);
	app->audio->UnLoadFx(footStepFx);
	app->audio->UnLoadFx(attackFx);
	app->audio->UnLoadFx(hurtFx);
	app->audio->UnLoadFx(dieFx);

	return true;
}

void Wizard::HandleInput(float dt)
{
	switch (stance)
	{
	case PlayerStance::ROAMING:
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || app->input->pad->l_y < -0.5)
		{
			app->audio->PlayFx(channel, footStepFx);
			bounds.y -= SPEED_Y * dt;
			if (currentAnim != &walkUp)
			{
				walkUp.Reset();
				currentAnim = &walkUp;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || app->input->pad->l_y > 0.5)
		{
			app->audio->PlayFx(channel, footStepFx);
			bounds.y += SPEED_Y * dt;
			if (currentAnim != &walkDown)
			{
				walkDown.Reset();
				currentAnim = &walkDown;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || app->input->pad->l_x < -0.5)
		{
			app->audio->PlayFx(channel, footStepFx);
			bounds.x -= SPEED_X * dt;
			if (currentAnim != &walkLeft)
			{
				walkLeft.Reset();
				currentAnim = &walkLeft;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || app->input->pad->l_x > 0.5)
		{
			app->audio->PlayFx(channel, footStepFx);
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
	if (healthPoints > 0)
	{
		stance = PlayerStance::ATTACKING;
		target = enemy;
	}
}

void Wizard::GetDamage(int dmg)
{
	if (isDefending == false)
	{
		healthPoints -= dmg * dmg / (dmg + armorPoints);
	}
	else
	{
		healthPoints -= (dmg * dmg / (dmg + armorPoints)) * 0.75;
		isDefending = false;
	}
	damageTaken.Reset();
	currentAnim = &damageTaken;
}

void Wizard::GetHealed(int heal)
{
	healthPoints += heal;
	if (healthPoints > maxHealthPoints) healthPoints = maxHealthPoints;
}

void Wizard::GetMana(int amount)
{
	manaPoints += amount;
	if (manaPoints > maxManaPoints) manaPoints = maxManaPoints;
}

void Wizard::Ability(Enemy* enemy, int currentAbility)
{
	stance = PlayerStance::ABILITY;

	switch (currentAbility)
	{
	case 1:
		GetMana(-this->abilityCost[0]);
		enemy->GetDamage(magicDamage + 10);
		LOG("Casting FIRE");
		break;
	case 2:
		GetMana(-this->abilityCost[1]);
		enemy->GetDamage(magicDamage);
		LOG("Casting WATER");
		break;
	case 3:
		GetMana(-this->abilityCost[2]);
		enemy->GetDamage(magicDamage + 150);
		LOG("Casting THUNDER");
		break;
	case 4:
		GetMana(-this->abilityCost[3]);
		enemy->GetDamage(magicDamage * 10);
		LOG("Casting GRAVITY");
		break;
	}
	stance = PlayerStance::ABILITY_FINISHED;
}

bool Wizard::CanUseAbility(int abilityNum)
{
	if (GetManaPoints() >= this->abilityCost[abilityNum]) return true;
	return false;
}

void Wizard::UseObject(Player* player, int currentObject)
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

void Wizard::SetDefend(bool option)
{
	isDefending = option;
}

void Wizard::Travel(iPoint destination, float dt)
{
	if (bounds.x < destination.x) bounds.x += 200 * dt;
	if (bounds.x > destination.x) bounds.x -= 200 * dt;
	if (bounds.y > destination.y) bounds.y -= 200 * dt;
	if (bounds.y < destination.y) bounds.y += 200 * dt;
}