#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char *text, Menu* listener, Font* font);
	virtual ~GuiButton();

	bool Update(Input* input, float dt, int id) override;
	bool Draw(Render* render, bool showColliders, int size = 64, SDL_Color color = {255, 255, 255, 255});

	int alineation;

public:

	// Gui Button specific properties
	// Maybe some animation properties for state change?

	//Fx
	uint clickFx;
	uint focusedFx;
	bool isPlayable;

	Font* font;
};

#endif // __GUIBUTTON_H__
