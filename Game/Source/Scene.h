#ifndef __SCENE_H__
#define __SCENE_H__

#include "TransitionsManager.h"

#include "Log.h"

enum class SceneType
{
	LOGO,
	MENU,
	TITLE,
	GAMEPLAY,
	ENDING
};

enum class TransitionType;

class Scene
{
public:

	Scene() : active(false), transitionRequired(false), win(false) {}

	// Destructor
	virtual ~Scene() {}

	// Called before render is available
	bool Awake() { return true; }

	// Called before the first frame
	virtual bool Load() { return true; }

	// Called each loop iteration
	virtual bool Update(float dt) { return true; }

	// Called before all Updates
	virtual void Draw() {}

	// Called before quitting
	virtual bool UnLoad() { return true; }

	virtual bool LoadState(pugi::xml_node&) { return true; }

	virtual bool SaveState(pugi::xml_node&) const { return true; }


	void TransitionToScene(SceneType scene, TransitionType type, bool w = false)
	{
		LOG("Changing Scene");
		transitionRequired = true;
		nextScene = scene;
		win = w;
		//transitionType = type;
		TransitionsManager::GetInstance()->SetType(type);
	}

public:
	SString name;
	bool active;
	
	bool transitionRequired;
	SceneType nextScene;
	bool isTown;
	bool isDungeon;
	bool showColliders;

	int channel;
	bool win;
};

#endif // __SCENE_H__