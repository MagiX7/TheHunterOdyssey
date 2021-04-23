#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"

class Easing;

class SceneLogo : public Scene
{
public:
	SceneLogo();

	virtual ~SceneLogo() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

private:
	SDL_Texture* logo;
	Easing* easing1;
	Easing* easing2;
	float logoPositionX;
	float logoPositionX2;
	
	int state;
	float timeCounter;
	float logoAlpha;
	int logoFx;

	float drawCounter;
};

#endif //__SCENELOGO_H__