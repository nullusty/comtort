// camera.h
// --------
// Declares the SimpleCamera class which provides
// a representation for how the user sees the 
// nodes through the SimpleCanvas rendering panel.

#ifndef CAMERA_H

#include "glm/glm.hpp"	// GLM core
#include "glm/ext.hpp"	// GLM extensions

class SimpleCamera {
	// global coords of camera position
	glm::vec2 pos{ 0.0f,0.0f };

	// zoom level (altitude)
	float zoom{ 0.0f };

	// width and height of viewport
	float width{ 0.0f };
	float height{ 0.0f };

public:
	SimpleCamera();

	void SetPosition(float x, float y);
	const glm::vec2& GetPosition(void);

	void SetZoomLevel(float zoomLevel);
	float GetZoomLevel(void);

	void SetViewportWidth(float viewportWidth);
	void SetViewportHeight(float viewportHeight);

	float GetViewportWidth(void);
	float GetViewportHeight(void);

	glm::vec2 ViewportTransformation(glm::vec2 globalPosition);
};

#define CAMERA_H
#endif