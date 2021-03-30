#include "App.h"
#include "Input.h"
#include "Render.h"

#include "SceneGameplay.h"

#include "PauseMenu.h"

PauseMenu::PauseMenu(SceneGameplay* s) : scene(s)
{
}

PauseMenu::~PauseMenu()
{
}

bool PauseMenu::Load()
{
	btnResume = new GuiButton(1, { 466, 150, 360, 64 }, "Resume", this);
	btnReturnTitle = new GuiButton(2, { 466, 250, 360, 64 }, "Return to Title", this);
	btnOptions = new GuiButton(3, { 466, 350, 360, 64 }, "Options", this);
	btnExit = new GuiButton(4, { 466, 450, 360, 64 }, "Exit", this);
	
	return true;
}

bool PauseMenu::Update(float dt)
{
	bool ret = true;

	switch (state)
	{
	case PauseState::DEFAULT:
		btnResume->Update(app->input, dt);
		btnReturnTitle->Update(app->input, dt);
		btnOptions->Update(app->input, dt);
		ret = btnExit->Update(app->input, dt);
		break;
	case PauseState::OPTIONS:
		break;
	}
	
	return ret;
}

void PauseMenu::Draw(bool showColliders)
{
	switch (state)
	{
	case PauseState::DEFAULT:
		btnResume->Draw(app->render, showColliders);
		btnReturnTitle->Draw(app->render, showColliders);
		btnOptions->Draw(app->render, showColliders);
		btnExit->Draw(app->render, showColliders);
		break;
	case PauseState::OPTIONS:
		break;
	}
}

bool PauseMenu::UnLoad()
{
	RELEASE(btnResume);
	RELEASE(btnReturnTitle);
	RELEASE(btnOptions);
	RELEASE(btnExit);

	return true;
}

bool PauseMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) scene->ChangeState(GameplayMenuState::NONE);
		else if (control->id == 2) scene->TransitionToScene(SceneType::TITLE);
		else if (control->id == 3) state = PauseState::OPTIONS;
		else if (control->id == 4) return false;
	}
	
	return true;
}
