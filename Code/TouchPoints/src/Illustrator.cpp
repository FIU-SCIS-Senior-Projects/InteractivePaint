#include "Illustrator.h"
#include "cinder/Rand.h"
#include <cinder/gl/scoped.h>
#include <cinder/gl/draw.h>
#include "Line.h"
#include "VerticalTriangle.h"

namespace touchpoints { namespace drawing
{
	Illustrator::Illustrator() {}

	Illustrator::Illustrator(Brush* brush, std::vector<std::shared_ptr<gl::Fbo>>* layerList, SymmetryLine* symmetryLine, 
		int windowWidth, int windowHeight)
		: mBrush(brush), canvas(windowWidth, windowHeight, 3), menuLayer(windowWidth, windowHeight),
		symmetryLine(symmetryLine)
	{
		mLayerList = layerList;
		numberOfActiveDrawings = 0;

		//Create Map for all Fbo's in Program 
		for (auto layers : *mLayerList)
		{
			std::list<std::shared_ptr<gl::Fbo>> storedFbo;
			myTimeMachine.insert(make_pair(layers, storedFbo));
		}
	}

	Illustrator::Illustrator(Brush* brush, vector<shared_ptr<gl::Fbo>>* layerList, SymmetryLine* symmetryLine, 
		int windowWidth, int windowHeight, ui::MenuLayer menuLayer)
		: mBrush(brush), canvas(windowWidth, windowHeight, 3), menuLayer(menuLayer),
		symmetryLine(symmetryLine)
	{
		mLayerList = layerList;
		numberOfActiveDrawings = 0;

		//Create Map for all Fbo's in Program 
		for (auto layers : *mLayerList)
		{
			std::list<std::shared_ptr<gl::Fbo>> storedFbo;
			myTimeMachine.insert(make_pair(layers, storedFbo));
		}
	}

	// setter for the resize in TouchPointsApp
	void Illustrator::setIllustratorResize(Brush* brush, vector<shared_ptr<gl::Fbo>>* layerList)
	{
		mLayerList = layerList;
		mBrush = brush;
		numberOfActiveDrawings = 0;

		//Create Map for all Fbo's in Program 
		for (auto layers : *mLayerList)
		{
			std::list<std::shared_ptr<gl::Fbo>> storedFbo;
			//			myTimeMachine may need to be reset or be swaped with the previous pair
			myTimeMachine.insert(make_pair(layers, storedFbo));
		}
	}

	int Illustrator::getNumberOfActiveDrawings()
	{
		return numberOfActiveDrawings;
	}

	void Illustrator::drawActiveShapes()
	{
		for (auto& activePoint : myActiveCircles)
		{
			activePoint.second.Draw();
			if (mBrush->getSymmetry()->getSymmetryOn()) mBrush->getSymmetry()->symmetricCircle(activePoint.second).Draw();
		}
		for (auto& activePoint : myActiveRectangles)
		{
			activePoint.second.Draw();
			if (mBrush->getSymmetry()->getSymmetryOn()) mBrush->getSymmetry()->symmetricRectangle(activePoint.second).Draw();
		}
		for (auto& activePoint : myActiveTriangles)
		{
			activePoint.second.Draw();
			if (mBrush->getSymmetry()->getSymmetryOn()) mBrush->getSymmetry()->symmetricTriangle(activePoint.second).Draw();
		}
	}

