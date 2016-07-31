#include "SymmetryLine.h"

namespace touchpoints { namespace drawing
{
	SymmetryLine::SymmetryLine() {}

	SymmetryLine::SymmetryLine(float x, bool ySymmetric)
	{
		//Note: else never being called but could be used for vertical symmetry
		if (ySymmetric)
		{
			point1 = vec2(x, 0);
			point2 = vec2(x, 1);
		}
		else
		{
			point1 = vec2(0, x);
			point2 = vec2(1, x);
		}
		symmetryOn = false;
	}

	void SymmetryLine::toggleSymmetry()
	{
		symmetryOn = !symmetryOn;
	}

	bool SymmetryLine::getSymmetryOn()
	{
		return symmetryOn;
	}

	void SymmetryLine::setSymmetryWindowWidth(float x, bool ySymmetric)
	{
		if (ySymmetric)
		{
			point1 = vec2(x, 0);
			point2 = vec2(x, 1);
		}
		else
		{
			point1 = vec2(0, x);
			point2 = vec2(1, x);
		}
	}

	TouchPoint SymmetryLine::symmetricLine(TouchPoint line)
	{
		auto points = line.getPointList();
		vec2 firstPoint = symmetricPoint(points.front());
		TouchPoint symmetricTouch(firstPoint, line.getColor(), line.getSize());
		for (auto touches : points)
		{
			symmetricTouch.addPoint(symmetricPoint(touches));
		}

		return symmetricTouch;
	};

	vec2 SymmetryLine::symmetricPoint(vec2 point)
	{
		if (point1.x - point2.x == 0)
		{
			auto symmetricX = point1.x - (point.x - point1.x);
			return vec2(symmetricX, point.y);
		}
		else if (point1.y - point2.y == 0)
		{
			auto symmetricY = point1.y - (point.y - point1.y);
			return vec2(point.x, symmetricY);
		}
		return vec2(0, 0);
	};

	TouchCircle SymmetryLine::symmetricCircle(TouchCircle circle)
	{
		vec2 symCenter = SymmetryLine::symmetricPoint(circle.getCenter());
		bool x = circle.getFilledShape();
		TouchCircle symmetricCircle(symCenter, circle.getRadius(), circle.getColor(), circle.getSize(), x);
		return symmetricCircle;
	}

	TouchRectangle SymmetryLine::symmetricRectangle(TouchRectangle rectangle)
	{
		vec2 symUpperLeft = SymmetryLine::symmetricPoint(vec2(rectangle.upperLeftX(), rectangle.upperLeftY()));
		vec2 symLowerRight = SymmetryLine::symmetricPoint(vec2(rectangle.lowerRightX(), rectangle.lowerRightY()));

		int x1 = symUpperLeft.x;
		int y1 = symUpperLeft.y;
		int x2 = symLowerRight.x;
		int y2 = symLowerRight.y;

		bool symBool = rectangle.getFilledShape();

		TouchRectangle symmetricRectangle(x1, y1, x2, y2, rectangle.getColor(), rectangle.getSize(), symBool);
		return symmetricRectangle;
	}

	TouchVerticalTriangle SymmetryLine::symmetricTriangle(TouchVerticalTriangle triangle)
	{
		vec2 symPoint1 = symmetricPoint(triangle.GetBaseVertexLeft());
		vec2 symPoint2 = symmetricPoint(triangle.GetBaseVertexRight());
		vec2 symPoint3 = symmetricPoint(triangle.GetOppositeBaseVertex());
		vec2 symBaseCenter = symmetricPoint(triangle.GetBaseCenter());
		bool symBool = triangle.getFilledShape();

		TouchVerticalTriangle symmetricTriangle(symPoint1, symPoint2, symPoint3, symBaseCenter, triangle.getColor(), triangle.getSize(), symBool);
		return symmetricTriangle;
	}

	shared_ptr<TouchPoint> SymmetryLine::symmetricLineRef(shared_ptr<TouchPoint> line)
	{
		auto points = line->getPointList();
		vec2 firstPoint = symmetricPoint(points.front());
		auto symmetricLine = shared_ptr<TouchPoint>(new TouchPoint(firstPoint, line->getColor(), line->getSize()));
		for (auto touches : points)
		{
			symmetricLine->addPoint(symmetricPoint(touches));
		}

		return symmetricLine;
	}

	shared_ptr<TouchCircle> SymmetryLine::symmetricCircleRef(shared_ptr<TouchCircle> circle)
	{
		vec2 symCenter = SymmetryLine::symmetricPoint(circle->getCenter());
		bool x = circle->getFilledShape();
		return shared_ptr<TouchCircle>(new TouchCircle(symCenter, 
			circle->getRadius(), circle->getColor(), circle->getSize(), x));
	}

	shared_ptr<TouchRectangle> SymmetryLine::symmetricRectangleRef(shared_ptr<TouchRectangle> rectangle)
	{
		vec2 symUpperLeft = SymmetryLine::symmetricPoint(vec2(rectangle->upperLeftX(), rectangle->upperLeftY()));
		vec2 symLowerRight = SymmetryLine::symmetricPoint(vec2(rectangle->lowerRightX(), rectangle->lowerRightY()));

		int x1 = symUpperLeft.x;
		int y1 = symUpperLeft.y;
		int x2 = symLowerRight.x;
		int y2 = symLowerRight.y;

		bool symBool = rectangle->getFilledShape();

		return shared_ptr<TouchRectangle>(new TouchRectangle(x1, y1, x2, y2, 
			rectangle->getColor(), rectangle->getSize(), symBool));
	}

	shared_ptr<TouchVerticalTriangle> SymmetryLine::symmetricTriangleRef(shared_ptr<TouchVerticalTriangle> triangle)
	{
		vec2 symPoint1 = symmetricPoint(triangle->GetBaseVertexLeft());
		vec2 symPoint2 = symmetricPoint(triangle->GetBaseVertexRight());
		vec2 symPoint3 = symmetricPoint(triangle->GetOppositeBaseVertex());
		vec2 symBaseCenter = symmetricPoint(triangle->GetBaseCenter());
		bool symBool = triangle->getFilledShape();

		return shared_ptr<TouchVerticalTriangle>(new TouchVerticalTriangle(symPoint1, symPoint2, symPoint3, 
			symBaseCenter, triangle->getColor(), triangle->getSize(), symBool));
	}

	shared_ptr<TouchEraserPoints> SymmetryLine::symmetricEraserRef(shared_ptr<TouchEraserPoints> eraser)
	{
		auto points = eraser->getPointList();
		vec2 firstPoint = symmetricPoint(points.front());
		auto symmetricEraser = shared_ptr<TouchEraserPoints>(new TouchEraserPoints(firstPoint, eraser->getColor(), eraser->getSize()));
		for (auto touches : points)
		{
			symmetricEraser->addPoint(symmetricPoint(touches));
		}

		return symmetricEraser;
	}
}}
