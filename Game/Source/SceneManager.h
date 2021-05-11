#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"

#define MAX_BARS_SIZE 20

struct SDL_Texture;

enum class TransitionStep
{
	NONE = 0,
	ENTERING,
	CHANGING,
	EXITING
};

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

	TransitionStep transitionStep;
	// Transitions rects
	// Wipe
	SDL_Rect rectWipe;

	// Alternating bars
	SDL_Rect bars[MAX_BARS_SIZE];

	// Half Height Recangles
	SDL_Rect rectUpper;
	SDL_Rect rectLower;

	// Fade to Black
	float transitionAlpha;

	// Half Width Rectangles
	SDL_Rect rectUpper2;
	SDL_Rect rectLower2;
	float halfWidthCount;

};

#endif // __SCENE_H__