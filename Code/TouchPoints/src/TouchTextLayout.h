#pragma once
#include "TouchShape.h"
#include <cinder/gl/Texture.h>

namespace touchpoints { namespace drawing
{
	struct TouchTextLayout : public TouchShape
	{
		TouchTextLayout();
		TouchTextLayout(vec2 startPoint, int width, int height, std::string text,
		std::string font, ColorA textColor, int fontSize);

		inline std::string GetText() const { return text; }
		inline std::string GetFont() const { return font; }
		inline ColorA GetTextColor() const { return textColor; }
		inline int GetFontSize() const { return fontSize; }

		inline void SetText(std::string value) { text = value; }
		inline void SetFont(std::string value) { font = value; }
		inline void SetTextColor(ColorA value) { textColor = value; }
		inline void SetFontSize(int value) { fontSize = value; }

		void Draw() override;
	private:
		vec2 startPoint;
		int width;
		int height;
		std::string text;
		std::string font;
		ColorA textColor;
		int fontSize;
		gl::Texture2dRef texture;
		
		void initializeTexture();
	};
}}
