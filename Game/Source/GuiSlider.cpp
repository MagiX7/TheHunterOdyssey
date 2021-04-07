#include "GuiSlider.h"
#include "App.h"
#include "Audio.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, Menu* listener) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->observer = listener;

	//Load Fx
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/button_click.wav");
	focusedFx = app->audio->LoadFx("Assets/Audio/Fx/button_focused.wav");
	isPlayable = true;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (isPlayable == true)
			{
				app->audio->PlayFx(focusedFx);
				isPlayable = false;
			}

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				state = GuiControlState::PRESSED;
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
				{
					app->audio->PlayFx(clickFx);
				}
			}

			//TODO
		}
		else
		{
			state = GuiControlState::NORMAL;
			isPlayable = true;
		}
	}

	return false;
}

bool GuiSlider::Draw(Render* render, bool showColliders)
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED: if (showColliders) render->DrawRectangle(bounds, 100, 100, 100, 255);
		break;
	case GuiControlState::NORMAL: if (showColliders) render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	case GuiControlState::FOCUSED: if (showColliders) render->DrawRectangle(bounds, 255, 255, 0, 255);
		break;
	case GuiControlState::PRESSED: if (showColliders) render->DrawRectangle(bounds, 0, 255, 255, 255);
		break;
	case GuiControlState::SELECTED: if (showColliders) render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	default:
		break;
	}

	return false;
}
