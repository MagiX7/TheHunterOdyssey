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

	bool Update(Input* input, float dt);
	bool Draw(Render* render, bool showColliders, int size = 64, SDL_Color color = {255, 255, 255, 255});

private:

	// Gui Button specific properties
	// Maybe some animation properties for state change?

	//Fx
	uint clickFx;
	uint focusedFx;
	bool isPlayeable;

	Font* font;
};

#endif // __GUIBUTTON_H__
