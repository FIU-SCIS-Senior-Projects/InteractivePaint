#include "TouchTexture.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/scoped.h>

namespace touchpoints {
	namespace drawing
	{
		TouchTexture::TouchTexture() {}
		TouchTexture::TouchTexture(vec2 startPoint, int width, int height, gl::TextureRef texture, float alpha)
			: startPoint(startPoint), width(width), height(height), texture(texture), alpha(alpha)
		{
			auto startX = startPoint.x;
			auto startY = startPoint.y;
			boundingRect = Rectf(startX, startY, startX + width, startY + height);
		}
		
		void TouchTexture::Draw()
		{
			gl::color(1.0, 1.0, 1.0, alpha.GetValue());
			gl::draw(texture, boundingRect);
		}
	}
}
