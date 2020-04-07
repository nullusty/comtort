#ifndef MATH_DEFS_H

const float pi = 3.14159265;

// 2D vector
struct vec2f {
	float x{ 0.0f }, y{ 0.0f };	// components
};

// 2D rectangle, full parameterization
struct rect4 {
	// coordinates of center
	float x{ 0.0f };
	float y{ 0.0f };
	// spatial dimensions
	float w{ 0.0f };
	float h{ 0.0f };
};

// interval contains 
//  returns true if a < x < b or b < x < a
bool intervalContains(float a, float b, float x);

#define MATH_DEFS_H
#endif