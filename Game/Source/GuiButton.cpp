#include "App.h"
#include "Audio.h"

#include "GuiButton.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, Menu* listener, Font* font) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->state = GuiControlState::NORMAL;
	this->observer = listener;
	this->font = font;
	this->section = { 0,0,0,0 };
	this->sectionFocused = { 0,0,0,0 };
	this->alineation = 0;

	//Load Fx
	clickFx = app->audio->LoadFx("Assets/Audio/Fx/button_click.wav");
	focusedFx = app->audio->LoadFx("Assets/Audio/Fx/button_focused.wav");
	isPlayable = true;
}

GuiButton::~GuiButton()
{
	app->audio->UnLoadFx(clickFx);
	app->audio->UnLoadFx(focusedFx);

	text.Clear();
}

bool GuiButton::Update(Input* input, float dt, int id)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		input->GetMousePosition(mouseX, mouseY);

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)) && SDL_ShowCursor(-1) == SDL_ENABLE)
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

			// If mouse button pressed -> Generate event!
			if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)
			{
				state = GuiControlState::NORMAL;
				if (NotifyObserver() == false) return false;
			}
		}
		else if (this->id == id)
		{
			if (input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN
				|| input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT)
			{
				state = GuiControlState::FOCUSED;
			}

			if (isPlayable == true && SDL_ShowCursor(-1) == SDL_DISABLE)
			{
				app->audio->PlayFx(focusedFx);
				isPlayable = false;
			}

			if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT
				|| input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
				{
					app->audio->PlayFx(clickFx);
				}
			}

			if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP)
			{
				state = GuiControlState::NORMAL;
				if (NotifyObserver() == false) return false;
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
			isPlayable = true;
		}
	}

	return true;
}

bool GuiButton::Draw(Render* render, bool showColliders, int size, SDL_Color color)
{
	SDL_Rect r = { -render->camera.x + bounds.x, -render->camera.y + bounds.y, bounds.w, bounds.h };
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawTexture(texture, -render->camera.x + bounds.x, -render->camera.y + bounds.y, &section);

		// Left alineation
		if (alineation == 0) render->DrawText(font, text.GetString(), bounds.x + 37, bounds.y + (bounds.h - size) / 2, size, 5, color);
		// Middle Alineation
		else if (alineation == 1) render->DrawCenterText(font, text.GetString(), bounds, size, 5, color);

		if (showColliders) render->DrawRectangle(r, 150, 150, 150, 150);
		else render->DrawRectangle(r, 150, 150, 150, 150);
		break;
	case GuiControlState::NORMAL:
		render->DrawTexture(texture, -render->camera.x + bounds.x, -render->camera.y + bounds.y, &section);

		if (alineation == 0) render->DrawText(font, text.GetString(), bounds.x + 37, bounds.y + (bounds.h - size) / 2, size, 5, color);
		else if (alineation == 1) render->DrawCenterText(font, text.GetString(), bounds, size, 5, color);

		if (showColliders) render->DrawRectangle(r, 255, 0, 0, 150);
		break;
	case GuiControlState::FOCUSED:
		render->DrawTexture(texture, -render->camera.x + bounds.x, -render->camera.y + bounds.y, &section);

		// Left alineation
		if (alineation == 0) render->DrawText(font, text.GetString(), bounds.x + 37, bounds.y + (bounds.h - size) / 2, size, 5, color);
		// Middle Alineation
		else if (alineation == 1) render->DrawCenterText(font, text.GetString(), bounds, size, 5, color);
		
		if (showColliders) render->DrawRectangle(r, 255, 255, 0, 150);
		// Draw a texture when there's texture, if not, default
		else if (this->sectionFocused.w != 0)
		{
			render->DrawTexture(texture, bounds.x, bounds.y, &sectionFocused, false);
			render->DrawRectangle(bounds, 255, 255, 0, 150, true, false);
		}
		else render->DrawRectangle(bounds, 255, 255, 0, 150, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawTexture(texture, -render->camera.x + bounds.x, -render->camera.y + bounds.y, &section);

		if (alineation == 0) render->DrawText(font, text.GetString(), bounds.x + 37, bounds.y + (bounds.h - size) / 2, size, 5, color);
		else if (alineation == 1) render->DrawCenterText(font, text.GetString(), bounds, size, 5, color);

		if (showColliders) render->DrawRectangle(r, 0, 255, 255, 150);
		else render->DrawRectangle(r, 0, 255, 255, 150);
		break;
	case GuiControlState::SELECTED:
		render->DrawTexture(texture, -render->camera.x + bounds.x, -render->camera.y + bounds.y, &section);

		if (alineation == 0) render->DrawText(font, text.GetString(), bounds.x + 37, bounds.y + (bounds.h - size) / 2, size, 5, color);
		else if (alineation == 1) render->DrawCenterText(font, text.GetString(), bounds, size, 5, color);

		if (showColliders) render->DrawRectangle(r, 0, 255, 0, 150);
		else if (this->sectionFocused.w != 0) render->DrawTexture(texture, bounds.x, bounds.y, &sectionFocused);
		else render->DrawRectangle(r, 0, 255, 0, 150);
		break;
	default:
		break;
	}

	return false;
}