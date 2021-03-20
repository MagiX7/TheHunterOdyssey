#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

class Player;
class GuiButton;
class GuiManager;
class CharacterManager;

enum class PlayerType;

enum class SwapCharState
{
	NONE = 0,
	NORMAL,
};

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

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:
	SDL_Texture* bg;

	Player* player;

	CharacterManager* charManager;

	SwapCharState state;
};

#endif //__SCENEGAMEPLAY_H__