#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "GuiManager.h"

#include "SceneTitle.h"

#include "Audio.h"

#include "Log.h"

SceneTitle::SceneTitle()
{
	bg = nullptr;

}

bool SceneTitle::Load()
{
	LOG("Loading Scene Title");
	bool ret = true;

	return ret;
}

bool SceneTitle::Update(float dt)
{
	bool ret = true;

	return ret;
}

void SceneTitle::Draw()
{
}

bool SceneTitle::UnLoad()
{
	LOG("Unloading Scene Title");
	bool ret = true;

	return ret;
}

bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON: break;
	case GuiControlType::CHECKBOX: break;
	case GuiControlType::SLIDER: break;
	}
	return true;
}