#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "SceneEnding.h"
#include "Audio.h"

#include "Log.h"

SceneEnding::SceneEnding()
{
	bg = nullptr;

	showColliders = false;
}

bool SceneEnding::Load()
{
	LOG("Loading Scene Ending");
	bool ret = true;

	return ret;
}

bool SceneEnding::Update(float dt)
{
	bool ret = true;

	return ret;
}

void SceneEnding::Draw()
{
}

bool SceneEnding::UnLoad()
{
	LOG("Unloading Scene Ending");
	bool ret = true;

	return ret;
}