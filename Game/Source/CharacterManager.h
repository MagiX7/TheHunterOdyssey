#pragma once

#include "Menu.h"
#include "Font.h"

class Player;
class SceneGameplay;
class GuiButton;
enum class PlayerType;

class CharacterManager : public Menu
{
public:
	CharacterManager(SceneGameplay* s, PlayerType type, Font* font);
	virtual ~CharacterManager();

	bool Load(Font* font) override;

	bool Update(float dt) override;

	void Draw(Font* font, bool showColliders);

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

	void ChangeButtonState(PlayerType type);

	//void HandleInput();

	void UpdatingButtons(Input* input) override;

private:
	SceneGameplay* scene;

	SDL_Texture* guiTex;

	GuiButton* btnHunter;
	GuiButton* btnWizard;
	GuiButton* btnThief;
	GuiButton* btnWarrior;
	GuiButton* btnExit;
};