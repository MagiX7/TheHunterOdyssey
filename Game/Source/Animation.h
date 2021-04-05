#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 100

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingPong = false;

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingPongDirection = 1;
	bool hasFinished;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
		hasFinished = false;
	}

	bool HasFinished()
	{
		return hasFinished;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingPong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingPong)
				pingPongDirection = -pingPongDirection;

			hasFinished = true;
		}

	}

	bool IsPlaying()
	{
		if (currentFrame < totalFrames - 1)
			return true;

		return false;
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingPongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}
};

#endif