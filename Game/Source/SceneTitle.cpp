#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "MainMenu.h"
#include "SceneTitle.h"

#include "Audio.h"

#include "Log.h"

#define TITLE_FADE_SPEED 1.0f

SceneTitle::SceneTitle()
{
	bg = nullptr;
	state = TitleState::NONE;
	showColliders = true;

	titleAlpha = 1.0f;
}

bool SceneTitle::Load()
{
	LOG("Loading Scene Title");
	bool ret = true;
	
	// Logo texture loading
	bg = app->tex->Load("Assets/Textures/Scenes/game_title.png");

	// Music startup
	// TODO revise this when we change the module audio.
	app->audio->PlayMusic("Assets/Audio/Music/intro_theme.ogg");

	// Enter FX
	enterFx = app->audio->LoadFx("Assets/Audio/Fx/title_enter.wav");

	// Title FX
	titleFx = app->audio->LoadFx("Assets/Audio/Fx/hello_man.wav");

	mainMenu = new MainMenu(this);
	mainMenu->Load();

	return ret;
}

bool SceneTitle::Update(float dt)
{
	bool ret = true;

	switch (state)
	{
	case TitleState::NONE:
		app->audio->PlayFx(titleFx);
		state = TitleState::TITLE;
		break;
	case TitleState::TITLE:
		titleAlpha -= (TITLE_FADE_SPEED * dt);
		if (titleAlpha <= 0.0f)
		{
			titleAlpha = 0.0f;
		}

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			app->audio->PlayFx(enterFx);
			state = TitleState::FADE;
		}
		break;
	case TitleState::FADE:
		titleAlpha += (TITLE_FADE_SPEED * dt);

		if (titleAlpha >= 1.0f)
		{
			titleAlpha = 1.0f;
			state = TitleState::MENU;
		}
		break;
	case TitleState::MENU:
		titleAlpha -= (TITLE_FADE_SPEED * dt);

		if (titleAlpha <= 0.0f)
		{
			titleAlpha = 0.0f;
		}
		ret = mainMenu->Update(dt);
		break;
	}

	return ret;
}

void SceneTitle::Draw()
{
	switch (state)
	{
	case TitleState::TITLE:
		app->render->DrawTexture(bg, 378, 257, NULL);
		app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 255 * titleAlpha);
		break;
	case TitleState::FADE:
		app->render->DrawTexture(bg, 378, 257, NULL);
		app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 255 * titleAlpha);
		break;
	case TitleState::MENU:
		mainMenu->Draw(showColliders);
		app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 255 * titleAlpha);
		break;
	}
}

bool SceneTitle::UnLoad()
{
	LOG("Unloading Scene Title");
	bool ret = true;

	app->tex->UnLoad(bg);
	
	return ret;
}