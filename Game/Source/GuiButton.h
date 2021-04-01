#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char *text, Menu* listener);
	virtual ~GuiButton();

	bool Update(Input* input, float dt);
	bool Draw(Render* render, bool showColliders) override;

private:

	// Gui Button specific properties
	// Maybe some animation properties for state change?

	//Fx
	uint clickFx;
	uint focusedFx;
	bool isPlayeable;
};

#endif // __GUIBUTTON_H__
