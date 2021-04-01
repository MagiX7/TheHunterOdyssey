#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"

#include "Bat.h"


Bat::Bat(iPoint pos) : Enemy(EntityType::BAT)
{
	bounds = { pos.x, pos.y, 37, 33 };
	texture = app->tex->Load("Assets/Textures/Enemies/flying_eye.png");
	name = "Bat";

	health = 20.0f;
	mana = 80.0f;
	damage = 20.0f;
	defense = 20.0f;
	speed = 10.0f;

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
	return true;
}

bool Bat::CheckCollisions()
{
	return true;
}

void Bat::Draw(bool showColliders)
{
	if (showColliders)
		app->render->DrawRectangle(bounds, 0, 0, 255, 255);

	SDL_Rect rect = { 4,5,bounds.w,bounds.h };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &rect);

	SDL_Color color = { 255,255,255,255 };
	app->render->DrawText(font, "BAT", bounds.x, bounds.y - 15, 15, 5, color);

	char tmp[32] = { 0 };

	sprintf_s(tmp, 32, "Health: %.0f", health);
	color = { 0,255,0,255 };
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y, 15, 5, color);

	sprintf_s(tmp, 32, "Mana: %.0f", mana);
	color = { 0,0,255,255 };
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y + 20, 15, 5, color);
}

bool Bat::UnLoad()
{
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
