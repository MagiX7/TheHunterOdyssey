#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "SceneGameplay.h"

#include "Audio.h"

#include "Log.h"

SceneGameplay::SceneGameplay()
{
	name.Create("scenegameplay");

	bg = nullptr;
}

bool SceneGameplay::Load()
{
	LOG("Loading Scene Gameplay");
	bool ret = true;

	return ret;
}

bool SceneGameplay::Update(float dt)
{
	bool ret = true;

	return ret;
}

void SceneGameplay::Draw()
{
}

bool SceneGameplay::UnLoad()
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	return ret;
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{

	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{

	return true;
}