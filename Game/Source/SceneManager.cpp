#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"

#include "SceneManager.h"
#include "Scene.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGameplay.h"
#include "SceneEnding.h"

#include "Defs.h"
#include "Log.h"

#include "Optick/include/optick.h"

#define FADEOUT_TRANSITION_SPEED	2.0f
#define FADEIN_TRANSITION_SPEED		2.0f

SceneManager::SceneManager() : Module()
{
	name.Create("scenemanager");
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene Manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
	LOG("Scene Manager Start");
	bool ret = true;

	current = new SceneGameplay();
	current->Load();

	next = nullptr;

	// Transition rects
	// Wipe
	uint w, h;
	app->win->GetWindowSize(w, h);
	rectWipe = { 0,0,0,(int)h };

	// Alternating bars
	for (int i = 0; i < MAX_BARS_SIZE; ++i)
	{
		bars[i].h = h / MAX_BARS_SIZE;
		bars[i].y = i * bars[i].h;
		if (i % 2 == 0)
		{
			bars[i].w = 0;
			bars[i].x = 0;
		}
		else
		{
			bars[i].w = 0;
			bars[i].x = (int)w;
		}
	}

	// Half Height Rectangles
	rectUpper = { 0,0,0,(int)h / 2 };
	rectLower = { (int)w,(int)h / 2,0,(int)h / 2 };

	// Fade to Black
	transitionAlpha = 0.0f;

	// Half Width Rectangles
	rectUpper2 = { 0,0,(int)w, 0 };
	rectLower2 = { 0,(int)h,(int)w,0 };
	halfWidthCount = 0.0f;

	return ret;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	OPTICK_EVENT();

	//LOG("Updating Current Scene");
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_BACK)== KEY_DOWN) current->showColliders = !current->showColliders;

	if (transitionStep == TransitionStep::NONE)
	{
		ret = current->Update(dt);
	}
	else
	{
		uint w, h;
		app->win->GetWindowSize(w, h);
		switch (transitionStep)
		{
		case TransitionStep::ENTERING:
			if (current->transitionType == TransitionType::WIPE)
			{
				rectWipe.w += 1000 * dt;
				
				if (rectWipe.w >= w) transitionStep = TransitionStep::CHANGING;
			}
			else if (current->transitionType == TransitionType::ALTERNATING_BARS)
			{
				for (int i = 0; i < MAX_BARS_SIZE; ++i)
				{
					if (i % 2 == 0) bars[i].w += 1000 * dt;
					else bars[i].w -= 1000 * dt;
				}

				if (bars[MAX_BARS_SIZE - 1].w < -(int)w) transitionStep = TransitionStep::CHANGING;
			}
			else if (current->transitionType == TransitionType::HALF_HEIGHT_RECTANGLES)
			{
				if (rectUpper.w <= (int)w + 100)
					rectUpper.w += 1000 * dt;
				else
				{
					rectLower.w -= 1000 * dt;
					if(rectLower.w <= -(int)w) transitionStep = TransitionStep::CHANGING;
				}
			}
			else if (current->transitionType == TransitionType::FADE_TO_BLACK)
			{
				transitionAlpha += dt;
				if(transitionAlpha > 1.01f) transitionStep = TransitionStep::CHANGING;
			}
			else if (current->transitionType == TransitionType::HALF_WIDHT_RECTANGLES)
			{
				rectUpper2.h += 500 * dt;
				rectLower2.h -= 500 * dt;
				if (rectLower2.h <= -(int)h / 2) transitionStep = TransitionStep::CHANGING;
			}
			break;

		case TransitionStep::CHANGING:
			current->UnLoad();
			if (app->audio->FadeOutCompleted() == false)
			{
				TransitionType tmpEnteringType = current->transitionType;
				next->Load();
				RELEASE(current);
				current = next;
				next = nullptr;
				current->transitionType = tmpEnteringType;
				transitionStep = TransitionStep::EXITING;
			}
			break;

		case TransitionStep::EXITING:
			if (current->transitionType == TransitionType::WIPE)
			{
				rectWipe.w -= 1000 * dt;
				LOG("%i", rectWipe.w);

				if (rectWipe.w < 0) transitionStep = TransitionStep::NONE;
			}
			else if (current->transitionType == TransitionType::ALTERNATING_BARS)
			{
				for (int i = 0; i < MAX_BARS_SIZE; ++i)
				{
					if (i % 2 == 0) bars[i].w -= 1000 * dt;
					else bars[i].w += 1000 * dt;
				}

				if (bars[MAX_BARS_SIZE - 1].w > 0) transitionStep = TransitionStep::NONE;
			}
			else if (current->transitionType == TransitionType::HALF_HEIGHT_RECTANGLES)
			{
				if (rectLower.w <= 0)
					rectLower.w += 1000 * dt;
				else
				{
					rectUpper.w -= 1000 * dt;
					if(rectUpper.w <= 0) transitionStep = TransitionStep::NONE;
				}
			}
			else if (current->transitionType == TransitionType::FADE_TO_BLACK)
			{
				transitionAlpha -= dt;
				if (transitionAlpha < -0.01f) transitionStep = TransitionStep::NONE;
			}
			else if (current->transitionType == TransitionType::HALF_WIDHT_RECTANGLES)
			{
				halfWidthCount += dt;
				if (halfWidthCount >= 0.5f)
				{
					rectUpper2.h -= 500 * dt;
					rectLower2.h += 500 * dt;
					if (rectLower2.h >= 0) transitionStep = TransitionStep::NONE;
				}
			}
			break;
		}
	}

	// Draw current scene
	current->Draw();

	// Draw full screen rectangle in front of everything

	if(transitionStep != TransitionStep::NONE)
	{
		switch (current->transitionType)
		{
		case TransitionType::WIPE:
			app->render->DrawRectangle(rectWipe, 0, 0, 0);
			break;

		case TransitionType::ALTERNATING_BARS:
			for (int i = 0; i < MAX_BARS_SIZE; ++i) app->render->DrawRectangle(bars[i], 0, 0, 0);
			break;

		case TransitionType::HALF_HEIGHT_RECTANGLES:
			app->render->DrawRectangle(rectUpper, 0, 0, 0);
			app->render->DrawRectangle(rectLower, 0, 0, 0);
			break;

		case TransitionType::FADE_TO_BLACK:
			app->render->DrawRectangle({ 0, 0, 1280,720 }, 0, 0, 0, (unsigned char)(255.0f * transitionAlpha));
			break;

		case TransitionType::HALF_WIDHT_RECTANGLES:
			app->render->DrawRectangle(rectUpper2, 0, 0, 0);
			app->render->DrawRectangle(rectLower2, 0, 0, 0);
			break;
		}
	}

	if (current->transitionRequired)
	{
		transitionStep = TransitionStep::ENTERING;

		switch (current->nextScene)
		{
		case SceneType::LOGO:
			next = new SceneLogo(); break;
		case SceneType::TITLE:
			next = new SceneTitle(); break;
		case SceneType::GAMEPLAY:
			next = new SceneGameplay(); break;
		case SceneType::ENDING:
			next = new SceneEnding(true); break;
		default: break;
		}
		current->transitionRequired = false;
	}

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing Scene Manager");
	bool ret = true;

	return ret;
}

bool SceneManager::LoadState(pugi::xml_node& load)
{
	LOG("Loading Scene Manager");
	bool ret = true;

	//current->UnLoad();
	//RELEASE(current);
	//SString type;
	//type = load.child("Type").attribute("sceneType").as_string();
	//if (type == "scenegameplay") current = new SceneGameplay();
	//current->Load();

	if (next != nullptr) next->LoadState(load.child(next->name.GetString()));
	else current->LoadState(load.child(current->name.GetString()));

	return ret;
}

bool SceneManager::SaveState(pugi::xml_node& save) const
{
	LOG("Saving Scene Manager");
	bool ret = true;
	//const pugi::char_t* name = current->name.GetString();
	////pugi::xml_node toSaveScene = toSaveEntites.append_child("scene");
	//save.append_child("Type").append_attribute("sceneType").set_value(name);
	////TODO
	current->SaveState(save.append_child(current->name.GetString()));

	return ret;
}