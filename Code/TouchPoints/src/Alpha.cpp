#include "Alpha.h"
#include <cassert>

namespace touchpoints { namespace drawing {
	Alpha::Alpha() : alpha(1.0f) {}
		
	Alpha::Alpha(float value)
	{
		SetValue(value);
	}

	void Alpha::SetValue(float value)
	{
		assert(value >= 0.0f && value <= 1.0f, "Alpha must be between 0.0f and 1.0f");

		this->alpha = value;
	}
}}