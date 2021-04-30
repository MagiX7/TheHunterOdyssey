#include "App.h"
#include "Render.h"
#include "MurderQuest.h"

MurderQuest::MurderQuest(pugi::xml_node n) : Quest()
{
	this->id = n.attribute("id").as_int();
	this->name = n.attribute("name").as_string();
	this->requiredId = n.attribute("requiredId").as_int();
	this->isCompleted = n.attribute("isCompleted").as_bool();
	this->type = (EntityType)n.attribute("enemy").as_int();
	this->total = n.attribute("total").as_int();
	this->actual = n.attribute("actual").as_int();
}

MurderQuest::~MurderQuest()
{
}

bool MurderQuest::Update(EntityType t, SString n)
{
	if (type == t) isCompleted = true;

	return isCompleted;
}

void MurderQuest::Draw(Font* font)
{
	app->render->DrawText(font, name.c_str(), 10, 10, 30, 4, { 255, 255, 255 });
}