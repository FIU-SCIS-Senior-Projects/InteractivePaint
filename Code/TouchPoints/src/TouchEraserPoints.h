#pragma once
#include "TouchPoint.h"

namespace touchpoints { namespace drawing
{
	class TouchEraserPoints : public TouchPoint
	{
	public:
		TouchEraserPoints();
		TouchEraserPoints(vec2 initialPt, ColorA color, float size);
		TouchEraserPoints(vec2 initialPt, vec2 endPt, ColorA color, float size);
		void Draw() override;
	};
}}