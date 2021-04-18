#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "GuiControl.h"

#include "Defs.h"
#include "EASTL/list.h"

class GuiManager
{
public:

	GuiManager();

	// Destructor
	virtual ~GuiManager();

	bool Update(float dt);

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* name, Menu* menu, int min, int max, int value, bool checked);
	void DestroyGuiControl(GuiControl* entity);

public:

	//List<GuiControl*> controls;
	eastl::list<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __GUIMANAGER_H__
