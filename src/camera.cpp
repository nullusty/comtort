#include <cmath>

#include "camera.h"

// constructor
Camera::Camera(vec2f viewPosition, rect viewRectangle, 
		float viewAngle, rect clientRectangle)
		: mViewPos(viewPosition), mViewRect(viewRectangle), 
		  mViewAngle(viewAngle), mClientRect(clientRectangle)
{}

// view setters/getters
//  position
void Camera::SetViewPosition(vec2f position) {
	mViewPos = position;
}
const vec2f& Camera::GetViewPosition() const {
	return mViewPos;
}
//  rectangle
//  rectangle dimensions
void Camera::SetViewWidth(float width) {
	mViewRect.w = width;
}
float Camera::GetViewWidth() const {
	return mViewRect.w;
}
void Camera::SetViewHeight(float height) {
	mViewRect.h = height;
}
float Camera::GetViewHeight() const {
	return mViewRect.h;
}
//  orientation (angular, radians)
float Camera::GetViewAngle() const {
	return mViewAngle;
}
void Camera::SetViewAngle(float angle) {
	mViewAngle = angle;
}

// view methods
void Camera::TranslateView(vec2f translation) {
	mViewPos.x += translation.x;
	mViewPos.y += translation.y;
}
void Camera::ScaleView(rect scale) {
	mViewRect.w *= scale.w;
	mViewRect.h *= scale.h;
}
void Camera::RotateView(float angle) {
	mViewAngle += angle;
}

// client getters/setters
void Camera::SetClientWidth(float width) {
	mClientRect.w = width;
}
float Camera::GetClientWidth() const {
	return mClientRect.w;
}
void Camera::SetClientHeight(float height) {
	mClientRect.h = height;
}
float Camera::GetClientHeight() const {
	return mClientRect.h;
}

// point transformation interface
//  global coords -> client coords
const vec2f& Camera::ViewportTransform(vec2f& position) const {
	vec2f outPos{ position.x, position.y };
	// translate
	outPos.x -= mViewPos.x;
	outPos.y -= mViewPos.y;
	// rotate
	outPos.x = outPos.x * cos(mViewAngle) - outPos.y * sin(mViewAngle);
	outPos.y = outPos.x * sin(mViewAngle) + outPos.y * cos(mViewAngle);
	// scale
	outPos.x *= mClientRect.w / mViewRect.w;
	outPos.y *= mClientRect.h / mViewRect.h;
	return outPos;
}
const vec2f& Camera::ViewTransform(vec2f& position) const {
	vec2f outPos{ position.x, position.y };
	// scale
	outPos.x *= mViewRect.w / mClientRect.w;
	outPos.y *= mViewRect.h / mClientRect.h;
	// rotate
	outPos.x = outPos.x * cos(-mViewAngle) - outPos.y * sin(-mViewAngle);
	outPos.y = outPos.x * sin(-mViewAngle) + outPos.y * cos(-mViewAngle);
	// translate
	outPos.x += mViewPos.x;
	outPos.y += mViewPos.y;
	return outPos;
}

// rectangle transforms
const rect4& Camera::ViewTransformRect(rect4& rectangle) const {
	rect4 outRect{ rectangle.x, rectangle.y, rectangle.w, rectangle.h };
	// transform position
	vec2f inPos{ rectangle.x, rectangle.y };
	vec2f outPos = ViewTransform(inPos);
	outRect.x = outPos.x;
	outRect.y = outPos.y;

	// transform width
	outRect.w *= mViewRect.w / mClientRect.w;
	outRect.h *= mViewRect.h / mClientRect.h;
	return outRect;
}
