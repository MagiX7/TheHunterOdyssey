#pragma once

#include "Menu.h"

class Player;
class SceneGameplay;
class GuiButton;
enum class PlayerType;

class CharacterManager : public Menu
{
public:
	CharacterManager(Player* pl, SceneGameplay* s);
	virtual ~CharacterManager();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders);

	bool UnLoad() override;

	bool OnGuiMouseClickEvent(GuiControl* control) override;

	void ChangeButtonState(PlayerType type);

private:
	Player* player;
	SceneGameplay* scene;

	GuiButton* btnHunter;
	GuiButton* btnWizard;
	GuiButton* btnThief;
	GuiButton* btnWarrior;
	GuiButton* btnExit;
};