	/*Bresenhams Line Algorithm for smooth lines*/
	void Illustrator::missedPoints(int xi, int yi, int xf, int yf, TouchPoint& points)
	{
		int dx, dy, x, y, d, ds, dt;
		//Find Abs value of difference between x and y coords of two points
		dx = abs(xi - xf);
		dy = abs(yi - yf);
		//Ceck which coordinates have bigger range x or y
		if (dx >= dy)
		{
			d = 2 * dy - dx;
			ds = 2 * dy;
			dt = 2 * (dy - dx);
			//Figure out which point to use as starting point
			if (xi < xf)
			{
				x = xi;
				y = yi;
			}
			else
			{
				x = xf;
				y = yf;
				xf = xi;
				yf = yi;
			}

			//Would be starting point passed to method
			points.addPoint(vec2(x, y));

			while (x < xf)
			{
				if (d < 0)
				{
					d += ds;
				}
				else
				{
					if (y < yf)
					{
						y++;
						d += dt;
					}
					else
					{
						y--;
						d += dt;
					}
				}

				x++;

				points.addPoint(vec2(x, y));
			}
		}
		else
		{
			d = 2 * dx - dy;
			ds = 2 * dx;
			dt = 2 * (dx - dy);
			//Figure out which point to use as starting point
			if (yi < yf)
			{
				x = xi;
				y = yi;
			}
			else
			{
				x = xf;
				y = yf;
				xf = xi;
				yf = yi;
			}

			points.addPoint(vec2(x, y));

			while (y < yf)
			{
				if (d < 0)
				{
					d += ds;
				}
				else
				{
					if (x > xf)
					{
						x--;
						d += dt;
					}
					else
					{
						x++;
						d += dt;
					}
				}

				y++;

				points.addPoint(vec2(x, y));
			}
		}
	}

	void Illustrator::beginTouchShapes(uint32_t myId, vec2 myPos)
	{
		/*Should go here*/
		saveCurrentFbo();
		

		switch (mBrush->getShape())
			{
				case Shape::Shape::Line:
				{
					numberOfActiveDrawings++;
					if (mBrush->getRandColor())
					{
						ColorA newColor(CM_HSV, Rand::randFloat(), 0.5f, 1.0f, mBrush->getAlphaColor());
						myActivePoints.insert(make_pair(myId, TouchPoint(myPos, newColor, mBrush->getLineSize())));
					}
					else
					{
						myActivePoints.insert(make_pair(myId, TouchPoint(myPos, mBrush->getColor(), mBrush->getLineSize())));
					}
					break;
				}
				case Shape::Shape::Circle:
				{
					numberOfActiveDrawings++;
					if (mBrush->getRandColor())
					{
						ColorA newColor(CM_HSV, Rand::randFloat(), 0.5f, 1.0f, mBrush->getAlphaColor());
						bool filledShapes = mBrush->getFilledShapes();
						myActiveCircles.insert(make_pair(myId, TouchCircle(myPos, 30.0f, newColor, mBrush->getLineSize(), filledShapes)));
					}
					else
					{
						bool filledShapes = mBrush->getFilledShapes();
						myActiveCircles.insert(make_pair(myId, TouchCircle(myPos, 30.0f, mBrush->getColor(), mBrush->getLineSize(), filledShapes)));
					}
					break;
				}
				case Shape::Shape::Rectangle:
				{
					numberOfActiveDrawings++;
					if (mBrush->getRandColor())
					{
						ColorA newColor(CM_HSV, Rand::randFloat(), 0.5f, 1.0f, mBrush->getAlphaColor());
						bool filledShapes = mBrush->getFilledShapes();
						myActiveRectangles.insert(make_pair(myId, TouchRectangle(myPos.x, myPos.y, myPos.x, myPos.y, newColor, mBrush->getLineSize(), filledShapes)));
					}
					else
					{
						bool filledShapes = mBrush->getFilledShapes();
						myActiveRectangles.insert(make_pair(myId, TouchRectangle(myPos.x, myPos.y, myPos.x, myPos.y, mBrush->getColor(), mBrush->getLineSize(), filledShapes)));
					}
					break;
				}
				case Shape::Shape::Triangle:
				{
					numberOfActiveDrawings++;
					if (mBrush->getRandColor())
					{
						ColorA newColor(CM_HSV, Rand::randFloat(), 0.5f, 1.0f, mBrush->getAlphaColor());
						bool filledShapes = mBrush->getFilledShapes();
						myActiveTriangles.insert(make_pair(myId, TouchVerticalTriangle(myPos, myPos, myPos, myPos, newColor, mBrush->getLineSize(), filledShapes)));
					}
					else
					{
						bool filledShapes = mBrush->getFilledShapes();
						myActiveTriangles.insert(make_pair(myId, TouchVerticalTriangle(myPos, myPos, myPos, myPos, mBrush->getColor(), mBrush->getLineSize(), filledShapes)));
					}
					break;
				}
				case Shape::Shape::Eraser:
				{
					numberOfActiveDrawings++;
					myActivePoints.insert(make_pair(myId, TouchPoint(myPos, mBrush->getColor(), mBrush->getLineSize() * 2)));
					break;
				}
			}
	}

