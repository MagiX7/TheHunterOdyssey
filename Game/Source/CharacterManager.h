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

	void CharacterSwap(PlayerType pType);

	bool OnGuiMouseClickEvent(GuiControl* control) override;

private:
	Player* player;
	SceneGameplay* scene;

	GuiButton* hunterButton;
	GuiButton* wizardButton;
	GuiButton* exitButton;
};