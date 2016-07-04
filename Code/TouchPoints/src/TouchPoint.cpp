#include "TouchPoint.h"
#include <cinder/gl/Context.h>
#include <cinder/gl/draw.h>

using namespace cinder;

namespace touchpoints { namespace drawing
{
	TouchPoint::TouchPoint() {}

	TouchPoint::TouchPoint(vec2 initialPt, ColorA color, float size)
	{
		pointList.emplace_back(initialPt);
		mColor = color;
		mSize = size;
	}

	TouchPoint::TouchPoint(vec2 initialPt, vec2 endPt, ColorA color, float size)
	{
		pointList.emplace_back(initialPt);
		addPoint(endPt);
		mColor = color;
		mSize = size;
	}

	void TouchPoint::addPoint(const vec2& pt)
	{
		pointList.push_back(pt);
	}

	void TouchPoint::clearPoints()
	{
		pointList.clear();
	}

	vec2 TouchPoint::getFirstPoint()
	{
		return pointList.front();
	}

	vector<vec2> TouchPoint::getPointList() const
	{
		return pointList;
	}

	void TouchPoint::Draw()
	{
		gl::lineWidth(mSize);
		gl::color(mColor);

		for(int i = 1; i < pointList.size(); i++)
		{
			gl::drawLine(pointList[i - 1], pointList[i]);
		}
	}
}}