#include "App.h"
#include "Input.h"
#include "Render.h"

#include "SceneGameplay.h"

#include "PauseMenu.h"
#include "Textures.h"

PauseMenu::PauseMenu(SceneGameplay* s) : scene(s)
{
}

PauseMenu::~PauseMenu()
{
}

bool PauseMenu::Load()
{
	guiTex = app->tex->Load("Assets/Textures/UI/gui_pause_menu.png");

	btnResume = new GuiButton(1, { 505, 223, 270, 64 }, "Resume Game", this);
	btnResume->section = { 430,0,270,64 };
	btnResume->texture = guiTex;
	btnLoadSave = new GuiButton(2, { 505, 308, 270, 64 }, "Load/Save", this);
	btnLoadSave->section = { 430,0,270,64 };
	btnLoadSave->texture = guiTex;
	btnSave = new GuiButton(6, { 505, 256, 270, 64 }, "Save", this);
	btnSave->section = { 430,0,270,64 };
	btnSave->texture = guiTex;
	btnLoad = new GuiButton(7, { 505, 341, 270, 64 }, "Load", this);
	btnLoad->section = { 430,0,270,64 };
	btnLoad->texture = guiTex;
	btnBack = new GuiButton(8, { 505, 528, 270, 64 }, "Back", this);
	btnBack->section = { 430,0,270,64 };
	btnBack->texture = guiTex;
	btnOptions = new GuiButton(3, { 505, 393, 270, 64 }, "Options", this);
	btnOptions->section = { 430,0,270,64 };
	btnOptions->texture = guiTex;
	btnReturnTitle = new GuiButton(4, { 505, 478, 270, 64 }, "Return to Title", this);
	btnReturnTitle->section = { 430,0,270,64 };
	btnReturnTitle->texture = guiTex;
	btnExit = new GuiButton(5, { 505, 563, 270, 64 }, "Exit Game", this);
	btnExit->section = { 430,0,270,64 };
	btnExit->texture = guiTex;

	font = new Font("Assets/Font/font3.xml", app->tex);
	
	state = PauseState::DEFAULT;

	return true;
}

bool PauseMenu::Update(float dt)
{
	bool ret = true;
	
	switch (state)
	{
	case PauseState::DEFAULT:
		btnResume->Update(app->input, dt);
		btnLoadSave->Update(app->input, dt);
		btnOptions->Update(app->input, dt);
		btnReturnTitle->Update(app->input, dt);
		ret = btnExit->Update(app->input, dt);
		break;
	case PauseState::OPTIONS:
		break;
	case PauseState::SAVE:
		btnSave->Update(app->input, dt);
		btnLoad->Update(app->input, dt);
		btnBack->Update(app->input, dt);
		break;
	}

	return ret;
}

void PauseMenu::Draw(bool showColliders)
{
	SDL_Rect section;

	switch (state)
	{
	case PauseState::DEFAULT:
		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, 425, 48, &section);

		btnResume->Draw(app->render, showColliders);
		btnLoadSave->Draw(app->render, showColliders);
		btnOptions->Draw(app->render, showColliders);
		btnReturnTitle->Draw(app->render, showColliders);
		btnExit->Draw(app->render, showColliders);

		app->render->DrawText(font, "PAUSE", 552, 121, 72, 5, { 255,255,255,255 });
		app->render->DrawText(font, btnResume->text.GetString(), 529, 239, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnLoadSave->text.GetString(), 549, 324, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnOptions->text.GetString(), 578, 409, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnReturnTitle->text.GetString(), 515, 494, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnExit->text.GetString(), 559, 579, 36, 5, { 0,0,0,255 });
		break;
	case PauseState::OPTIONS:
		break;
	case PauseState::SAVE:
		section = { 0,0,430,650 };
		app->render->DrawTexture(guiTex, 425, 48, &section);

		btnSave->Draw(app->render, showColliders);
		btnLoad->Draw(app->render, showColliders);
		btnBack->Draw(app->render, showColliders);

		app->render->DrawText(font, "PAUSE", 552, 121, 72, 5, { 255,255,255,255 });
		app->render->DrawText(font, btnSave->text.GetString(), 601, 272, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnLoad->text.GetString(), 601, 357, 36, 5, { 0,0,0,255 });
		app->render->DrawText(font, btnBack->text.GetString(), 602, 544, 36, 5, { 0,0,0,255 });
		break;
	}
}

bool PauseMenu::UnLoad()
{
	RELEASE(btnResume);
	RELEASE(btnLoadSave);
	RELEASE(btnSave);
	RELEASE(btnLoad);
	RELEASE(btnBack);
	RELEASE(btnOptions);
	RELEASE(btnReturnTitle);
	RELEASE(btnExit);

	return true;
}

bool PauseMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
		if (control->id == 1) scene->ChangeState(GameplayMenuState::NONE);
		else if (control->id == 2) state = PauseState::SAVE;
		else if (control->id == 3) state = PauseState::OPTIONS;
		else if (control->id == 4) scene->TransitionToScene(SceneType::TITLE);
		else if (control->id == 5) return false;
		else if (control->id == 6); // Save Game
		else if (control->id == 7); // Load Game
		else if (control->id == 8) state = PauseState::DEFAULT;
	}
	
	return true;
}
