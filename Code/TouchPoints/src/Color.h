#pragma once
#include <cinder/Color.h>

namespace touchpoints { namespace drawing
{
	enum ourColors
	{
		Black,
		Red,
		Yellow,
		Green,
		Aqua,
		Blue,
		Purple,
		Orange,
		White,
		Transparent
	};

	inline ourColors ColorAToColorEnumMapper(cinder::ColorA color)
	{
		if (color == cinder::ColorA(0.0f, 0.0f, 0.0f, 1.0f))
		{
			return ourColors::Black;
		}
		else if (color == cinder::ColorA(256.0f, 0.0f, 0.0f, 1.0f))
		{
			return ourColors::Red;
		}
		else if (color == cinder::ColorA(256.0f, 256.0f, 0.0f, 1.0f))
		{
			return ourColors::Yellow;
		}
		else if (color == cinder::ColorA(0.0f, 256.0f, 0.0f, 1.0f))
		{
			return ourColors::Green;
		}
		else if (color == cinder::ColorA(0.0f, 256.0f, 256.0f, 1.0f))
		{
			return ourColors::Aqua;
		}
		else if (color == cinder::ColorA(0.0f, 0.0f, 256.0f, 1.0f))
		{
			return ourColors::Blue;
		}
		else if (color == cinder::ColorA(256.0f, 0.0f, 256.0f, 1.0f))
		{
			return ourColors::Purple;
		}
		else if (color == cinder::ColorA(1.0f, 0.3f, 0.0f, 1.0f))
		{
			return ourColors::Orange;
		}
		else if (color == cinder::ColorA(1.0f, 1.0f, 1.0f, 1.0f))
		{
			return ourColors::White;
		}
		else if (color == cinder::ColorA(0.0f, 0.0f, 0.0f, 0.0f))
		{
			return ourColors::Transparent;
		}
	}

	inline cinder::ColorA ColorEnumToColorAMapper(ourColors color)
	{
		if (color == ourColors::Black)
		{
			return  cinder::ColorA(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (color == ourColors::Red)
		{
			return cinder::ColorA(256.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (color == ourColors::Yellow)
		{
			return cinder::ColorA(256.0f, 256.0f, 0.0f, 1.0f);
		}
		else if (color == ourColors::Green)
		{
			return cinder::ColorA(0.0f, 256.0f, 0.0f, 1.0f);
		}
		else if (color == ourColors::Aqua)
		{
			return cinder::ColorA(0.0f, 256.0f, 256.0f, 1.0f);
		}
		else if (color == ourColors::Blue)
		{
			return cinder::ColorA(0.0f, 0.0f, 256.0f, 1.0f);
		}
		else if (color == ourColors::Purple)
		{
			return cinder::ColorA(256.0f, 0.0f, 256.0f, 1.0f);
		}
		else if (color == ourColors::Orange)
		{
			return cinder::ColorA(1.0f, 0.3f, 0.0f, 1.0f);
		}
		else if (color == ourColors::White)
		{
			return cinder::ColorA(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (color == ourColors::Transparent)
		{
			return cinder::ColorA(0.0f, 0.0f, 0.0f, 0.0f);
		}
	}

	inline ourColors IncrementColorEnum(ourColors color)
	{
		if (color == ourColors::Transparent)
		{
			return ourColors::Black;
		}
		else if (color == ourColors::Black)
		{
			return ourColors::Red;
		}
		else if (color == ourColors::Red)
		{
			return ourColors::Yellow;
		}
		else if (color == ourColors::Yellow)
		{
			return ourColors::Green;
		}
		else if (color == ourColors::Green)
		{
			return ourColors::Aqua;
		}
		else if (color == ourColors::Aqua)
		{
			return ourColors::Blue;
		}
		else if (color == ourColors::Blue)
		{
			return ourColors::Purple;
		}
		else if (color == ourColors::Purple)
		{
			return ourColors::Orange;
		}
		else if (color == ourColors::Orange)
		{
			return ourColors::White;
		}
		else if (color == ourColors::White)
		{
			return ourColors::Transparent;
		}
	}
}}
