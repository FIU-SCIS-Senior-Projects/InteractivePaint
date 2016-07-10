#pragma once
#include "cinder/Vector.h"

using namespace cinder;

namespace touchpoints { namespace math
{
	class Rect
	{
	public:
		Rect();
		Rect(vec2 oneCorner, vec2 oppositeCorner);
		inline vec2 GetTopLeftCorner() const { return topLeftCorner; }
		inline vec2 GetTopRightCorner() const { return topRightCorner; }
		inline vec2 GetBottomRightCorner() const { return bottomRightCorner; }
		inline vec2 GetBottomLeftCorner() const { return bottomLeftCorner; }
		bool Contains(vec2 point) const;
	private:
		float left;
		float right;
		float top;
		float bottom;
		vec2 topLeftCorner;
		vec2 topRightCorner;
		vec2 bottomRightCorner;
		vec2 bottomLeftCorner;
	};
}}