	void Illustrator::movingTouchShapes(uint32_t myId, vec2 myPos, vec2 prevPos)
	{
			switch (mBrush->getShape())
			{
				case Shape::Shape::Line:
				{
					if (myActivePoints.find(myId) == myActivePoints.end()) return;

					missedPoints(prevPos.x, prevPos.y, myPos.x, myPos.y, myActivePoints[myId]);
					myPoints.push_back(myActivePoints[myId]);
					myActivePoints[myId].clearPoints();

					//Draws to the layer at the end of the list. Which is drawn on 'top'
					mLayerList->back()->bindFramebuffer();
					for (auto oldPoints = myPoints.begin(); oldPoints != myPoints.end();)
					{
						oldPoints->Draw();
						if (mBrush->getSymmetry()->getSymmetryOn()) mBrush->getSymmetry()->symmetricLine(*oldPoints).Draw();
						++oldPoints;
					}
					mLayerList->back()->unbindFramebuffer();
					myPoints.clear();
					break;
				}
				case Shape::Shape::Circle:
				{
					if (myActiveCircles.find(myId) == myActiveCircles.end()) return;
					float a;
					float b;
					float c;
					a = myPos.x - myActiveCircles[myId].getCenterX();
					b = myPos.y - myActiveCircles[myId].getCenterY();
					a = a * a;
					b = b * b;
					c = sqrt(a + b);

					myActiveCircles[myId].changeRadius(c);
					break;
				}
				case Shape::Shape::Rectangle:
				{
					if (myActiveRectangles.find(myId) == myActiveRectangles.end()) return;
					//myActiveRectangles[myId].changePoint(myPos.x, myPos.y);
					break;
				}
				case Shape::Shape::Triangle:
				{
					if (myActiveTriangles.find(myId) == myActiveTriangles.end()) return;
					//myActiveTriangles[myId].changeSize(myPos.x, myPos.y);
					break;
				}
				case Shape::Shape::Eraser:
				{
					if (myActivePoints.find(myId) == myActivePoints.end()) return;

					missedPoints(prevPos.x, prevPos.y, myPos.x, myPos.y, myActivePoints[myId]);
					myPoints.push_back(myActivePoints[myId]);
					myActivePoints[myId].clearPoints();

					//Draws to the layer at the end of the list. Which is drawn on 'top'
					mLayerList->back()->bindFramebuffer();

					glEnable(GL_BLEND);
					glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
					glBlendEquation(GL_FUNC_ADD);

					for (auto oldPoints = myPoints.begin(); oldPoints != myPoints.end();)
					{
						oldPoints->Draw();
						if (mBrush->getSymmetry()->getSymmetryOn()) mBrush->getSymmetry()->symmetricLine(*oldPoints).Draw();
						++oldPoints;
					}

					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glBlendEquation(GL_FUNC_ADD);

					mLayerList->back()->unbindFramebuffer();

					myPoints.clear();
				}
				break;
			}
	}

