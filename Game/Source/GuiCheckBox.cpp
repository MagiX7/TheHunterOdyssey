#include "App.h"
#include "Audio.h"

#include "GuiCheckBox.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text, Menu* listener, bool checked) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	this->observer = listener;

	this->checked = checked;
	
	//Load Fx
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/button_click.wav");
	focusedFx = app->audio->LoadFx("Assets/Audio/Fx/button_focused.wav");
	isPlayable = true;
}

GuiCheckBox::~GuiCheckBox()
{
	app->audio->UnLoadFx(clickFx);
	app->audio->UnLoadFx(focusedFx);
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

			if (isPlayable == true)
			{
				app->audio->PlayFx(channel, focusedFx);
				isPlayable = false;
			}

			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				state = GuiControlState::PRESSED;
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
				{
					app->audio->PlayFx(channel, clickFx);
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
			isPlayable = true;
		}
	}

	return false;
}

bool GuiCheckBox::Draw(Render* render, bool showColliders)
{
	// Draw the right button depending on state
	SDL_Rect checkSection = { section.x + 35, section.y + 10, 16, 12 };
	SDL_Rect r = bounds;
	r.x = -render->camera.x + bounds.x;
	r.y = -render->camera.y + bounds.y;

	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 32, -render->camera.y + bounds.y, &section);
		if (showColliders)
		{
			//if (checked) render->DrawTexture(texture, bounds.x + 8, bounds.y + 10, &checkSection);
			if (checked) render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 24, -render->camera.y + bounds.y + 10, &checkSection);
			
			render->DrawRectangle(r, 150, 150, 150, 150);
		}
	} 
	break;
	case GuiControlState::NORMAL:

		render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 32, -render->camera.y + bounds.y, &section);
		if (checked) render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 24, -render->camera.y + bounds.y + 10, &checkSection);
		if (showColliders)
		{
			render->DrawRectangle(r, 255, 0, 0, 150);
		}

	break;
	case GuiControlState::FOCUSED:

		render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 32, -render->camera.y + bounds.y, &section);
		r = { -render->camera.x + bounds.x + bounds.w - 32, -render->camera.y + bounds.y, 32,32 };
		render->DrawRectangle(r, 255, 255, 0, 150);

		if (checked) render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 24, -render->camera.y + bounds.y + 10, &checkSection);
		if (showColliders)
		{
			r = bounds;
			r.x = -render->camera.x + bounds.x;
			r.y = -render->camera.y + bounds.y;
			render->DrawRectangle(bounds, 255, 255, 0, 150);
		}
	break;
	case GuiControlState::PRESSED:

		render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 32, -render->camera.y + bounds.y, &section);
		render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 24, -render->camera.y + bounds.y + 10, &checkSection);
		if (showColliders)
		{
			//if (checked) render->DrawTexture(texture, bounds.x + 8, bounds.y + 10, &checkSection);
			render->DrawRectangle(r, 0, 255, 255, 150);
		}

		break;
	case GuiControlState::SELECTED:

		render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 32, -render->camera.y + bounds.y, &section);
		render->DrawTexture(texture, -render->camera.x + bounds.x + bounds.w - 24, -render->camera.y + bounds.y + 10, &checkSection);
		if (showColliders)
		{
			//if (checked) render->DrawTexture(texture, bounds.x + 8, bounds.y + 10, &checkSection);
			render->DrawRectangle(r, 0, 255, 0, 150);
		}

		break;
	default:
		break;
	}

	return false;
}
