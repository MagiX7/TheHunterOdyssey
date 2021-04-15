#include "App.h"
#include "Window.h"
#include "Input.h"

#include "Defs.h"
#include "Log.h"

#define MAX_KEYS 300
#define MAX_BUTTONS 15


GamePad::GamePad()
{
	//start = back = guide = x = y = a = b = l1 = r1 = l3 = r3 = up = down = left = right = false;
	buttons = new KeyState[MAX_BUTTONS];
	memset(buttons, KEY_IDLE, sizeof(KeyState) * MAX_BUTTONS);

	//btns = new SDL_GameControllerButton[MAX_BUTTONS];
	//memset(btns, 0, sizeof(SDL_GameControllerButton) * MAX_BUTTONS);
	memset(btns, 0, sizeof(SDL_GameControllerButton) * MAX_BUTTONS);

	btns[0] = SDL_CONTROLLER_BUTTON_A;
	btns[1] = SDL_CONTROLLER_BUTTON_B;
	btns[2] = SDL_CONTROLLER_BUTTON_X;
	btns[3] = SDL_CONTROLLER_BUTTON_Y;
	btns[4] = SDL_CONTROLLER_BUTTON_BACK;
	btns[5] = SDL_CONTROLLER_BUTTON_GUIDE;
	btns[6] = SDL_CONTROLLER_BUTTON_START;
	btns[7] = SDL_CONTROLLER_BUTTON_LEFTSTICK;
	btns[8] = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
	btns[9] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	btns[10] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	btns[11] = SDL_CONTROLLER_BUTTON_DPAD_UP;
	btns[12] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	btns[13] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	btns[14] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;

	enabled = false;
}

GamePad::~GamePad()
{
	RELEASE(buttons);
}

void GamePad::HandleDeviceConnection(int index)
{
	if (SDL_IsGameController(index))
	{
		if (enabled == false)
		{
			if (controller = SDL_GameControllerOpen(index))
			{
				LOG("Found a gamepad with id %i named %s", 0, SDL_GameControllerName(controller));
				enabled = true;
				l_dz = r_dz = 0.1f;
				haptic = SDL_HapticOpen(index);
				if (haptic != nullptr)
					LOG("... gamepad has force feedback capabilities");
				index = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
			}
		}
	}
}

void GamePad::HandleDeviceRemoval(int index)
{
	// If an existing gamepad has the given index, deactivate all SDL device functionallity

	if (enabled /*&& index == index*/)
	{
		SDL_HapticClose(haptic);
		SDL_GameControllerClose(controller);
		RELEASE(buttons);
		//RELEASE(btns);
		//memset(&pad, 0, sizeof(GamePad));
		//RELEASE(pad);
	}
	
}