	void Illustrator::endTouchShapes(uint32_t myId)
	{
		switch (mBrush->getShape())
		{
			case Shape::Shape::Line:
			{
				if (myActivePoints.find(myId) == myActivePoints.end()) return;

				numberOfActiveDrawings--;
				myActivePoints.erase(myId);
				break;
			}
			case Shape::Shape::Circle:
			{
				if (myActiveCircles.find(myId) == myActiveCircles.end()) return;

				numberOfActiveDrawings--;

				myCircles.push_back(myActiveCircles[myId]);

				//Draws to the layer at the end of the list. Which is drawn on 'top'
				mLayerList->back()->bindFramebuffer();
				for (auto oldPoints = myCircles.begin(); oldPoints != myCircles.end();)
				{
					oldPoints->Draw();
					if (mBrush->getSymmetry()->getSymmetryOn()) mBrush->getSymmetry()->symmetricCircle(*oldPoints).Draw();
					++oldPoints;
				}
				mLayerList->back()->unbindFramebuffer();
				myActiveCircles.erase(myId);

				myCircles.clear();
				break;
			}
			case Shape::Shape::Rectangle:
			{
				if (myActiveRectangles.find(myId) == myActiveRectangles.end()) return;

				numberOfActiveDrawings--;

				myRectangles.push_back(myActiveRectangles[myId]);

				//Draws to the layer at the end of the list. Which is drawn on 'top'
				mLayerList->back()->bindFramebuffer();
				for (auto oldPoints = myRectangles.begin(); oldPoints != myRectangles.end();)
				{
					oldPoints->Draw();
					if (mBrush->getSymmetry()->getSymmetryOn()) mBrush->getSymmetry()->symmetricRectangle(*oldPoints).Draw();
					++oldPoints;
				}
				mLayerList->back()->unbindFramebuffer();
				myActiveRectangles.erase(myId);

				myRectangles.clear();
				break;
			}
			case Shape::Shape::Triangle:
			{
				if (myActiveTriangles.find(myId) == myActiveTriangles.end()) return;

				numberOfActiveDrawings--;

				myTriangles.push_back(myActiveTriangles[myId]);
				myActiveTriangles.erase(myId);
				//Draws to the layer at the end of the list. Which is drawn on 'top'
				mLayerList->back()->bindFramebuffer();
				for (auto oldPoints = myTriangles.begin(); oldPoints != myTriangles.end();)
				{
					oldPoints->Draw();
					if (mBrush->getSymmetry()->getSymmetryOn()) mBrush->getSymmetry()->symmetricTriangle(*oldPoints).Draw();
					++oldPoints;
				}
				mLayerList->back()->unbindFramebuffer();
				myActiveTriangles.erase(myId);

				myTriangles.clear();
				break;
			}
			case Shape::Shape::Eraser:
			{
				if (myActivePoints.find(myId) == myActivePoints.end()) return;

				numberOfActiveDrawings--;
				myActivePoints.erase(myId);
				break;
			}
		}
	}

	//Need to add color background to parameter
	void Illustrator::saveCurrentFbo()
	{
		if (numberOfActiveDrawings > 0)
		{
			return;
		}
		//Check if the active drawing is occuring

		if (myTimeMachine[mLayerList->back()].size() == 100)
		{
			myTimeMachine[mLayerList->back()].pop_back();
		}

		std::shared_ptr<gl::Fbo> tempFbo;

		gl::Fbo::Format format;
		tempFbo = gl::Fbo::create(mLayerList->back()->getSize().x
		                          , mLayerList->back()->getSize().y
		                          , format);

		tempFbo->bindFramebuffer();

		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		gl::color(1.0, 1.0, 1.0, 1.0);
		gl::draw(mLayerList->back()->getColorTexture());
		tempFbo->unbindFramebuffer();

		//Place current fbo on list
		myTimeMachine[mLayerList->back()].emplace_front(tempFbo);
	}

