#pragma once
#include "Leap.h"
#include "ImageHandler.h"
#include "UserInterface.h"
#include "DrawEvent.h"
#include "guid.h"
#include "Menu.h"

#define TOTAL_SYMBOLS 7

using namespace cinder;

namespace touchpoints { namespace devices
{
	class LeapMotionHandler
	{
	public:
		LeapMotionHandler();
		LeapMotionHandler(int windowWidth, int windowHeight, drawing::Illustrator* illustrator);
		void setLeapMotionResize(int windowWidth, int windowHeight);
		void InitLeapMotion();
		void enableGest(Leap::Controller controller);
		void leapSave(ui::UserInterface& gui, drawing::ImageHandler& imageHandler);
		void leapColorChange(drawing::Brush& brush, ui::UserInterface& gui,
		    drawing::ImageHandler& imageHandler);
		void leapShapeChange(int& currShape, bool& imageFlag, drawing::Brush& brush,
		    ui::UserInterface& gui, drawing::ImageHandler& imageHandler);
		void ProcessDrawInput(bool& lockCurrentFrame);
		void gestRecognition(bool isDrawing, bool& processing,
			int& currShape, bool& imageFlag, drawing::Brush& brush, ui::UserInterface& gui,
			drawing::ImageHandler& imageHandler, gl::Fbo* proxFbo);
		inline Leap::Frame GetCurrentFrame() const { return this->currentFrame; }
		inline void SetCurrentFrame() { this->currentFrame = this->leapController.frame(); }
	private:
		void toggleProximityMenu(gl::Fbo* proxFbo);
		ColorA distanceToColor(float distance);
		GuidGenerator guidGenerator;
		shared_ptr<ui::Menu> proximityMenu;
		drawing::Illustrator* illustrator;
		bool isProximityMenuVisible;
		//gets guid mapped to pointId, or returns a new Guid if not mapping exists
		Guid getGuid(int pointId);
		void createPointIdToGuidMapping(int pointId, Guid guid);
		//keeping 2 lists of Draw events, some that fire constantly while drawing(temporary Draw events)
		//and others that fire only once a touch id draws and then exits Draw zone(finalizing the Draw event)
		unordered_map<Guid, drawing::DrawEvent> finalizeableDrawEvents;
		unordered_map<Guid, drawing::DrawEvent> temporaryDrawEvents;
		//ensures that illustrator can uniquely identify objects from each device
		unordered_map<int, Guid> pointIdToGuidMap;
		int windowWidth;
		int windowHeight;
		Leap::Controller leapController;
		Leap::Frame currentFrame;
		Leap::GestureList gestureList;
		Leap::Listener leapListener;
	};
}}
