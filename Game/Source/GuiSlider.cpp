#include "App.h"
#include "Audio.h"

#include "GuiSlider.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, Menu* listener, int min, int max, int value) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->observer = listener;
	this->minValue = min;
	this->maxValue = max;
	this->value = value * 2;
	this->channel = app->audio->SetChannel();

	//Load Fx
	clickFx = app->audio->LoadFx("Audio/Fx/button_click.wav");
	focusedFx = app->audio->LoadFx("Audio/Fx/button_focused.wav");
	isPlayable = true;
}

GuiSlider::~GuiSlider()
{
	//app->audio->UnLoadFx(clickFx);
	//app->audio->UnLoadFx(focusedFx);
}

bool GuiSlider::Update(Input* input, float dt, int id)
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

			if ((mouseX > bounds.x) && (mouseX < bounds.x + 46)) this->sectionFocused = { section.x, section.y + section.h + 5, 46, 42 };
			else if ((mouseX < bounds.x + bounds.w) && (mouseX > bounds.x + bounds.w - 46)) this->sectionFocused = { section.x + section.w - 46, section.y + section.h + 5, 46, 42 };

			if ((mouseX > bounds.x) && (mouseX < bounds.x + 46) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				SubstractValue();
				NotifyObserver();
			}
			else if ((mouseX < bounds.x + bounds.w) && (mouseX > bounds.x + bounds.w - 46) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				AddValue();
				NotifyObserver();
			}
			else if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				state = GuiControlState::PRESSED;
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
				{
					CalculeValue(input);
					NotifyObserver();
				}
				else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)	app->audio->PlayFx(channel, clickFx);
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
				app->audio->PlayFx(channel, focusedFx);
				isPlayable = false;
			}

			if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT
				|| input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
				{
					app->audio->PlayFx(channel, clickFx);
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

	return false;
}

bool GuiSlider::Draw(Render* render, bool showColliders)
{
	if (this->value > 221) this->value = 220;
	else if (this->value < 0) this->value = 0;
	slider = { section.x + 59, section.y + section.h + 5, this->value, 32 };

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED: 
		render->DrawTexture(texture, bounds.x, bounds.y, &section, false);
		render->DrawTexture(texture, bounds.x + 59, bounds.y + 6, &slider, false);
		if (showColliders) render->DrawRectangle(bounds, 100, 100, 100, 150, true, false);
		break;

	case GuiControlState::NORMAL: 
		render->DrawTexture(texture, bounds.x, bounds.y, &section, false);
		render->DrawTexture(texture, bounds.x + 59, bounds.y + 6, &slider, false);
		if (showColliders) render->DrawRectangle(bounds, 0, 255, 0, 150, true, false);
		break;

	case GuiControlState::FOCUSED: 
		render->DrawTexture(texture, bounds.x, bounds.y, &section, false);
		render->DrawTexture(texture, bounds.x + 59, bounds.y + 6, &slider, false);
		{
			SDL_Rect r = { bounds.x, bounds.y, 46,46 };
			if ((mouseX > bounds.x) && (mouseX < bounds.x + 46)) render->DrawRectangle(r, 255, 255, 0, 120, true, false);
			r.x = bounds.x + bounds.w - 46;
			if ((mouseX < bounds.x + bounds.w) && (mouseX > bounds.x + bounds.w - 46)) render->DrawRectangle(r, 255, 255, 0, 120, true, false);

			if (showColliders) render->DrawRectangle(bounds, 255, 255, 0, 150, true, false);
		}
		break;

	case GuiControlState::PRESSED: 
		render->DrawTexture(texture, bounds.x, bounds.y, &section, false);
		render->DrawTexture(texture, bounds.x + 59, bounds.y + 6, &slider, false);
		
		if ((mouseX > bounds.x) && (mouseX < bounds.x + 46)) render->DrawTexture(texture, bounds.x, bounds.y, &sectionFocused, false);
		else if ((mouseX < bounds.x + bounds.w) && (mouseX > bounds.x + bounds.w - 46)) render->DrawTexture(texture, bounds.x + bounds.w - 46, bounds.y, &sectionFocused, false);
		
		render->DrawRectangle(bounds, 0, 255, 255, 150, true, false);

		if (showColliders) render->DrawRectangle(bounds, 0, 255, 255, 150, true, false);
		break;

	case GuiControlState::SELECTED: 
		render->DrawTexture(texture, bounds.x, bounds.y, &section, false);
		render->DrawTexture(texture, bounds.x + 59, bounds.y + 6, &slider, false);
		if (showColliders) render->DrawRectangle(bounds, 0, 255, 0, 150, true, false);
		break;

	default:
		break;
	}

	return false;
}

void GuiSlider::AddValue()
{
	if (value < 221) this->value += 2;
}

void GuiSlider::SubstractValue()
{
	if (value >= 2) this->value -= 2;
}

void GuiSlider::CalculeValue(Input* input)
{
	int mouseX, mouseY;
	input->GetMousePosition(mouseX, mouseY);

	this->value = mouseX - bounds.x - 62;
}

int GuiSlider::GetValue() const
{
	int auxValue = this->value / 2;
	return auxValue;
}

void GuiSlider::SetValue(int value)
{
	this->value = value * 2;
	NotifyObserver();
}