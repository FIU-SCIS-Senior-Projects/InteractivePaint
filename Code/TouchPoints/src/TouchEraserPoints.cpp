#include "TouchEraserPoints.h"
#include <cinder/gl/draw.h>

namespace touchpoints { namespace drawing
{
	TouchEraserPoints::TouchEraserPoints() {}
	TouchEraserPoints::TouchEraserPoints(vec2 initialPt, ColorA color, float size) : TouchPoint(initialPt, color, size) {}
	TouchEraserPoints::TouchEraserPoints(vec2 initialPt, vec2 endPt, ColorA color, float size)
		: TouchPoint(initialPt, endPt, color, size) {}

	void TouchEraserPoints::Draw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		TouchPoint::Draw();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
	}
}}
