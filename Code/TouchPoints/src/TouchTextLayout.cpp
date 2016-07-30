#include "TouchTextLayout.h"
#include <cinder/Text.h>
#include <cinder/gl/scoped.h>
#include <cinder/gl/draw.h>

namespace touchpoints { namespace drawing
{
	TouchTextLayout::TouchTextLayout() { }
	TouchTextLayout::TouchTextLayout(vec2 startPoint, int width, int height, std::string text,
		std::string font, ColorA textColor, int fontSize)
		: startPoint(startPoint), width(width), height(height), text(text),
		font(font), textColor(textColor), fontSize(fontSize)

	{
		initializeTexture();
	}
			
	void TouchTextLayout::initializeTexture()
	{
		TextLayout layout1;
		layout1.clear(ColorA(0.0f, 0.0f, 0.0f, 0.0f));
		layout1.setFont(Font(font, fontSize));
		layout1.setColor(textColor);
		layout1.addLine(text);
		Surface8u surface = layout1.render(true, false);
		texture = gl::Texture2d::create(surface);
	}

	void TouchTextLayout::Draw()
	{
		auto x1 = startPoint.x;
		auto y1 = startPoint.y;
		auto x2 = x1 + width;
		auto y2 = y1 + height;

		gl::color(textColor);//IS THIS NECCESARY?
		gl::draw(texture, Rectf(Rectf(x1, y1, x2, y2)));
	}
}}
