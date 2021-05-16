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

#include "TransitionsManager.h"

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

	current = new SceneLogo();
	current->Load();

	next = nullptr;

	transitionStep = TransitionStep::NONE;

	return ret;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	OPTICK_EVENT();

	//LOG("Updating Current Scene");
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER)== KEY_DOWN) current->showColliders = !current->showColliders;

	if (transitionStep == TransitionStep::NONE)
	{
		ret = current->Update(dt);
	}
	else
	{
		uint w, h;
		app->win->GetWindowSize(w, h);
		switch (TransitionsManager::GetInstance()->GetStep())
		{
		case TransitionStep::ENTERING:
			/*transitionStep = */TransitionsManager::GetInstance()->EnteringTransition(dt);
			break;

		case TransitionStep::CHANGING:
			if (app->audio->FadeOutCompleted() == false)
			{
				//TransitionType tmpEnteringType = current->transitionType;
				current->UnLoad();
				next->Load();
				RELEASE(current);
				current = next;
				next = nullptr;
				//current->transitionType = tmpEnteringType;
				//transitionStep = TransitionStep::EXITING;
				TransitionsManager::GetInstance()->SetStep(TransitionStep::EXITING);
			}
			break;
		case TransitionStep::EXITING:
			transitionStep = TransitionsManager::GetInstance()->ExitingTransition(dt);
			break;
		}
	}

	// Draw current scene
	current->Draw();

	// Draw the current transition in front of everything
	if(transitionStep != TransitionStep::NONE)
	{
		TransitionsManager::GetInstance()->Draw();
	}

	if (current->transitionRequired)
	{
		TransitionsManager::GetInstance()->SetStep(TransitionStep::ENTERING);
		transitionStep = TransitionsManager::GetInstance()->GetStep();
		switch (current->nextScene)
		{
		case SceneType::LOGO: next = new SceneLogo(); break;
		case SceneType::TITLE: next = new SceneTitle(); break;
		case SceneType::GAMEPLAY: next = new SceneGameplay(); break;
		case SceneType::ENDING: next = new SceneEnding(current->win); break;
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

	if (next != nullptr) next->LoadState(load.child(next->name.GetString()));
	else current->LoadState(load.child(current->name.GetString()));

	return ret;
}

bool SceneManager::SaveState(pugi::xml_node& save) const
{
	LOG("Saving Scene Manager");
	bool ret = true;

	current->SaveState(save.append_child(current->name.GetString()));

	return ret;
}