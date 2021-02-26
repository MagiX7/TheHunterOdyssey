#include "App.h"
#include "Input.h"
#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

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
	if (control != nullptr) controls.Add(control);

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	ListItem<GuiControl*>* item = controls.start;

	while (item)
	{
		if (item->data == entity)
		{
			controls.Del(item);
			break;
		}
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
	ListItem<GuiControl*>* item = controls.start;

	while (item)
	{
		item->data->Update(app->input, dt);
		item = item->next;
	}

	return true;
}

bool GuiManager::Draw()
{
	ListItem<GuiControl*>* item = controls.start;

	while (item)
	{
		item->data->Draw(app->render);
		item = item->next;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* item = controls.start;

	controls.Clear();

	return true;
}