#ifndef __SCENE_H__
#define __SCENE_H__

#include "Log.h"

struct SDL_Texture;
class GuiControl;

enum class SceneType
{
	LOGO,
	MENU,
	TITLE,
	GAMEPLAY,
	ENDING
};

class Scene
{
public:

	Scene() : active(false), transitionRequired(false) {}

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

	void TransitionToScene(SceneType scene)
	{
		LOG("Changing Scene");
		transitionRequired = true;
		nextScene = scene;
	}

public:
	SString name;
	bool active;

	bool transitionRequired;
	SceneType nextScene;

	bool showColliders;
};

#endif // __SCENE_H__