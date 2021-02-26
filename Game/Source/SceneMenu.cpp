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
	guiManager = new GuiManager();

	return true;
}

bool SceneMenu::Update(float dt)
{
	guiManager->Update(dt);

	return true;
}

void SceneMenu::Draw()
{
	guiManager->Draw();
}

bool SceneMenu::UnLoad()
{
	guiManager->CleanUp();

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