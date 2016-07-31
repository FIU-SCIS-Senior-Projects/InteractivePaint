#pragma once
#include "cinder/app/App.h"
#include "AllShapes.h"

namespace touchpoints { namespace drawing
{
	struct SymmetryLine
	{
	public:
		SymmetryLine();
		SymmetryLine(float x, bool ySymmetric);
		TouchPoint symmetricLine(TouchPoint line);
		TouchCircle symmetricCircle(TouchCircle circle);
		TouchRectangle symmetricRectangle(TouchRectangle rectangle);
		TouchVerticalTriangle symmetricTriangle(TouchVerticalTriangle triangle);
		shared_ptr<TouchPoint> symmetricLineRef(shared_ptr<TouchPoint> line);
		shared_ptr<TouchCircle> symmetricCircleRef(shared_ptr<TouchCircle> circle);
		shared_ptr<TouchRectangle> symmetricRectangleRef(shared_ptr<TouchRectangle> rectangle);
		shared_ptr<TouchVerticalTriangle> symmetricTriangleRef(shared_ptr<TouchVerticalTriangle> triangle);
		shared_ptr<TouchEraserPoints> symmetricEraserRef(shared_ptr<TouchEraserPoints> eraser);
		vec2 symmetricPoint(vec2 point);
		void toggleSymmetry();
		bool getSymmetryOn();

		//setter for resize in TouchPointsApp
		void SymmetryLine::setSymmetryWindowWidth(float x, bool ySymmetric);

	private:
		vec2 point1;
		vec2 point2;
		bool symmetryOn;
	};
}}
