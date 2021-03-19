#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "SceneLogo.h"
#include "Audio.h"

#include "Log.h"

#define LOGO_FADE_IN_SPEED 0.4f
#define LOGO_FADE_OUT_SPEED 1.0f

SceneLogo::SceneLogo()
{
	logo = nullptr;

	state = 0;
	timeCounter = 0.0f;
	logoAlpha = 1.0f;
	drawCounter = 0.75f;

	showColliders = false;
}

bool SceneLogo::Load()
{
	LOG("Loading Scene Logo");
	bool ret = true;
	logoFx = app->audio->LoadFx("Assets/Audio/Fx/logo_intro.wav");
	logo = app->tex->Load("Assets/Textures/Scenes/logo.png");

	return ret;
}

bool SceneLogo::Update(float dt)
{
	bool ret = true;

	switch (state)
	{
	case 0:
		state = 1;
		break;
	case 1:

		drawCounter -= dt;

		if (drawCounter <= 0)
		{
			app->audio->PlayFx(logoFx);
			logoAlpha -= (LOGO_FADE_IN_SPEED * dt);
		}
		if (logoAlpha < 0.0f)
		{
			logoAlpha = 0.0f;
			state = 2;
		}
		break;
	case 2:
		timeCounter += dt;
		if (timeCounter >= 1.5f) state = 3;
		break;
	case 3:
		logoAlpha += (LOGO_FADE_OUT_SPEED * dt);

		if (logoAlpha > 1.0f)
		{
			logoAlpha = 1.0f;
			TransitionToScene(SceneType::TITLE);
		}
		break;
	}

	return ret;
}

void SceneLogo::Draw()
{
	if (drawCounter <= 0)
	{
		drawCounter = 0;
		app->render->DrawRectangle({ 0,0,1280,720 }, 255, 255, 255, 255);
		app->render->DrawTexture(logo, 315, 35, NULL);
	}

	// Fade in Logo
	if (state == 1) app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 255 * logoAlpha);
}

bool SceneLogo::UnLoad()
{
	LOG("Unloading Scene Logo");
	bool ret = true;
	app->tex->UnLoad(logo);

	return ret;
}