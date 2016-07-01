#pragma once
#include "cinder/app/App.h"

using namespace cinder;

namespace touchpoints { namespace drawing
{
	struct TouchShape
	{
	public:
		inline float size() const { return mSize; }
		inline ColorA getColor() const { return mColor; }
		inline bool getFilledShape() const { return mFilledShapes; }
		inline float getSize() const { return mSize; }
		inline void setColor(ColorA newColor) { mColor = newColor; }
		inline void DecrementFramesDrawn() { framesDrawn--; }
		inline bool ShouldDraw() { return framesDrawn > 0; }
		virtual void draw() = 0;
	protected:
		bool mFilledShapes;
		ColorA mColor;
		float mSize;
		//this is to help you manage how many framesDrawn to draw this object for. 
		int framesDrawn;
	};
}}