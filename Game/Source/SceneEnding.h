#ifndef __SCENEENDING_H__
#define __SCENEENDING_H__

#include "Scene.h"

class SceneEnding : public Scene
{
public:
	SceneEnding();

	virtual ~SceneEnding() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

private:
	SDL_Texture* bg;
};

#endif //__SCENEENDING_H__