#include "TouchEraserPoints.h"
#include <cinder/gl/draw.h>

namespace touchpoints { namespace drawing
{
	void TouchEraserPoints::draw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		TouchPoint::draw();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
	}
}}
