#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char *text, Menu* listener, int min, int max, int value);
	virtual ~GuiSlider();

	bool Update(Input* input, float dt);
	bool Draw(Render* render, bool showColliders);

	int GetValue() const;
	void SetValue(int value);

private:

	void AddValue();
	void SubstractValue();
	void CalculeValue(Input* input);

	// GuiSlider specific properties
	// Maybe some animation properties for state change?
	SDL_Rect slider;
	int value;

	int minValue;
	int maxValue;

	//Fx
	uint clickFx;
	uint focusedFx;
	bool isPlayable;
};

#endif // __GUISLIDER_H__
