#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Easings.h"

#include "MainMenu.h"
#include "SceneTitle.h"

#include "Audio.h"

#include "Log.h"

#define TITLE_FADE_SPEED 1.0f

SceneTitle::SceneTitle()
{
	bg = nullptr;
	state = TitleState::NONE;
	showColliders = false;

	currIt = 0;
	totalIt = 180;
	easingActivated = true;

	section = { 0, 0, 161, 116 };
	section2 = {174, 0, 349, 123};
	section3 = {87, 123, 348, 100};

	titleAlpha = 1.0f;
}

bool SceneTitle::Load()
{
	LOG("Loading Scene Title");
	bool ret = true;
	
	// Logo texture loading
	bgText = app->tex->Load("Assets/Textures/Scenes/battle_bg.png");
	bg = app->tex->Load("Assets/Textures/Scenes/game_title.png");

	// Music startup
	// TODO revise this when we change the module audio.
	app->audio->PlayMusic("Assets/Audio/Music/intro_theme.ogg");

	// Enter FX
	enterFx = app->audio->LoadFx("Assets/Audio/Fx/title_enter.wav");

	// Title FX
	titleFx = app->audio->LoadFx("Assets/Audio/Fx/hello_man.wav");

	font = new Font("Assets/Font/font3.xml", app->tex);
	
	mainMenu = new MainMenu(this);
	mainMenu->Load(font);

	if (app->render->camera.x != 0)
		app->render->camera.x = 0;
	if (app->render->camera.y != 0)
		app->render->camera.y = 0;


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

		if (easingActivated)
		{
			the_x = easing->elasticEaseOut(currIt, 0, 378, totalIt);
			hunter_x = easing->elasticEaseOut(currIt, 1280, -728, totalIt);
			odyssey_y = easing->elasticEaseOut(currIt, 720, -367, totalIt);
			
			if (currIt < totalIt)
			{
				currIt++;
			}
			else
			{
				easingActivated = false;
				currIt = 0;
			}
		}

		titleAlpha -= (TITLE_FADE_SPEED * dt);
		if (titleAlpha <= 0.0f)
		{
			titleAlpha = 0.0f;
		}

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pads->a)
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
		app->render->DrawTexture(bgText, 0, 0);

		app->render->DrawTexture(bg, the_x, 230, &section);
		app->render->DrawTexture(bg, hunter_x, 230, &section2);
		app->render->DrawTexture(bg, 467, odyssey_y, &section3);
		app->render->DrawCenterText(font, "Press ENTER to continue", { 0, 400, 1280, 320 }, 36, 5, { 255,255,255,255 });
		//app->render->DrawText(font, "Press ENTER to continue", 450, 516, 36, 5, { 255,255,255 });
		app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 255 * titleAlpha);
		break;
	case TitleState::FADE:
		app->render->DrawTexture(bg, 378, 230, NULL);
		app->render->DrawCenterText(font, "Press ENTER to continue", { 0, 400, 1280, 320 }, 36, 5, { 255,255,255,255 });
		//app->render->DrawText(font, "Press ENTER to continue", 450, 516, 36, 5, { 255,255,255 });
		app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 255 * titleAlpha);
		break;
	case TitleState::MENU:
		mainMenu->Draw(font, showColliders);
		app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 255 * titleAlpha);
		break;
	}
}

bool SceneTitle::UnLoad()
{
	LOG("Unloading Scene Title");
	bool ret = true;

	font->UnLoad(app->tex);
	app->tex->UnLoad(bg);
	
	RELEASE(font);

	return ret;
}