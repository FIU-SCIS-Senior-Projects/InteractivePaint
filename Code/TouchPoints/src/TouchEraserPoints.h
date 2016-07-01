#pragma once
#include "TouchPoint.h"

namespace touchpoints { namespace drawing
{
	class TouchEraserPoints : public TouchPoint
	{
	public:
		void draw() override;
	};
}}