	void Illustrator::undoDraw(Color background)
	{
		if (myTimeMachine[mLayerList->back()].size() != 0)
		{
			mLayerList->back()->bindFramebuffer();

			glClearColor(background.r, background.g, background.b, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			gl::color(1.0, 1.0, 1.0, 1.0);

			gl::draw(myTimeMachine[mLayerList->back()].front()->getColorTexture());

			mLayerList->back()->unbindFramebuffer();

			myTimeMachine[mLayerList->back()].pop_front();
		}
	}

	void Illustrator::clearTimeMachine()
	{
		//Working
		myTimeMachine.clear();
		for (auto layers : *mLayerList)
		{
			std::list<std::shared_ptr<gl::Fbo>> storedFbo;
			myTimeMachine.insert(make_pair(layers, storedFbo));
		}
	}

	void Illustrator::addToTemporaryCircles(TouchCircle tempCircle)
	{
		temporaryCircles.push_back(tempCircle);
	}

	void Illustrator::addToTemporaryCircles(vector<TouchCircle> tempCircles)
	{
		for (auto tempCircle : tempCircles)
		{
			temporaryCircles.push_back(tempCircle);
		}
	}

	void Illustrator::addToTemporaryTriangles(TouchVerticalTriangle tempTriangle)
	{
		temporaryTriangles.push_back(tempTriangle);
	}

	void Illustrator::addToTemporaryTriangles(vector<TouchVerticalTriangle> tempTriangles)
	{
		for (auto tempTriangle : tempTriangles)
		{
			temporaryTriangles.push_back(tempTriangle);
		}
	}

	void Illustrator::addToTemporaryRectangles(TouchRectangle tempRectangle)
	{
		temporaryRectangles.push_back(tempRectangle);
	}

	void Illustrator::addToTemporaryRectangles(vector<TouchRectangle> tempRectangles)
	{
		for (auto tempRectangle : tempRectangles)
		{
			temporaryRectangles.push_back(tempRectangle);
		}
	}

	void Illustrator::drawTemporary()
	{
		for (auto it = begin(temporaryCircles); it != end(temporaryCircles);)
		{
			if (!it->ShouldDraw())
			{
				it = temporaryCircles.erase(it);
			}
			else
			{
				it->Draw();
				it->DecrementFramesDrawn();
				++it;
			}
		}

		for (auto it = begin(temporaryTriangles); it != end(temporaryTriangles);)
		{
			if (!it->ShouldDraw())
			{
				it = temporaryTriangles.erase(it);
			}
			else
			{
				it->Draw();
				it->DecrementFramesDrawn();
				++it;
			}
		}

		for (auto it = begin(temporaryRectangles); it != end(temporaryRectangles);)
		{
			if (!it->ShouldDraw())
			{
				it = temporaryRectangles.erase(it);
			}
			else
			{
				it->Draw();
				it->DecrementFramesDrawn();
				++it;
			}
		}
	}

	void Illustrator::Draw()
	{
		canvas.Draw();
		drawTemporary();
		menuLayer.Draw();
	}

	void Illustrator::Update()
	{
		processDrawEventQueue();
	}

	void Illustrator::AddMenu(shared_ptr<ui::Menu> menu)
	{
		menuLayer.AddMenu(menu);
	}

	void Illustrator::Undo()
	{
		canvas.Undo();
	}

	void Illustrator::addDrawEventToQueue(DrawEvent event)
	{
		drawEventQueue.push(event);
	}

	void Illustrator::addDrawEventsToQueue(vector<DrawEvent> events)
	{
		for (auto event : events)
		{
			drawEventQueue.push(event);
		}
	}

	void Illustrator::processDrawEventQueue()
	{
		for (int i = 0; i < drawEventQueue.size(); i++)
		{
			auto event = drawEventQueue.front();
			drawEventQueue.pop();
			auto currentShape = mBrush->getShape();

			switch (currentShape)
			{
				case(Shape::Circle):
				{
					circleEventHandler(event);
					break;
				}
				case(Shape::Triangle):
				{
					triangleEventHandler(event);
					break;
				}
				case(Shape::Rectangle):
				{
					rectangleEventHandler(event);
					break;
				}
				case(Shape::Line):
				{
					lineEventHandler(event);
					break;
				}
				case(Shape::Eraser):
				{
					eraserEventHandler(event);
					break;
				}
			}
		}
	}

	void Illustrator::circleEventHandler(DrawEvent event)
	{
		auto startPoint = event.ShouldFinalizeShape() ? event.GetStartPoint() : event.GetParentStartPoint();
		auto endPoint = event.GetEndPoint();
		auto radius = math::CalculateDistance(startPoint, endPoint) / 2;
		auto midPoint = math::FindMidPoint(startPoint, endPoint);

		

		if (event.ShouldFinalizeShape())
		{
			auto circle = shared_ptr<TouchCircle>(new TouchCircle(midPoint, radius, mBrush->getColor(), mBrush->getLineSize(), mBrush->getFilledShapes(), 3));
			
			if (symmetryLine->getSymmetryOn())
			{
				auto symmetricCircle = symmetryLine->symmetricCircleRef(circle);
				canvas.AddSymmetricShapes(make_pair(circle, symmetricCircle));
			}
			else
			{
				canvas.AddShape(circle);
			}
		}
		else
		{
			auto circle = TouchCircle(midPoint, radius, mBrush->getColor(), mBrush->getLineSize(), mBrush->getFilledShapes(), 3);
			addToTemporaryCircles(circle);
		}
	}

	void Illustrator::triangleEventHandler(DrawEvent event)
	{
		auto startPoint = event.ShouldFinalizeShape() ? event.GetStartPoint() : event.GetParentStartPoint();
		auto endPoint = event.GetEndPoint();

		//if the endPoint is further down than the start point the triangle is pointing down
		bool isPointingDown = startPoint.y < endPoint.y;

		auto geomTriangle = math::VerticalTriangle(startPoint, endPoint, isPointingDown);

		

		if (event.ShouldFinalizeShape())
		{
			auto triangle = shared_ptr<TouchVerticalTriangle>(new TouchVerticalTriangle(geomTriangle.GetBaseVertexLeft(), geomTriangle.GetBaseVertexRight(),
				geomTriangle.GetOppositeBaseVertex(), geomTriangle.GetBaseCenter(),
				mBrush->getColor(), mBrush->getLineSize(), mBrush->getFilledShapes(), 3));
			if(symmetryLine->getSymmetryOn())
			{
				auto symmetricTriangle = symmetryLine->symmetricTriangleRef(triangle);
				canvas.AddSymmetricShapes(make_pair(triangle, symmetricTriangle));
			}
			else
			{
				canvas.AddShape(triangle);
			}
		}
		else
		{
			auto triangle = TouchVerticalTriangle(geomTriangle.GetBaseVertexLeft(), geomTriangle.GetBaseVertexRight(),
				geomTriangle.GetOppositeBaseVertex(), geomTriangle.GetBaseCenter(),
				mBrush->getColor(), mBrush->getLineSize(), mBrush->getFilledShapes(), 3);
			addToTemporaryTriangles(triangle);
		}
	}

	void Illustrator::rectangleEventHandler(DrawEvent event)
	{
		auto startPoint = event.ShouldFinalizeShape() ? event.GetStartPoint() : event.GetParentStartPoint();
		auto endPoint = event.GetEndPoint();

		

		if (event.ShouldFinalizeShape())
		{
			auto rectangle = shared_ptr<TouchRectangle>(new TouchRectangle(startPoint.x, startPoint.y, endPoint.x, endPoint.y,
				mBrush->getColor(), mBrush->getLineSize(), mBrush->getFilledShapes(), 3));
			
			if(symmetryLine->getSymmetryOn())
			{
				auto symmetricRectangle = symmetryLine->symmetricRectangleRef(rectangle);
				canvas.AddSymmetricShapes(make_pair(rectangle, symmetricRectangle));
			}
			else 
			{
				canvas.AddShape(rectangle);
			}
		}
		else
		{
			TouchRectangle rectangle(startPoint.x, startPoint.y, endPoint.x, endPoint.y,
				mBrush->getColor(), mBrush->getLineSize(), mBrush->getFilledShapes(), 3);
			addToTemporaryRectangles(rectangle);
		}
	}

	void Illustrator::lineEventHandler(DrawEvent event)
	{
		auto lineId = event.GetShapeGuid();
		auto isFinalizableLine = event.ShouldFinalizeShape();

		auto parentLineIterator = unfinalizedActivePointsMap.find(lineId);
		bool isContinuationLine = parentLineIterator != unfinalizedActivePointsMap.end();
		if (isContinuationLine)
		{
			auto parentLine = parentLineIterator->second;
			if (!isFinalizableLine)
			{
				//do not add start point of finalizing event, that is way back at the beggining of the line
				parentLine.addPoint(event.GetStartPoint());
			}
			parentLine.addPoint(event.GetEndPoint());

			if (isFinalizableLine)
			{
				auto sharedLine = make_shared<TouchPoint>(parentLine);
				
				if(symmetryLine->getSymmetryOn())
				{
					auto symmetricLine = symmetryLine->symmetricLineRef(sharedLine);
					canvas.AddSymmetricShapes(make_pair(sharedLine, symmetricLine));
				}
				else
				{
					canvas.AddShape(sharedLine);
				}
				unfinalizedActivePointsMap.erase(lineId);
			}
			else
			{
				unfinalizedActivePointsMap.insert_or_assign(lineId, parentLine);
			}
		}
		else //create new line and finalize it
		{
			auto line = drawing::TouchPoint(event.GetStartPoint(), event.GetEndPoint(), mBrush->getColor(), mBrush->getLineSize());

			if (isFinalizableLine)
			{
				auto sharedLine = make_shared<TouchPoint>(line);

				if (symmetryLine->getSymmetryOn())
				{
					auto symmetricLine = symmetryLine->symmetricLineRef(sharedLine);
					canvas.AddSymmetricShapes(make_pair(sharedLine, symmetricLine));
				}
				else
				{
					canvas.AddShape(sharedLine);
				}
			}
			else
			{
				unfinalizedActivePointsMap.insert_or_assign(lineId, line);
			}
		}
	}

	void Illustrator::eraserEventHandler(DrawEvent event)
	{
		auto lineId = event.GetShapeGuid();
		auto isFinalizableLine = event.ShouldFinalizeShape();

		auto parentLineIterator = unfinalizedActiveEraserMap.find(lineId);
		bool isContinuationLine = parentLineIterator != unfinalizedActiveEraserMap.end();
		if (isContinuationLine)
		{
			auto parentLine = parentLineIterator->second;
			if (!isFinalizableLine)
			{
				//do not add start point of finalizing event, that is way back at the beggining of the line
				parentLine.addPoint(event.GetStartPoint());
			}
			parentLine.addPoint(event.GetEndPoint());

			if (isFinalizableLine)
			{
				auto sharedLine = make_shared<TouchEraserPoints>(parentLine);

				if (symmetryLine->getSymmetryOn())
				{
					auto symmetricLine = symmetryLine->symmetricLineRef(sharedLine);
					canvas.AddSymmetricShapes(make_pair(sharedLine, symmetricLine));
				}
				else
				{
					canvas.AddShape(sharedLine);
				}
				unfinalizedActiveEraserMap.erase(lineId);
			}
			else
			{
				unfinalizedActiveEraserMap.insert_or_assign(lineId, parentLine);
			}
		}
		else //create new line and finalize it
		{
			auto line = drawing::TouchEraserPoints(event.GetStartPoint(), event.GetEndPoint(), mBrush->getColor(), mBrush->getLineSize() * 2);

			if (isFinalizableLine)
			{
				auto sharedLine = make_shared<TouchPoint>(line);

				if (symmetryLine->getSymmetryOn())
				{
					auto symmetricLine = symmetryLine->symmetricLineRef(sharedLine);
					canvas.AddSymmetricShapes(make_pair(sharedLine, symmetricLine));
				}
				else
				{
					canvas.AddShape(sharedLine);
				}
			}
			else
			{
				unfinalizedActiveEraserMap.insert_or_assign(lineId, line);
			}
		}
	}

	vector<shared_ptr<gl::Fbo>>Illustrator::GetLayerList()
	{
		return *mLayerList;
	}
}}
