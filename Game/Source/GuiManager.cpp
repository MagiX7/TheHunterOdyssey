#include "App.h"
#include "Input.h"
#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

#include "EASTL/iterator.h"
#include "EASTL/fixed_allocator.h"

GuiManager::GuiManager()
{
}

GuiManager::~GuiManager()
{
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* name, Menu* menu)
{
	GuiControl* control = nullptr;

	switch (type)
	{
		// Create the corresponding GuiControl type
		case GuiControlType::BUTTON: 
			control = new GuiButton(id, bounds, name, menu); 
			control->SetObserver(menu);
			break;
		case GuiControlType::CHECKBOX: 
			control = new GuiCheckBox(id, bounds, name, menu);
			control->SetObserver(menu);
			break;
		case GuiControlType::SLIDER: 
			control = new GuiSlider(id, bounds, name);
			control->SetObserver(menu);
			break;
		default: break;
	}

	// Created entities are added to the list
	if (control != nullptr) controls.push_back(control);

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* control)
{
	eastl::list<GuiControl*>::iterator item;

	for (item = controls.begin(); item != controls.end(); ++item)
	{
		if (*item == control)
		{
			controls.remove(*item);
			break;
		}
	}
}

bool GuiManager::Update(float dt)
{
	eastl::list<GuiControl*>::iterator control = controls.begin().mpNode;

	for (control = controls.begin(); control != controls.end(); ++control)
		(*control)->Update(app->input, dt);

	return true;
}

bool GuiManager::Draw()
{
	eastl::list<GuiControl*>::iterator control;

	for (control = controls.begin(); control != controls.end(); ++control)
		//(*control)->Draw(app->render, );

	return true;
}

bool GuiManager::CleanUp()
{
	controls.clear();

	return true;
}