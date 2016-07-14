#pragma once
#include "cinder/app/App.h"
#include "Brush.h"
#include "Illustrator.h"
#include "DeviceHandler.h"
#include "TouchKeyboard.h"

namespace touchpoints
{
	namespace ui
	{
		class UIDependency
		{
		public:
			UIDependency();
			UIDependency(int mWindowWidth, int mWindowHeight, drawing::Brush* brush, drawing::Illustrator* mIllustrator, devices::DeviceHandler* mDeviceHandler, std::shared_ptr<gl::Fbo> fbo, std::vector<std::shared_ptr<gl::Fbo>>* fboLayerList, std::vector<float>* fboLayerAlpha);
			void setWindowWidth(bool sWindowWidth);
			int getWindowWidth();
			void setWindowHeight(bool sWindowHeight);
			int getWindowHeight();
			void setModeButtons(bool sModeButtons);
			bool getModeButtons();
			void setBrushButtons(bool sBrushButtons);
			void toggleBrushButtons(bool sBrushButtons);
			bool getBrushButtons();
			void setColorButtons(bool sColorButtons);
			void toggleColorButtons(bool sColorButtons);
			bool getColorButtons();
			void setShapeButtons(bool sShapeButtons);
			void toggleShapeButtons(bool sShapeButtons);
			bool getShapeButtons();
			void toggleDeviceButtons(bool sDeviceButtons);
			void setDeviceButtons(bool sDeviceButtons);
			bool getDeviceButtons();
			void setLayerVisualizations(bool sLayerVisualization);
			void toggleLayerVisualizations(bool sLayerVisualization);
			bool getLayerVisualization();
			void toggleUiFlag();
			bool getUiFboFlag();
			void setModeChangeFlag();
			Color getBackgroundColor();
			bool getSettingsButtons();
			void toggleSettingsButtons(bool tSettingsButtons);
			drawing::Brush* getBrush();
			drawing::Illustrator* getIllustrator();
			devices::DeviceHandler* getDeviceHandler();

		private:

			drawing::Brush* mBrush;
			drawing::Illustrator* illustrator;
			devices::DeviceHandler* deviceHandler;
			TouchKeyboard keyboard;

			//Frame Buffers for the UI. Mostly Fbo's that held button draw calls.
			std::shared_ptr<gl::Fbo> uiFbo;

			int windowWidth;
			int windowHeight;

			int uiOutlineSize = 5;

			//Mode Flags
			bool modeChangeFlag;
			bool uiFboFlag;
			bool modeButtons;
			bool brushButtons;
			bool colorButtons;
			bool shapeButtons;
			bool deviceButtons;
			bool layerVisualization;
			bool uiFlag;
			bool transparentBackground;
			bool settingsButtons = false;

			std::vector<std::shared_ptr<gl::Fbo>>* layerList;

			list<Color> backgroundList;
			std::vector<float>* layerAlpha;
			Color backgroundColor;
		};
	}
}
