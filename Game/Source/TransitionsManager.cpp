#include "App.h"
#include "Window.h"
#include "Render.h"

#include "TransitionsManager.h"

TransitionsManager* TransitionsManager::instance = nullptr;

TransitionsManager* TransitionsManager::GetInstance()
{
	if (instance == nullptr) instance = new TransitionsManager();
	return instance;
}

TransitionsManager::TransitionsManager()
{
	Load();
}

TransitionsManager::~TransitionsManager()
{
}

bool TransitionsManager::Load()
{
	// Transition rects
	// Wipe
	uint w, h;
	app->win->GetWindowSize(w, h);
	rectWipe = { 0,0,0,(int)h };

	// Alternating bars
	for (int i = 0; i < MAX_BARS_SIZE; ++i)
	{
		bars[i].h = h / MAX_BARS_SIZE;
		bars[i].y = i * bars[i].h;
		if (i % 2 == 0)
		{
			bars[i].w = 0;
			bars[i].x = 0;
		}
		else
		{
			bars[i].w = 0;
			bars[i].x = (int)w;
		}
	}

	// Half Height Rectangles
	rectUpper = { 0,0,0,(int)h / 2 };
	rectLower = { (int)w,(int)h / 2,0,(int)h / 2 };

	// Fade to Black
	transitionAlpha = 0.0f;

	// Half Width Rectangles
	rectUpper2 = { 0,0,(int)w, 0 };
	rectLower2 = { 0,(int)h,(int)w,0 };
	halfWidthCount = 0.0f;

	step = TransitionStep::NONE;
	type = TransitionType::NONE;

	return true;
}

void TransitionsManager::Draw()
{
	switch (type)
	{
	case TransitionType::WIPE:
		app->render->DrawRectangle(rectWipe, 0, 0, 0, 255, true, false);
		break;

	case TransitionType::ALTERNATING_BARS:
		for (int i = 0; i < MAX_BARS_SIZE; ++i) 
			app->render->DrawRectangle(bars[i], 0, 0, 0, 255, true, false);
		break;

	case TransitionType::HALF_HEIGHT_RECTANGLES:
		app->render->DrawRectangle(rectUpper, 0, 0, 0, 255, true, false);
		app->render->DrawRectangle(rectLower, 0, 0, 0, 255, true, false);
		break;

	case TransitionType::FADE_TO_BLACK:
		app->render->DrawRectangle({ 0, 0, 1280,720 }, 0, 0, 0, transitionAlpha * 255.0f, true, false);
		break;

	case TransitionType::HALF_WIDHT_RECTANGLES:
		app->render->DrawRectangle(rectUpper2, 0, 0, 0, 255, true, false);
		app->render->DrawRectangle(rectLower2, 0, 0, 0, 255, true, false);
		break;
	}
}

bool TransitionsManager::UnLoad()
{
	RELEASE(instance);
	return true;
}

TransitionStep TransitionsManager::EnteringTransition(float dt)
{
	uint w, h;
	app->win->GetWindowSize(w, h);

	if (type == TransitionType::WIPE)
	{
		rectWipe.w += 1000 * dt;

		if (rectWipe.w >= w) step = TransitionStep::CHANGING;
	}
	else if (type == TransitionType::ALTERNATING_BARS)
	{
		for (int i = 0; i < MAX_BARS_SIZE; ++i)
		{
			if (i % 2 == 0) bars[i].w += 1000 * dt;
			else bars[i].w -= 1000 * dt;
		}

		if (bars[MAX_BARS_SIZE - 1].w < -(int)w) step = TransitionStep::CHANGING;
	}
	else if (type == TransitionType::HALF_HEIGHT_RECTANGLES)
	{
		if (rectUpper.w <= (int)w + 100)
			rectUpper.w += 1000 * dt;
		else
		{
			rectLower.w -= 1000 * dt;
			if (rectLower.w <= -(int)w) step = TransitionStep::CHANGING;
		}
	}
	else if (type == TransitionType::FADE_TO_BLACK)
	{
		transitionAlpha += dt;
		if (transitionAlpha > 1.01f)
		{
			step = TransitionStep::CHANGING;
			transitionAlpha = 1.0f;
		}
	}
	else if (type == TransitionType::HALF_WIDHT_RECTANGLES)
	{
		rectUpper2.h += 500 * dt;
		rectLower2.h -= 500 * dt;
		if (rectLower2.h <= -(int)h / 2) step = TransitionStep::CHANGING;
	}

	return step;
}

TransitionStep TransitionsManager::ExitingTransition(float dt)
{
	uint w, h;
	app->win->GetWindowSize(w, h);

	if (type == TransitionType::WIPE)
	{
		rectWipe.w -= 1000 * dt;

		if (rectWipe.w < 0) step = TransitionStep::NONE;
	}
	else if (type == TransitionType::ALTERNATING_BARS)
	{
		for (int i = 0; i < MAX_BARS_SIZE; ++i)
		{
			if (i % 2 == 0) bars[i].w -= 1000 * dt;
			else bars[i].w += 1000 * dt;
		}

		if (bars[MAX_BARS_SIZE - 1].w > 0) 
			step = TransitionStep::NONE;
	}
	else if (type == TransitionType::HALF_HEIGHT_RECTANGLES)
	{
		if (rectLower.w <= 0)
			rectLower.w += 1000 * dt;
		else
		{
			rectUpper.w -= 1000 * dt;
			if (rectUpper.w <= 0) step = TransitionStep::NONE;
		}
	}
	else if (type == TransitionType::FADE_TO_BLACK)
	{
		transitionAlpha -= dt;
		if (transitionAlpha < -0.01f)
		{
			step = TransitionStep::NONE;
			transitionAlpha = 0.0f;
		}
	}
	else if (type == TransitionType::HALF_WIDHT_RECTANGLES)
	{
		halfWidthCount += dt;
		if (halfWidthCount >= 0.5f)
		{
			rectUpper2.h -= 500 * dt;
			rectLower2.h += 500 * dt;
			if (rectLower2.h >= 0) step = TransitionStep::NONE;
		}
	}

	return step;
}