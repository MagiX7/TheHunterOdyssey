#include "App.h"
#include "Textures.h"

#include "SceneMenu.h"
#include "GuiManager.h"
#include "Font.h"

SceneMenu::SceneMenu()
{
}

bool SceneMenu::Load()
{

	return true;
}

bool SceneMenu::Update(float dt)
{

	return true;
}

void SceneMenu::Draw()
{

}

bool SceneMenu::UnLoad()
{

	return true;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) return false;
	}

	return false;
}