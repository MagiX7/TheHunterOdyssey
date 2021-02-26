#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "GuiControl.h"

class SceneTitle : public Scene
{
public:
	SceneTitle();

	virtual ~SceneTitle() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

	//TODO
	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	SDL_Texture* bg;
};

#endif //__SCENETITLE_H__