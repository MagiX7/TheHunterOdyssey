#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"

#include "Skull.h"


Skull::Skull(iPoint pos) : Enemy(EntityType::SKULL)
{
	bounds = { pos.x, pos.y, 29, 29 };
	texture = app->tex->Load("Assets/Textures/Enemies/floating_skull.png");
	name = "Skull";

	health = 50.0f;
	mana = 100.0f;
	damage = 20.0f;
	defense = 20.0f;
	speed = 10.0f;

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

	sprintf_s(tmp, 32, "Health: %.0f", health);
	color = { 0,255,0,255 };
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y, 15, 5, color);

	sprintf_s(tmp, 32, "Mana: %.0f", mana);
	color = {0,0,255,255};
	app->render->DrawText(font, tmp, bounds.x + bounds.w + 5, bounds.y + 20, 15, 5, color);


}

bool Skull::UnLoad()
{
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
