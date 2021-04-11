#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

class MainMenu;
class Easing;
class SDL_Rect;

enum class TitleState
{
	NONE = 0,
	TITLE,
	FADE,
	MENU
};

class SceneTitle : public Scene
{
public:
	SceneTitle();

	virtual ~SceneTitle() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

private:
	SDL_Texture* bg;
	SDL_Texture* bgText;
	
	int enterFx;
	int titleFx;

	TitleState state;

	MainMenu* mainMenu;

	Font* font;

	float titleAlpha;
	float the_x;
	float hunter_x;
	float odyssey_y;
	float currIt;
	float totalIt;
	bool easingActivated;

	SDL_Rect section;
	SDL_Rect section2;
	SDL_Rect section3;

	Easing* easing;
};

#endif //__SCENETITLE_H__