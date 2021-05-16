#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

#include "SceneTitle.h"
#include "Easings.h"
#include "MainMenu.h"

#include "Log.h"

#define TITLE_FADE_SPEED 1.0f

SceneTitle::SceneTitle()
{
	bg = nullptr;
	state = TitleState::TITLE;
	showColliders = false;

	currIt = 0;
	totalIt = 180;
	titleEasingActivated = true;

	currItEnterY = 0;
	totalItEnterY = 120;
	enterEasingActivatedY = true;

	currItEnterX = 0;
	totalItEnterX = 60;
	enterEasingActivatedX = true;

	text_x = 420;
	text_y = 540;

	the_x = -200;
	hunter_x = 1480;
	odyssey_y = 920;

	section = { 0, 0, 161, 116 };
	section2 = {174, 0, 349, 123};
	section3 = {87, 123, 348, 100};

	titleAlpha = 0.0f;
}

bool SceneTitle::Load()
{
	LOG("Loading Scene Title");
	bool ret = true;
	
	// Logo texture loading
	bgText = app->tex->Load("Textures/Scenes/battle_bg2.png");
	bg = app->tex->Load("Textures/Scenes/game_title.png");

	// Music startup
	// TODO revise this when we change the module audio.
	app->audio->PlayMusic("Audio/Music/intro_theme.ogg");

	// Enter FX
	enterFx = app->audio->LoadFx("Audio/Fx/title_enter.wav");

	// Title FX
	//titleFx = app->audio->LoadFx("Assets/Audio/Fx/hello_man.wav");

	font = new Font(app,"Font/font3.xml", app->tex);
	
	mainMenu = new MainMenu(this);
	mainMenu->Load(font);

	channel = app->audio->SetChannel();

	if (app->render->camera.x != 0) app->render->camera.x = 0;
	if (app->render->camera.y != 0) app->render->camera.y = 0;

	return ret;
}

bool SceneTitle::Update(float dt)
{
	bool ret = true;

	switch (state)
	{
	case TitleState::NONE:
		//app->audio->PlayFx(titleFx);
		state = TitleState::TITLE;
		break;
	case TitleState::TITLE:
		//TITLE
		if (titleEasingActivated)
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
				titleEasingActivated = false;
				currIt = 0;
			}
		}
		//ENTER Y
		if (enterEasingActivatedY)
		{
			text_y = easing->sineEaseInOut(currItEnterY, 540, 15, totalItEnterY);
			
			if (currItEnterY < totalItEnterY)
			{
				currItEnterY++;
			}
			else
			{
				enterEasingActivatedY = false;
				enterEasingActivated2Y = true;
				currItEnterY = 0;
			}
		}
		if (enterEasingActivated2Y)
		{
			text_y = easing->sineEaseInOut(currItEnterY, 555, -15, totalItEnterY);

			if (currItEnterY < totalItEnterY)
			{
				currItEnterY++;
			}
			else
			{
				enterEasingActivatedY = true;
				enterEasingActivated2Y = false;
				currItEnterY = 0;
			}
		}
		//ENTER X
		if (enterEasingActivatedX)
		{
			text_x = easing->sineEaseOut(currItEnterX, 420, 10, totalItEnterX);

			if (currItEnterX < totalItEnterX)
			{
				currItEnterX++;
			}
			else
			{
				enterEasingActivatedX = false;
				enterEasingActivated2X = true;
				enterEasingActivated3X = false;
				enterEasingActivated4X = false;
				currItEnterX = 0;
			}
		}
		if (enterEasingActivated2X)
		{
			text_x = easing->sineEaseIn(currItEnterX, 430, -10, totalItEnterX);

			if (currItEnterX < totalItEnterX)
			{
				currItEnterX++;
			}
			else
			{
				enterEasingActivatedX = false;
				enterEasingActivated2X = false;
				enterEasingActivated3X = true;
				enterEasingActivated4X = false;
				currItEnterX = 0;
			}
		}
		if (enterEasingActivated3X)
		{
			text_x = easing->sineEaseOut(currItEnterX, 420, -10, totalItEnterX);

			if (currItEnterX < totalItEnterX)
			{
				currItEnterX++;
			}
			else
			{
				enterEasingActivatedX = false;
				enterEasingActivated2X = false;
				enterEasingActivated3X = false;
				enterEasingActivated4X = true;
				currItEnterX = 0;
			}
		}
		if (enterEasingActivated4X)
		{
			text_x = easing->sineEaseIn(currItEnterX, 410, 10, totalItEnterX);

			if (currItEnterX < totalItEnterX)
			{
				currItEnterX++;
			}
			else
			{
				enterEasingActivatedX = true;
				enterEasingActivated2X = false;
				enterEasingActivated3X = false;
				enterEasingActivated4X = false;
				currItEnterX = 0;
			}
		}

		/*titleAlpha -= (TITLE_FADE_SPEED * dt);
		if (titleAlpha <= 0.0f)
		{
			titleAlpha = 0.0f;
		}*/

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pad->GetButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A) == KEY_DOWN || app->input->pad->GetButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START) == KEY_DOWN)
		{
			app->audio->PlayFx(channel, enterFx);
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
		app->render->DrawText(font, "Press ENTER to continue", text_x, text_y, 36, 5, { 255,255,255,255 });
		//app->render->DrawText(font, "Press ENTER to continue", 450, 516, 36, 5, { 255,255,255 });
		app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 255 * titleAlpha);
		break;
	case TitleState::FADE:
		app->render->DrawTexture(bgText, 0, 0);
		app->render->DrawTexture(bg, 378, 230, NULL);
		app->render->DrawCenterText(font, "Press ENTER to continue", { 0, 400, 1280, 320 }, 36, 5, { 255,255,255,255 });
		//app->render->DrawText(font, "Press ENTER to continue", 450, 516, 36, 5, { 255,255,255 });
		app->render->DrawRectangle({ 0,0,1280,720 }, 0, 0, 0, 255 * titleAlpha);
		break;
	case TitleState::MENU:
		app->render->DrawTexture(bgText, 0, 0);
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
	RELEASE(font);

	mainMenu->UnLoad();
	RELEASE(mainMenu);

	app->tex->UnLoad(bg);
	app->tex->UnLoad(bgText);
	//app->audio->UnLoadFx(enterFx);
	//app->audio->UnLoadFx(titleFx);
	app->audio->UnLoadFxs();

	return ret;
}