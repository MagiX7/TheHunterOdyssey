#ifndef EASINGS_H
#define EASINGS_H

#include <math.h>
#define PI 3.14159265

enum class Easing_Type
{
	BACK,
	BOUNCE,
	CIRCULAR,
	CUBIC,
	ELASTIC,
	EXPONENTIAL,
	LINEAR,
	QUAD,
	QUART,
	QUINT,
	SINE
};

class Easing 
{
public:
	//CONSTRUCTOR AND DESTRUCTOR
	Easing();
	Easing(Easing_Type type);
	~Easing();

	//BACK METHODS
	static float backEaseIn(float t, float b, float c, float d);
	static float backEaseOut(float t, float b, float c, float d);
	static float backEaseInOut(float t, float b, float c, float d);

	//BOUNCE METHODS
	static float bounceEaseIn(float t, float b, float c, float d);
	static float bounceEaseOut(float t, float b, float c, float d);
	static float bounceEaseInOut(float t, float b, float c, float d);

	//CIRCULAR METHODS
	static float circularEaseIn(float t, float b, float c, float d);
	static float circularEaseOut(float t, float b, float c, float d);
	static float circularEaseInOut(float t, float b, float c, float d);

	//CUBIC METHODS
	static float cubicEaseIn(float t, float b, float c, float d);
	static float cubicEaseOut(float t, float b, float c, float d);
	static float cubicEaseInOut(float t, float b, float c, float d);

	//ELASTIC METHODS
	static float elasticEaseIn(float t, float b, float c, float d);
	static float elasticEaseOut(float t, float b, float c, float d);
	static float elasticEaseInOut(float t, float b, float c, float d);

	//EXPONENTIAL METHODS
	static float exponentialEaseIn(float t, float b, float c, float d);
	static float exponentialEaseOut(float t, float b, float c, float d);
	static float exponentialEaseInOut(float t, float b, float c, float d);

	//LINEAR METHODS
	static float linearEaseNone(float t, float b, float c, float d);
	static float linearEaseIn(float t, float b, float c, float d);
	static float linearEaseOut(float t, float b, float c, float d);
	static float linearEaseInOut(float t, float b, float c, float d);

	//QUAD METHODS
	static float quadEaseIn(float t, float b, float c, float d);
	static float quadEaseOut(float t, float b, float c, float d);
	static float quadEaseInOut(float t, float b, float c, float d);

	//QUART METHODS
	static float quartEaseIn(float t, float b, float c, float d);
	static float quartEaseOut(float t, float b, float c, float d);
	static float quartEaseInOut(float t, float b, float c, float d);

	//QUINT METHODS
	static float quintEaseIn(float t, float b, float c, float d);
	static float quintEaseOut(float t, float b, float c, float d);
	static float quintEaseInOut(float t, float b, float c, float d);

	//SINE METHODS
	static float sineEaseIn(float t, float b, float c, float d);
	static float sineEaseOut(float t, float b, float c, float d);
	static float sineEaseInOut(float t, float b, float c, float d);

public:
	Easing_Type type;
	float currentIteration, initialPos, deltaPos, totalIterations;
};

#endif // EASINGS_H
