#include "Rect.h"
#include <algorithm>

namespace  touchpoints { namespace math
{
	Rect::Rect() : Rect(vec2(0,0), vec2(0,0)) {}
	Rect::Rect(vec2 oneCorner, vec2 oppositeCorner)
	{
		//(0,0) is in the top left corner
		left = std::min(oneCorner.x, oppositeCorner.x);
		right = std::max(oneCorner.x, oppositeCorner.x);
		top = std::min(oneCorner.y, oppositeCorner.y);
		bottom = std::max(oneCorner.y, oppositeCorner.y);

		topLeftCorner = vec2(left, top);
		topRightCorner = vec2(right, top);
		bottomRightCorner = vec2(right, bottom);
		bottomLeftCorner = vec2(left, bottom);
	}

	bool Rect::Contains(vec2 point) const
	{
		auto x = point.x;
		auto y = point.y;

		return x >= left && x <= right && y >= top && y <= bottom;
	}
}}