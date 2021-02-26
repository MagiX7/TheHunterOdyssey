#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

class EntityManager;

class SceneGameplay : public Scene
{
public:
	SceneGameplay();

	virtual ~SceneGameplay() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

private:
	SDL_Texture* bg;

	EntityManager* entityManager;
};

#endif //__SCENEGAMEPLAY_H__