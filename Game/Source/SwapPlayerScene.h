#pragma once

#include "Scene.h"

class Player;
class GuiButton;
enum class PlayerType;

class CharacterManager : public Scene
{
public:
	CharacterManager(Player* pl);
	virtual ~CharacterManager();

	bool Load() override;

	bool Update(float dt) override;

	void Draw(bool showColliders);

	bool UnLoad() override;

	Player* CharacterSwap(PlayerType pType);

	bool OnGuiMouseClickEvent(GuiControl* control) override;

	bool GetSwap() const;

	bool GetExit();

	void SetExit(bool setExit);

	Player* GetPlayer();

private:
	Player* player;

	GuiButton* hunterButton;
	GuiButton* wizardButton;
	GuiButton* exitButton;

	bool exit;
	bool swap;
};