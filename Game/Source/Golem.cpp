#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"

#include "Golem.h"
#include "Player.h"


Golem::Golem(iPoint pos) : Enemy(EntityType::GOLEM)
{
	bounds = { pos.x, pos.y, 49, 47 };
	texture = app->tex->Load("Assets/Textures/Enemies/golem.png");
	name = "Golem";
	
	health = 10;
	mana = 50;
	damage = 1000;
	defense = 20;
	speed = 10;

	font = new Font("Assets/Font/font3.xml", app->tex);
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
	app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);

	SDL_Color color = { 255,255,255,255 };
	app->render->DrawText(font, "GOLEM", bounds.x, bounds.y - 15, 15, 5, color);

	char tmp[32] = { 0 };

	sprintf_s(tmp, 32, "Health: %i", health);
	color = { 0,255,0,255 };
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y, 15, 5, color);

	sprintf_s(tmp, 32, "Mana: %i", mana);
	color = { 0,0,255,255 };
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y + 20, 15, 5, color);
}

bool Golem::UnLoad()
{
	app->tex->UnLoad(texture);
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
	return true;
}

void Golem::GetDamage(int dmg)
{
	health -= dmg * dmg / (dmg + defense);

	if (health <= 0)
	{
		health = 0;
	}
}

void Golem::Attack(Player* player)
{
	player->GetDamage(damage);
}
