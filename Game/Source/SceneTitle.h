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
	
	//TITLE EASING
	float the_x;
	float hunter_x;
	float odyssey_y;
	float currIt;
	float totalIt;
	bool titleEasingActivated;
	
	//ENTER EASING
	bool enterEasingActivatedY;
	bool enterEasingActivated2Y;
	bool enterEasingActivatedX;
	bool enterEasingActivated2X;
	bool enterEasingActivated3X;
	bool enterEasingActivated4X;
	float text_x;
	float text_y;
	float currItEnterY;
	float totalItEnterY;	
	float currItEnterX;
	float totalItEnterX;

	SDL_Rect section;
	SDL_Rect section2;
	SDL_Rect section3;

	Easing* easing;
};

#endif //__SCENETITLE_H__