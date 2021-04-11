#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"

class MainMenu;
class Easing;

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
	
	int enterFx;
	int titleFx;

	TitleState state;

	MainMenu* mainMenu;

	Font* font;

	float titleAlpha;
	float position_x;
	float currIt;
	bool easingActivated;

	Easing* easing;
};

#endif //__SCENETITLE_H__