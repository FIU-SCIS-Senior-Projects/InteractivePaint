#pragma once
#include "TouchShape.h"
#include <cinder/gl/Texture.h>
#include "Alpha.h"

namespace touchpoints { namespace drawing
{
	class TouchTexture : public TouchShape
	{
	public:
		TouchTexture();
		TouchTexture(vec2 startPoint, int width, int height, gl::TextureRef texture, float alpha);
		void Draw() override;
	private:
		vec2 startPoint;
		int width;
		int height;
		gl::TextureRef texture;
		Alpha alpha;
		Rectf boundingRect;
	};
}}
