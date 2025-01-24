#pragma once

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
	constexpr Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
	Vector4() = default;
};