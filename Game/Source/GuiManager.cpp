#include "App.h"
#include "Input.h"
#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

#include "EASTL/iterator.h"
#include "EASTL/fixed_allocator.h"

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, uint32 id, SDL_Rect bounds, const char* name, SceneMenu* menu)
{
	GuiControl* control = nullptr;

	switch (type)
	{
		// Create the corresponding GuiControl type
		case GuiControlType::BUTTON: 
			control = new GuiButton(id, bounds, name); 
			control->SetObserver(menu);
			break;
		case GuiControlType::CHECKBOX: 
			control = new GuiCheckBox(id, bounds, name);
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

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	eastl::list<GuiControl*>::iterator item = controls.begin().mpNode;

	while (item.mpNode)
	{
		if (item.mpNode->mValue == entity)
		{
			controls.remove(item.mpNode->mValue);
			break;
		}
		item = item.mpNode->mpNext;
	}
}

GuiManager::GuiManager()
{
}

GuiManager::~GuiManager()
{
}

bool GuiManager::Update(float dt)
{
	eastl::list<GuiControl*>::iterator control = controls.begin().mpNode;

	while (control != controls.end())
	{
		control.mpNode->mValue->Update(app->input, dt);
		control = control.next();
	}

	return true;
}

bool GuiManager::Draw()
{
	eastl::list<GuiControl*>::iterator control = controls.begin().mpNode;

	while (control != controls.end())
	{
		control.mpNode->mValue->Draw(app->render);
		control = control.next();
	}

	return true;
}

bool GuiManager::CleanUp()
{
	//ListItem<GuiControl*>* item = controls.start;

	controls.clear();

	return true;
}