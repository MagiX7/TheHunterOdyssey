#pragma once

#include "Scene.h"

class GuiButton;
class GuiManager;
class Font;
class MainMenu;

//enum class MenuState
//{
//	NONE = 0,
//	NORMAL,
//	OPTIONS,
//	CREDITS,
//	EXIT,
//	GLOBAL_EXIT,
//};

class SceneMenu : public Scene
{
public:
	SceneMenu();

	virtual ~SceneMenu() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

private:
	SDL_Texture* bg;

	//MenuState state;

	MainMenu* mainMenu;
	
	// Buttons
	//GuiButton* btnNewGame;
	//GuiButton* btnContinue;
	//GuiButton* btnOptions;
	//GuiButton* btnCredits;
	//GuiButton* btnExit;
	//GuiButton* btnExitYes;
	//GuiButton* btnExitNo;
	//GuiButton* btnOptionsBack;
	//GuiButton* btnCreditsBack;

	// The user wants to exit the game
	//bool isExitPressed;
};