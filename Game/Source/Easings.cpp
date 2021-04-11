#include "Easings.h"

//BACK
float Easing::backEaseIn(float t, float b, float c, float d)
{
	float s = 1.70158f;
	float aux = t /= d;
	return c * (aux)*t * ((s + 1) * t - s) + b;
}

float Easing::backEaseOut(float t, float b, float c, float d)
{
	float s = 1.70158f;
	return c * ((t = t / d - 1) * t * ((s + 1) * t + s) + 1) + b;
}

float Easing::backEaseInOut(float t, float b, float c, float d)
{
	float s = 1.70158f;
	if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s *= (1.525f)) + 1) * t - s)) + b;
	float aux = t -= 2;
	return c / 2 * ((aux)*t * (((s *= (1.525f)) + 1) * t + s) + 2) + b;
}

//BOUNCE
float Easing::bounceEaseIn(float t, float b, float c, float d)
{
	return c - bounceEaseOut(d - t, 0, c, d) + b;
}

float Easing::bounceEaseOut(float t, float b, float c, float d)
{
	if ((t /= d) < (1 / 2.75f)) {
		return c * (7.5625f * t * t) + b;
	}
	else if (t < (2 / 2.75f)) {
		float aux = t -= (1.5f / 2.75f);
		return c * (7.5625f * (aux)*t + .75f) + b;
	}
	else if (t < (2.5 / 2.75)) {
		float aux = t -= (2.25f / 2.75f);
		return c * (7.5625f * (aux)*t + .9375f) + b;
	}
	else {
		float aux = t -= (2.625f / 2.75f);
		return c * (7.5625f * (aux)*t + .984375f) + b;
	}
}

float Easing::bounceEaseInOut(float t, float b, float c, float d)
{
	if (t < d / 2) return bounceEaseIn(t * 2, 0, c, d) * .5f + b;
	else return bounceEaseOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}

//CIRCULAR
float Easing::circularEaseIn(float t, float b, float c, float d)
{
	return -c * (sqrt(1 - (t /= d) * t) - 1) + b;
}

float Easing::circularEaseOut(float t, float b, float c, float d)
{
	return c * sqrt(1 - (t = t / d - 1) * t) + b;
}

float Easing::circularEaseInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;
	return c / 2 * (sqrt(1 - t * (t -= 2)) + 1) + b;
}

//CUBIC
float Easing::cubicEaseIn(float t, float b, float c, float d)
{
	return c * (t /= d) * t * t + b;
}

float Easing::cubicEaseOut(float t, float b, float c, float d)
{
	return c * ((t = t / d - 1) * t * t + 1) + b;
}

float Easing::cubicEaseInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;
	return c / 2 * ((t -= 2) * t * t + 2) + b;
}

//ELASTIC
float Easing::elasticEaseIn(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;
	float p = d * .3f;
	float a = c;
	float s = p / 4;
	float aux = a * pow(2, 10 * (t -= 1));
	return -(aux * sin((t * d - s) * (2 * PI) / p)) + b;
}

float Easing::elasticEaseOut(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;
	float p = d * .3f;
	float a = c;
	float s = p / 4;
	return (a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) + c + b);
}

float Easing::elasticEaseInOut(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d / 2) == 2) return b + c;
	float p = d * (.3f * 1.5f);
	float a = c;
	float s = p / 4;

	if (t < 1)
	{
		float aux = a * pow(2, 10 * (t -= 1));
		return -.5f * (aux * sin((t * d - s) * (2 * PI) / p)) + b;
	}
	float aux = a * pow(2, -10 * (t -= 1));
	return aux * sin((t * d - s) * (2 * PI) / p) * .5f + c + b;
}

//EXPONENTIAL
float Easing::exponentialEaseIn(float t, float b, float c, float d)
{
	return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}

float Easing::exponentialEaseOut(float t, float b, float c, float d)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}

float Easing::exponentialEaseInOut(float t, float b, float c, float d)
{
	if (t == 0) return b;
	if (t == d) return b + c;
	if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;
	return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}

//LINEAR
float Easing::linearEaseNone(float t, float b, float c, float d)
{
	return c * t / d + b;
}

float Easing::linearEaseIn(float t, float b, float c, float d)
{
	return c * t / d + b;
}

float Easing::linearEaseOut(float t, float b, float c, float d)
{
	return c * t / d + b;
}

float Easing::linearEaseInOut(float t, float b, float c, float d)
{
	return c * t / d + b;
}

//QUAD
float Easing::quadEaseIn(float t, float b, float c, float d)
{
	return c * (t /= d) * t + b;
}

float Easing::quadEaseOut(float t, float b, float c, float d)
{
	return -c * (t /= d) * (t - 2) + b;
}

float Easing::quadEaseInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return ((c / 2) * (t * t)) + b;
	return -c / 2 * (((t - 2) * (--t)) - 1) + b;
}

//QUART
float Easing::quartEaseIn(float t, float b, float c, float d)
{
	return c * (t /= d) * t * t * t + b;
}

float Easing::quartEaseOut(float t, float b, float c, float d)
{
	return -c * ((t = t / d - 1) * t * t * t - 1) + b;
}

float Easing::quartEaseInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t * t * t * t + b;
	return -c / 2 * ((t -= 2) * t * t * t - 2) + b;
}

//QUINT
float Easing::quintEaseIn(float t, float b, float c, float d)
{
	return c * (t /= d) * t * t * t * t + b;
}

float Easing::quintEaseOut(float t, float b, float c, float d)
{
	return c * ((t = t / d - 1) * t * t * t * t + 1) + b;
}

float Easing::quintEaseInOut(float t, float b, float c, float d) {
	if ((t /= d / 2) < 1) return c / 2 * t * t * t * t * t + b;
	return c / 2 * ((t -= 2) * t * t * t * t + 2) + b;
}

//SINE
float Easing::sineEaseIn(float t, float b, float c, float d)
{
	return -c * cos(t / d * (PI / 2)) + c + b;
}

float Easing::sineEaseOut(float t, float b, float c, float d)
{
	return c * sin(t / d * (PI / 2)) + b;
}

float Easing::sineEaseInOut(float t, float b, float c, float d)
{
	return -c / 2 * (cos(PI * t / d) - 1) + b;
}