#pragma once
#include "TouchShape.h"
#include <cinder/gl/Texture.h>

namespace touchpoints { namespace drawing
{
	class TouchImage : public TouchShape
	{
	public:
		TouchImage();
		TouchImage(vec2 startPoint, int width, int height, std::string filePath);
		void Draw() override;
	private:
		vec2 startPoint;
		int width;
		int height;
		gl::TextureRef texture;
		std::string filePath;
	};
}}
