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
			auto startX = startPoint.x + 2;//+52;
			auto startY = startPoint.y+3;//-196;
			auto startX2 = startX + width - 55;
			auto startY2 = startY + height + 135;
			boundingRect = Rectf(startX, startY, startX2, startY2);
		}
		
		void TouchTexture::Draw()
		{
			gl::color(1.0, 1.0, 1.0, alpha.GetValue());
			gl::draw(texture, boundingRect);
		}
	}
}
