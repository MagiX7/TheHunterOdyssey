#include "App.h"
#include "Render.h"

#include "KnightHelmet.h"

bool KnightHelmet::Load()
{
	atlasSection = { 96, 225, 32, 32 };

	return true;
}

void KnightHelmet::Draw(bool showColliders)
{
	app->render->DrawTexture(texture, bounds.x, bounds.y, &atlasSection);
}

bool KnightHelmet::SaveState(pugi::xml_node& n)
{
	pugi::xml_node node = n.append_child("position");
	node.append_attribute("x").set_value(bounds.x);
	node.append_attribute("y").set_value(bounds.y);
	node.append_attribute("w").set_value(bounds.w);
	node.append_attribute("h").set_value(bounds.h);

	node = n.append_child("object_type");
	node.append_attribute("value").set_value((int)objectType);

	node = n.append_child("armor_type");
	node.append_attribute("value").set_value((int)armorType);

	node = n.append_child("map_name");
	node.append_attribute("value").set_value(mapName.c_str());

	return true;
}