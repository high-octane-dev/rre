#pragma once
#include <cmath>

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
	constexpr Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	constexpr Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	constexpr inline float Dot3D(const Vector4& other) {
		return x * other.x + y * other.y + z * other.z;
	}
	constexpr inline Vector4 Cross3D(const Vector4& other) {
		// FIXME: pls verify i suck at math
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x,
			w
		};
	}
	inline float Length3D() {
		return std::sqrtf(x * x + y * y + z * z);
	}
	inline void Normalize3D() {
		float norm = std::sqrtf(x * x + y * y + z * z);
		if (!std::isnan(norm) && norm != 0.0f) {
			x /= norm;
			y /= norm;
			z /= norm;
			w = 1.0f;
			return;
		}
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}
};