#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/Fbo.h"
#include <vector>
#include <list>
#include "AllShapes.h"
#include "Brush.h"
#include <stack>
#include <queue>
#include "DrawEvent.h"
#include <unordered_map>
#include "GuidHash.h"
#include "Canvas.h"
#include "Menu.h"
#include "MenuLayer.h"
#include "SymmetryLine.h"

using namespace std;

namespace touchpoints { namespace drawing
{
	struct Illustrator
	{
	public:
		Illustrator();
		Illustrator(Brush* brush, vector<shared_ptr<gl::Fbo>>* layerList, SymmetryLine* symmetryLine, 
			int windowWidth, int windowHeight);
		Illustrator(Brush* brush, vector<shared_ptr<gl::Fbo>>* layerList, SymmetryLine* symmetryLine, 
			int windowWidth, int windowHeight, ui::MenuLayer menuLayer);
		void beginTouchShapes(uint32_t myId, vec2 myPos);
		void movingTouchShapes(uint32_t myId, vec2 myPos, vec2 prevPos);
		void endTouchShapes(uint32_t myId);
		void missedPoints(int xi, int yi, int xf, int yf, TouchPoint& points);
		int getNumberOfActiveDrawings();
		//Time Machine
		void saveCurrentFbo();
		void undoDraw(Color background);
		void clearTimeMachine();
		void drawActiveShapes();

		//manually add temp shapes to Draw
		void addToTemporaryCircles(TouchCircle tempCircle);
		void addToTemporaryCircles(vector<TouchCircle> tempCircles);
		void addToTemporaryTriangles(TouchVerticalTriangle tempTriangle);
		void addToTemporaryTriangles(vector<TouchVerticalTriangle> tempTriangles);
		void addToTemporaryRectangles(TouchRectangle tempRectangle);
		void addToTemporaryRectangles(vector<TouchRectangle> tempRectangles);
		void addDrawEventToQueue(DrawEvent event);
		void addDrawEventsToQueue(vector<DrawEvent> events);
		void Draw();
		void Update();
		void AddMenu(shared_ptr<ui::Menu> menu);
		void Undo();
		inline void DeleteLayer(int index) { canvas.DeleteLayer(index); };
		inline void AddLayer() { canvas.AddLayer(); };
		vector<shared_ptr<gl::Fbo>>GetLayerList(); //Returns layerslist 
		inline int GetNumberOfLayersInCanvas() const { return canvas.GetNumberOfLayers(); }
		inline void MakeLayerActive(int index) { canvas.MakeLayerActive(index); }
		//alpha of the top layer
		inline float GetTopAlpha() const { return canvas.GetTopAlpha(); }
		inline void SetTopAlpha(float value) { canvas.SetTopAlpha(value); }
		//alpha of i-th layer
		inline float GetAlpha(int index) const { return canvas.GetAlpha(index); }
		inline void SetAlpha(int index, float value) { canvas.SetAlpha(index, value); }
		inline gl::Texture2dRef GetTopLayerTexture() const { return canvas.GetTopLayerTexture(); }
		inline gl::Texture2dRef GetLayerTexture(int index) const { return canvas.GetLayerTexture(index); }
		//setters for resize() in TouchPointsApp
		void Illustrator::setIllustratorResize(Brush* brush, vector<shared_ptr<gl::Fbo>>* layerList);
	private:
		void circleEventHandler(DrawEvent event);
		void triangleEventHandler(DrawEvent event);
		void rectangleEventHandler(DrawEvent event);
		void lineEventHandler(DrawEvent event);
		void eraserEventHandler(DrawEvent event);

		vector<shared_ptr<gl::Fbo>>* mLayerList;
		Brush* mBrush;
		SymmetryLine* symmetryLine;
		int numberOfActiveDrawings;

		Canvas canvas;
		ui::MenuLayer menuLayer;
		GuidGenerator guidGenerator;

		//vectors of temporary shapes drawn for a limited number of frames
		vector<TouchCircle> temporaryCircles;
		vector<TouchVerticalTriangle> temporaryTriangles;
		vector<TouchRectangle> temporaryRectangles;
		//lines not yet finished, can still be appended to by subsequent draw events
		unordered_map<Guid, TouchPoint> unfinalizedActivePointsMap;
		unordered_map<Guid, TouchEraserPoints> unfinalizedActiveEraserMap;

		queue<DrawEvent> drawEventQueue;

		map<uint32_t, TouchPoint> myActivePoints;
		vector<TouchPoint> myPoints;
		map<uint32_t, TouchCircle> myActiveCircles;
		vector<TouchCircle> myCircles;
		map<uint32_t, TouchRectangle> myActiveRectangles;
		vector<TouchRectangle> myRectangles;
		map<uint32_t, TouchVerticalTriangle> myActiveTriangles;
		vector<TouchVerticalTriangle> myTriangles;

		//TimeMachine 
		map<shared_ptr<gl::Fbo>, list<shared_ptr<gl::Fbo>>> myTimeMachine;
		//Stack that tells which item in map to pop
		stack<shared_ptr<gl::Fbo>> undoOrder;
		int undoCount = 0;

		void drawTemporary();
		void processDrawEventQueue();
	};
}}