#include "camera.h"

SimpleCamera::SimpleCamera() {}

void SimpleCamera::SetPosition(float x, float y) {
	pos.x = x;
	pos.y = y;
}
const glm::vec2& SimpleCamera::GetPosition(void) {
	return pos;
}

void SimpleCamera::SetZoomLevel(float zoomLevel) {
	zoom = zoomLevel;
}
float SimpleCamera::GetZoomLevel(void) {
	return zoom;
}

void SimpleCamera::SetViewportWidth(float viewportWidth)
{
	width = viewportWidth;
}
void SimpleCamera::SetViewportHeight(float viewportHeight)
{
	height = viewportHeight;
}
float SimpleCamera::GetViewportHeight(void) {
	return height;
}
float SimpleCamera::GetViewportWidth(void) {
	return width;
}

glm::vec2 SimpleCamera::ViewportTransformation(glm::vec2 globalPosition) {
	// projection matrix
	//glm::mat4 proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	glm::mat4 proj = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f);

	// camera matrix
	glm::mat4 view = glm::lookAt(
		glm::vec3(pos.x, pos.y, zoom),	// camera position
		glm::vec3(pos.x, pos.y, 0.0f),	// camera target
		glm::vec3(0.0f, 1.0f, 0.0f)	// camera up is y+
		);

	// model matrix
	glm::mat4 model = glm::mat4(1.0f);

	// viewport transformation
	//  convert to homogenous coordinates
	glm::vec4 homoPos = glm::vec4(globalPosition.x, globalPosition.y, 0.0f, 1.0f);
	auto cameraCoords = view * model * homoPos;
	//  apply aggregate transformation
	glm::vec4 viewPos = proj * view * model * homoPos;
	//  convert from homogenous coordinates
	glm::vec2 result = glm::vec2(viewPos.x/viewPos.w, viewPos.y/viewPos.w);
	return result;
}
