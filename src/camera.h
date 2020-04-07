#ifndef CAMERA_H

#include "math_defs.h"	// for vec2f
#include "draw_defs.h"	// for rect

class Camera {
	// view rectangle dimensions
	rect mViewRect;
	// view position
	vec2f mViewPos;
	// view orientation (angular, radians)
	float mViewAngle;

	// viewport rectangle dimensions
	rect mClientRect;

public:
	// constructor
	Camera(vec2f viewPosition, rect viewRectangle,
		float viewAngle, rect clientRectangle);

	// view getters/setters
	//  position
	void SetViewPosition(vec2f position);
	const vec2f& GetViewPosition() const;
	//  rectangle
	void SetViewWidth(float width);
	float GetViewWidth() const;
	void SetViewHeight(float height);
	float GetViewHeight() const;
	//  orientation (angular, radians)
	void SetViewAngle(float angle);
	float GetViewAngle() const;

	// view methods interface
	void TranslateView(vec2f translation);
	void ScaleView(rect scale);
	void RotateView(float angle);

	// client setters/getters
	void SetClientWidth(float width);
	float GetClientWidth() const;
	void SetClientHeight(float height);
	float GetClientHeight() const;
	// point transformation interface
	//  global coords -> client coords
	const vec2f& ViewportTransform(vec2f& position) const;
	//  client coords -> global coords
	const vec2f& ViewTransform(vec2f& position) const;
	// rect transforms
	const rect4& ViewTransformRect(rect4& rect4) const;
};

#define CAMERA_H
#endif