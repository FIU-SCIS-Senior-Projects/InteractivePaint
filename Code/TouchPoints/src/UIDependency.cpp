#include "UIDependency.h"

namespace touchpoints
{
	namespace ui
	{
		UIDependency::UIDependency()
		{
			
		}


		UIDependency::UIDependency(int mWindowWidth, int mWindowHeight, drawing::Brush* brush, drawing::Illustrator* mIllustrator, devices::DeviceHandler* mDeviceHandler, std::shared_ptr<gl::Fbo> fbo, std::vector<std::shared_ptr<gl::Fbo>>* fboLayerList, std::vector<float>* fboLayerAlpha)
		{
			modeChangeFlag = true;
			windowWidth = mWindowWidth;
			windowHeight = mWindowHeight;
			mBrush = brush;
			illustrator = mIllustrator;
			deviceHandler = mDeviceHandler;
			uiFbo = fbo;
			uiFboFlag = true;
			modeButtons = true;
			brushButtons = false;
			colorButtons = false;
			shapeButtons = false;
			layerVisualization = false;
			deviceButtons = false;
			transparentBackground = true;

			//Adds our backgroundColors to the list. 
			backgroundList.emplace_back(Color(0.0f, 0.0f, 0.0f));
			backgroundList.emplace_back(Color(256.0f, 256.0f, 256.0f));
			backgroundList.emplace_back(Color(256.0f, 0.0f, 0.0f));
			backgroundList.emplace_back(Color(256.0f, 256.0f, 0.0f));
			backgroundList.emplace_back(Color(0.0f, 256.0f, 0.0f));
			backgroundList.emplace_back(Color(0.0f, 256.0f, 256.0f));
			backgroundList.emplace_back(Color(0.0f, 0.0f, 256.0f));
			backgroundList.emplace_back(Color(256.0f, 0.0f, 256.0f));

			backgroundColor = backgroundList.front();
			layerList = fboLayerList;
			layerAlpha = fboLayerAlpha;
			for (auto layers : *layerList)
			{
				(*layerAlpha).emplace_back(1.0f);
			}
			keyboard = TouchKeyboard(mWindowWidth, mWindowHeight);
		}

		void UIDependency::setWindowWidth(bool sWindowWidth)
		{
			windowWidth = sWindowWidth;
		}

		int UIDependency::getWindowWidth()
		{
			return windowWidth;
		}

		void UIDependency::setWindowHeight(bool sWindowHeight)
		{
			windowHeight = sWindowHeight;
		}

		int UIDependency::getWindowHeight()
		{
			return windowHeight;
		}

		void UIDependency::setModeButtons(bool sModeButtons)
		{
			modeButtons = sModeButtons;
		}

		bool UIDependency::getModeButtons()
		{
			return modeButtons;
		}

		//created bc UIDependency and interactiveUI both use brushButtons
		void UIDependency::setBrushButtons(bool sBrushButtons)
		{
			brushButtons = sBrushButtons;
		}

		void UIDependency::toggleBrushButtons(bool sBrushButtons)
		{
			brushButtons = !sBrushButtons;
		}

		bool UIDependency::getBrushButtons()
		{
			return brushButtons;
		}

		//created bc UIDependency and interactiveUI both use brushButtons
		void UIDependency::setColorButtons(bool sColorButtons)
		{
			colorButtons = sColorButtons;
		}

		void UIDependency::toggleColorButtons(bool sColorButtons)
		{
			colorButtons = !sColorButtons;
		}

		bool UIDependency::getColorButtons()
		{
			return colorButtons;
		}

		//created bc UIDependency and interactiveUI both use brushButtons
		void UIDependency::setShapeButtons(bool sShapeButtons)
		{
			shapeButtons = sShapeButtons;
		}

		void UIDependency::toggleShapeButtons(bool sShapeButtons)
		{
			shapeButtons = !sShapeButtons;
		}

		bool UIDependency::getShapeButtons()
		{
			return shapeButtons;
		}

		//created bc UIDependency and interactiveUI both use brushButtons
		void UIDependency::toggleDeviceButtons(bool sDeviceButtons)
		{
			deviceButtons = !sDeviceButtons;
		}

		void UIDependency::setDeviceButtons(bool sDeviceButtons)
		{
			deviceButtons = sDeviceButtons;
		}

		bool UIDependency::getDeviceButtons()
		{
			return deviceButtons;
		}

		//created bc UIDependency and interactiveUI both use brushButtons
		void UIDependency::setLayerVisualizations(bool sLayerVisualization)
		{
			layerVisualization = sLayerVisualization;
		}

		void UIDependency::toggleLayerVisualizations(bool sLayerVisualization)
		{
			layerVisualization = !sLayerVisualization;
		}

		bool UIDependency::getLayerVisualization()
		{
			return layerVisualization;
		}

		void UIDependency::toggleUiFlag()
		{
			if (uiFlag)
			{
				modeButtons = false;
				uiFboFlag = false;
				uiFlag = false;
			}
			else
			{
				uiFlag = true;
				uiFboFlag = true;
				modeButtons = true;
			}
		}

		bool UIDependency::getUiFboFlag()
		{
			return uiFboFlag;
		}

		void UIDependency::setModeChangeFlag()
		{
			modeChangeFlag = true;
		}

		Color UIDependency::getBackgroundColor()
		{
			return backgroundColor;
		}

		bool UIDependency::getSettingsButtons()
		{
			return settingsButtons;
		}

		void UIDependency::toggleSettingsButtons(bool tSettingsButtons)
		{
			settingsButtons = !tSettingsButtons;
		}

		drawing::Brush* UIDependency::getBrush()
		{
			return mBrush;
		}

		drawing::Illustrator* UIDependency::getIllustrator()
		{
			return illustrator;
		}

		devices::DeviceHandler* UIDependency::getDeviceHandler()
		{
			return deviceHandler;
		}

	}
};
