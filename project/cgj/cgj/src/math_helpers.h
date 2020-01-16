#pragma once
#ifndef __MATH_HELPERS__
#define __MATH_HELPERS__

#include <random>

constexpr float EPSILON = 0.0001f;
constexpr float PI = 3.14159265359f;

static bool fcmp(float x, float y) {
	return std::abs(x - y) < EPSILON;
}

static int index(int s, int r, int c) {
	return s * r + c;
}

static float degreesToRadians(float d) {
	return d * PI / 180.0f;
}

static float radiansToDegrees(float r) {
	return r * 180.0f / PI;
}

static float random(float low, float high) {
	std::random_device rd;
	std::mt19937 e(rd());
	std::uniform_real_distribution<float> dist(low, high);

	return dist(e);
}

static float smoothstep(float x) {
	return x * x * (3 - 2 * x);
}

#endif