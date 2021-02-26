#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "SceneGameplay.h"
#include "EntityManager.h"

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

	entityManager->Update(dt);

	return ret;
}

void SceneGameplay::Draw()
{
	entityManager->Draw();
}

bool SceneGameplay::UnLoad()
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	entityManager->UnLoad();

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