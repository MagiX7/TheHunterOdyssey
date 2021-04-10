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
	CharacterManager(Player* pl, SceneGameplay* s);
	virtual ~CharacterManager();

	bool Load(Font* font) override;

	bool Update(float dt) override;

	void Draw(Font* font, bool showColliders);

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

	void ChangeButtonState(PlayerType type);

	void HandleInput();

private:
	Player* player;
	SceneGameplay* scene;

	GuiButton* btnHunter;
	GuiButton* btnWizard;
	GuiButton* btnThief;
	GuiButton* btnWarrior;
	GuiButton* btnExit;
};