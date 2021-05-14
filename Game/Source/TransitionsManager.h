#pragma once

#include "SDL/include/SDL_rect.h"

#define MAX_BARS_SIZE 20

enum class TransitionType
{
	NONE = 0,
	FADE_TO_BLACK,
	WIPE,
	ALTERNATING_BARS,
	HALF_HEIGHT_RECTANGLES,
	HALF_WIDHT_RECTANGLES
};

enum class TransitionStep
{
	NONE = 0,
	ENTERING,
	CHANGING,
	EXITING
};


class TransitionsManager
{
public:
	TransitionsManager(const TransitionsManager&) = delete;
	TransitionsManager& operator=(const TransitionsManager&) = delete;

	static TransitionsManager* GetInstance();

	virtual ~TransitionsManager();

	//bool Update(float dt);
	void Draw();
	bool UnLoad();

	TransitionStep EnteringTransition(float dt);
	TransitionStep ExitingTransition(float dt);

	inline void SetStep(TransitionStep s) { step = s; }
	inline void SetType(TransitionType t) { type = t; }

	inline const TransitionType& GetType() { return type; }
	inline const TransitionStep& GetStep() { return step; }

private:
	TransitionsManager();
	static TransitionsManager* instance;

	bool Load();

	TransitionType type;
	TransitionStep step;

public:
	// Wipe
	SDL_Rect rectWipe;

	// Alternating bars
	SDL_Rect bars[MAX_BARS_SIZE];

	// Half Height Recangles
	SDL_Rect rectUpper;
	SDL_Rect rectLower;

	// Fade to Black
	float transitionAlpha;

	// Half Width Rectangles
	SDL_Rect rectUpper2;
	SDL_Rect rectLower2;
	float halfWidthCount;

};