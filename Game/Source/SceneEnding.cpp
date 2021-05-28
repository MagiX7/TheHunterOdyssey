#include "App.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Font.h"
#include "Easings.h"

#include "SceneEnding.h"
#include "EndingMenu.h"

#include "Log.h"

SceneEnding::SceneEnding(bool win)
{
	bg = nullptr;
	font = nullptr;
	hasWin = win;

	if (hasWin) app->audio->PlayMusic("Audio/Music/win_theme.ogg", true);
	else app->audio->PlayMusic("Audio/Music/gameover_theme.ogg", true);

	easingUp = new Easing();
	easingDown = new Easing();

	easingUp->easingsActivated = true;
	easingUp->currentIteration = 0;
	easingUp->totalIterations = 75;
	easingUp->initialPos = 300;
	easingUp->deltaPos = 50;

	easingDown->easingsActivated = false;
	easingDown->currentIteration = 0;
	easingDown->totalIterations = 75;
	easingDown->initialPos = 350;
	easingDown->deltaPos = -50;
	
	easingPosition = 300;

	showColliders = false;

	// Font
	font = new Font(app, "Font/font3.xml", app->tex);

	menu = new EndingMenu(font, this);
}

bool SceneEnding::Load()
{
	LOG("Loading Scene Ending");
	bool ret = true;

	if (hasWin) bg = app->tex->Load("Textures/Scenes/battle_bg3.png");
	else bg = app->tex->Load("Textures/Scenes/battle_bg2.png");

	return ret;
}

bool SceneEnding::Update(float dt)
{
	bool ret = true;

	if (easingUp->easingsActivated)
	{
		easingPosition = easingUp->exponentialEaseInOut(easingUp->currentIteration, easingUp->initialPos, easingUp->deltaPos, easingUp->totalIterations);
		if (easingUp->currentIteration < easingUp->totalIterations)
		{
			easingUp->currentIteration++;
		}
		else
		{
			easingUp->currentIteration = 0;
			easingUp->easingsActivated = false;
			easingDown->easingsActivated = true;
		}
	}

	if (easingDown->easingsActivated)
	{
		easingPosition = easingDown->exponentialEaseInOut(easingDown->currentIteration, easingDown->initialPos, easingDown->deltaPos, easingDown->totalIterations);
		if (easingDown->currentIteration < easingDown->totalIterations)
		{
			easingDown->currentIteration++;
		}
		else
		{
			easingDown->currentIteration = 0;
			easingDown->easingsActivated = false;
			easingUp->easingsActivated = true;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		TransitionToScene(SceneType::TITLE, TransitionType::HALF_WIDHT_RECTANGLES);

	menu->Update(dt);

	return ret;
}

void SceneEnding::Draw()
{
	app->render->DrawTexture(bg, 0, 0);

	SDL_Color color = { 255,255,255,255 };

	if (hasWin) app->render->DrawText(font, "YOU WIN", 350, easingPosition, 150, 10, color);
	else app->render->DrawText(font, "YOU LOSE", 350, easingPosition, 150, 10, color);

	menu->Draw(font, showColliders);
}

bool SceneEnding::UnLoad()
{
	LOG("Unloading Scene Ending");
	bool ret = true;

	app->tex->UnLoad(bg);
	font->UnLoad(app->tex);

	RELEASE(font);
	RELEASE(easingUp);
	RELEASE(easingDown);

	return ret;
}