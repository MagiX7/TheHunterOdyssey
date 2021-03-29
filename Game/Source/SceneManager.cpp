#include "App.h"
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
	onTransition = false;
	fadeOutCompleted = false;
	transitionAlpha = 0.0f;
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

	current = new SceneLogo();
	current->Load();

	next = nullptr;

	return ret;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	OPTICK_EVENT();

	LOG("Updating Current Scene");
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) app->render->camera.y += 5 * dt;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) app->render->camera.y -= 5 * dt;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) app->render->camera.x += 5 * dt;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) app->render->camera.x -= 5 * dt;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) current->showColliders = !current->showColliders;

	if (!onTransition)
	{
		ret = current->Update(dt);
	}
	else
	{
		if (!fadeOutCompleted)
		{
			transitionAlpha += (FADEOUT_TRANSITION_SPEED * dt);

			// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
			// For that reason we compare against 1.01f, to avoid last frame loading stop
			if (transitionAlpha > 1.01f)
			{
				transitionAlpha = 1.0f;

				current->UnLoad();	// Unload current screen
				next->Load();	// Load next screen

				RELEASE(current);	// Free current pointer
				current = next;		// Assign next pointer
				next = nullptr;

				// Activate fade out effect to next loaded screen
				fadeOutCompleted = true;
			}
		}
		else  // Transition fade out logic
		{
			transitionAlpha -= (FADEIN_TRANSITION_SPEED * dt);

			if (transitionAlpha < -0.01f)
			{
				transitionAlpha = 0.0f;
				fadeOutCompleted = false;
				onTransition = false;
			}
		}
	}

	// Draw current scene
	current->Draw();

	// Draw full screen rectangle in front of everything
	if (onTransition)
	{
		app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y, 1280, 720 }, 0, 0, 0, (unsigned char)(255.0f * transitionAlpha));
	}

	if (current->transitionRequired)
	{
		onTransition = true;
		fadeOutCompleted = false;
		transitionAlpha = 0.0f;

		switch (current->nextScene)
		{
		case SceneType::LOGO: next = new SceneLogo(); break;
		case SceneType::TITLE: next = new SceneTitle(); break;
		case SceneType::GAMEPLAY: next = new SceneGameplay(); break;
		case SceneType::ENDING: next = new SceneEnding(); break;
		default: break;
		}
		current->transitionRequired = false;
	}

	// Quit the game
	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;

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
	
	//TODO
	current->LoadState(load.child(current->name.GetString()));

	return ret;
}

bool SceneManager::SaveState(pugi::xml_node& save) const
{
	LOG("Saving Scene Manager");
	bool ret = true;
	
	//TODO
	current->SaveState(save.append_child(current->name.GetString()));

	return ret;
}