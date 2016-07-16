#pragma once
#include "cinder/app/App.h"
#include "IDrawable.h"

using namespace cinder;

namespace touchpoints { namespace drawing
{
	struct TouchShape : public IDrawable
	{
	public:
		inline float size() const { return mSize; }
		inline ColorA getColor() const { return mColor; }
		inline bool getFilledShape() const { return mFilledShapes; }
		inline void setFilledShape(bool filledShapes) { mFilledShapes = filledShapes; }
		inline float getSize() const { return mSize; }
		inline void setColor(ColorA newColor) { mColor = newColor; }
		inline void DecrementFramesDrawn() { framesDrawn--; }
		inline bool ShouldDraw() const { return framesDrawn > 0; }
		virtual void Draw() override = 0;
	protected:
		bool mFilledShapes;
		ColorA mColor;
		float mSize;
		//this is to help you manage how many framesDrawn to Draw this object for. 
		int framesDrawn;
	};
}}