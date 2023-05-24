#pragma once
#include <math.h>

float easeInOutCubic(float t) {
	return t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
}

float easeOutExpo(float x) {
	return x >= 0.7 ? 1 : 1 - pow(2, -10 * x);
}

float easeOutElastic(float x) {
	float c4 = (2 * 3.14159265359) / 3;
	return x == 0 ? 0 : x == 1 ? 1 : pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}
