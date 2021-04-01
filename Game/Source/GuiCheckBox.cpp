#include "GuiCheckBox.h"
#include "App.h"
#include "Audio.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text, Menu* listener) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	this->observer = listener;

	//Load Fx
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/button_click.wav");
	focusedFx = app->audio->LoadFx("Assets/Audio/Fx/button_focused.wav");
	isPlayeable = true;
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(Input* input, float dt)
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

			if (isPlayeable == true)
			{
				app->audio->PlayFx(focusedFx);
				isPlayeable = false;
			}

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				state = GuiControlState::PRESSED;
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
				{
					app->audio->PlayFx(clickFx);
				}
			}

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				checked = !checked;
				NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
			isPlayeable = true;
		}
	}

	return false;
}

bool GuiCheckBox::Draw(Render* render, bool showColliders)
{
	// Draw the right button depending on state
	SDL_Rect checkSection = { section.x + 35, section.y + 10, 16, 12 };

	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		if (showColliders)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &section);
			if (checked) render->DrawTexture(texture, bounds.x + 8, bounds.y + 10, &checkSection);
			render->DrawRectangle(bounds, 150, 150, 150, 150);
		}
	} break;
	case GuiControlState::NORMAL:

		if (showColliders)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &section);
			if (checked) render->DrawTexture(texture, bounds.x + 8, bounds.y + 10, &checkSection);
		}

		break;
	case GuiControlState::FOCUSED:

		if (showColliders)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &section);
			if (checked) render->DrawTexture(texture, bounds.x + 8, bounds.y + 10, &checkSection);
			render->DrawRectangle(bounds, 255, 255, 0, 150);
		}
		break;
	case GuiControlState::PRESSED:

		if (showColliders)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &section);
			if (checked) render->DrawTexture(texture, bounds.x + 8, bounds.y + 10, &checkSection);
			render->DrawRectangle(bounds, 0, 255, 255, 150);
		}

		break;
	case GuiControlState::SELECTED:

		if (showColliders)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &section);
			if (checked) render->DrawTexture(texture, bounds.x + 8, bounds.y + 10, &checkSection);
			render->DrawRectangle(bounds, 0, 255, 0, 150);
		}

		break;
	default:
		break;
	}

	return false;
}
