#include "TouchImage.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/scoped.h>

namespace touchpoints { namespace drawing {
	TouchImage::TouchImage() : startPoint(vec2(0,0)), width(0), height(0), filePath("") {}

	TouchImage::TouchImage(vec2 startPoint, int width, int height, std::string filePath) 
		: startPoint(startPoint), width(width), height(height), filePath(filePath)
	{
		mColor = ColorA(1.0f, 1.0f, 1.0f, 1.0f);
		texture = gl::Texture::create(loadImage(app::loadAsset(filePath)));
	}
	
	void TouchImage::Draw()
	{
		auto endPoint = vec2(startPoint.x + width, startPoint.y + height);
		auto boundingRect = Rectf(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
		gl::color(mColor);
		gl::draw(texture, boundingRect);
	}
}}
