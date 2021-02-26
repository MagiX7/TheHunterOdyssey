#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"

struct SDL_Texture;
class SceneMenu;

class SceneManager : public Module
{
public:

	SceneManager();

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);

	bool SaveState(pugi::xml_node&) const;

private:
	Scene* current;
	Scene* next;
	SceneMenu* menu;

	bool onTransition;
	bool fadeOutCompleted;
	float transitionAlpha;
};

#endif // __SCENE_H__