void GamePad::UpdateGamepadInput()
{
	// Update all input data

	if (enabled == true)
	{
		//int b = SDL_GameControllerEventState(-1);

		for (int i = 0; i <= MAX_BUTTONS; ++i)
		{
			if (SDL_GameControllerGetButton(controller, btns[i]) == 0)
			{
				// KEY IDLE

				if (buttons[i] == KEY_REPEAT || buttons[i] == KEY_DOWN)
					buttons[i] = KEY_UP;
				else
					buttons[i] = KEY_IDLE;
			}
			else if (SDL_GameControllerGetButton(controller, btns[i]) == 1)
			{
				if (buttons[i] == KEY_IDLE)
					buttons[i] = KEY_DOWN;
				else
					buttons[i] = KEY_REPEAT;
				// KEY_DOWN
			}



			//int btns = SDL_GameControllerEventState(-1);
			
			/*
			if (1 == 1)
			{
				if (buttons[i] == KEY_IDLE)
					buttons[i] = KEY_DOWN;
				else
					buttons[i] = KEY_REPEAT;
			}
			else
			{
				if (buttons[i] == KEY_REPEAT || buttons[i] == KEY_DOWN)
					buttons[i] = KEY_UP;
				else
					buttons[i] = KEY_IDLE;
			}*/
		}
		
		/*a = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A) == 1;
		b = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B) == 1;
		x = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X) == 1;
		y = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y) == 1;
		l1 = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1;
		r1 = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1;
		l3 = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1;
		r3 = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1;
		up = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1;
		down = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1;
		left = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1;
		right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1;

		start = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START) == 1;
		guide = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_GUIDE) == 1;
		back = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK) == 1;*/

		l2 = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) / 32767.0f;
		r2 = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / 32767.0f;

		l_x = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX)) / 32767.0f;
		l_y = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY)) / 32767.0f;
		r_x = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX)) / 32767.0f;
		r_y = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY)) / 32767.0f;

		// Apply deadzone. All values below the deadzone will be discarded
		l_x = (fabsf(l_x) > l_dz) ? l_x : 0.0f;
		l_y = (fabsf(l_y) > l_dz) ? l_y : 0.0f;
		r_x = (fabsf(r_x) > r_dz) ? r_x : 0.0f;
		r_y = (fabsf(r_y) > r_dz) ? r_y : 0.0f;

		if (rumble_countdown > 0)
			rumble_countdown--;
	}
}

bool GamePad::ShakeController(int id, int duration, float strength)
{
	bool ret = false;

	// Check if the given id is valid within the array bounds
	if (id < 0 || id >= 0) return ret;

	// Check if the gamepad is active and allows rumble
	//GamePad& pad = pad[id];
	if (!enabled || haptic == nullptr || SDL_HapticRumbleSupported(haptic) != SDL_TRUE) return ret;

	// If the pad is already in rumble state and the new strength is below the current value, ignore this call
	if (duration < rumble_countdown && strength < rumble_strength)
		return ret;

	if (SDL_HapticRumbleInit(haptic) == -1)
	{
		LOG("Cannot init rumble for controller number %d", id);
	}
	else
	{
		SDL_HapticRumbleStop(haptic);
		SDL_HapticRumblePlay(haptic, strength, duration / 60 * 1000); //Conversion from frames to ms at 60FPS

		rumble_countdown = duration;
		rumble_strength = strength;

		ret = true;
	}

	return ret;
}

const char* GamePad::GetControllerName(int id) const
{
	// Check if the given id has a valid controller
	if (id >= 0 && id < 1 && enabled == true && controller != nullptr)
		return SDL_GameControllerName(controller);

	return "unplugged";
}

// =========================================================================================================================

Input::Input() : Module()
{
	name.Create("input");

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
	
	pad = new GamePad();
}

// Destructor
Input::~Input()
{
	RELEASE(pad);
	delete[] keyboard;
}

// Called before render is available
bool Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_INIT_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0)
	{
		LOG("SDL_INIT_HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool Input::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if(mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
				return false;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case(SDL_CONTROLLERDEVICEADDED):
			{
				pad->HandleDeviceConnection(event.cdevice.which);
				break;
			}

			case(SDL_CONTROLLERDEVICEREMOVED):
			{
				pad->HandleDeviceRemoval(event.cdevice.which);
				RELEASE(pad);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouseButtons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
				int scale = app->win->GetScale();
				mouseMotionX = event.motion.xrel / scale;
				mouseMotionY = event.motion.yrel / scale;
				mouseX = event.motion.x / scale;
				mouseY = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}

	if (pad)
		pad->UpdateGamepadInput();

	return true;
}

// Called before quitting
bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");

	if (pad->haptic != nullptr)
	{
		SDL_HapticStopAll(pad[0].haptic);
		SDL_HapticClose(pad[0].haptic);
	}
	if (pad->controller != nullptr) SDL_GameControllerClose(pad->controller);
	
	if (pad != nullptr) RELEASE(pad);

	SDL_QuitSubSystem(SDL_INIT_HAPTIC);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}