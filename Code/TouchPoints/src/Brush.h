#pragma once
#include "cinder/app/App.h"
#include <vector>
#include "SymmetryLine.h"
#include "Enums.h"
#include "Color.h"

namespace touchpoints { namespace drawing
{
	struct Brush
	{
	public:
		Brush();
		Brush(Shape::Shape shape, ColorA color, float alphaColor, int lineSize, bool filledShapes, bool randColor, bool eraserMode, SymmetryLine* mySymmetry);
		ColorA getColor();
		int getLineSize();
		inline Shape::Shape getShape() const { return mShape; }
		SymmetryLine* getSymmetry();
		bool getRandColor();
		bool IsEraserActive() const;
		float getAlphaColor();
		void changeColor(ColorA color);
		void changeColor(int index);
		void changeLineSize(int lineSize);
		void increaseLineSize();
		void decreaseLineSize();
		void changeShape(Shape::Shape shape);
		inline bool getFilledShapes() const { return mFilledShapes; }
		inline void changeFilledShapes(bool filledShapes) { mFilledShapes = filledShapes; }
		inline void toggleFilledShapes() { mFilledShapes = !mFilledShapes; }
		void incrementShape();
		void changeRandColor(bool randColor);
		void changeAlphaColor(float alphaColor);
		void increaseAlpha();
		void decreaseAlpha();
		void incrementColor();
		void changeStaticColor(ourColors staticColor);
		void Brush::decrementColor();
		std::vector<Color> getColorList();
		void Brush::setSymmetry(SymmetryLine* mySymmetry);
	private:
		ColorA mColor;
		int mLineSize;
		Shape::Shape mShape;
		bool mFilledShapes;
		SymmetryLine* mMySymmetry;
		bool mRandColor;
		float mAlphaColor;
		ourColors mStaticColor;
		std::vector<Color> colorList;
		int currShape = 0;
		int currColor = 0;
	};
}}
