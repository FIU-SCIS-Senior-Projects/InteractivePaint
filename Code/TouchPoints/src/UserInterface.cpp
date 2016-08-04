#include "UserInterface.h"
#include <cinder/Rand.h>
#include "AllMenus.h"

using namespace cinder::app;

namespace touchpoints { namespace ui
{
	UserInterface::UserInterface() {}

	UserInterface::UserInterface(int mWindowWidth, int mWindowHeight, drawing::Brush* brush, 
		drawing::Illustrator* mIllustrator, devices::DeviceHandler* mDeviceHandler, 
		drawing::SymmetryLine* symmetryLine, std::shared_ptr<gl::Fbo> fbo, 
		std::vector<std::shared_ptr<gl::Fbo>>* fboLayerList, std::vector<float>* fboLayerAlpha)
	{
		modeChangeFlag = true;
		windowWidth = mWindowWidth;
		windowHeight = mWindowHeight;
		mBrush = brush;
		illustrator = mIllustrator;
		deviceHandler = mDeviceHandler;
		this->symmetryLine = symmetryLine;
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

		initializeMenuLayer();

		backgroundColor = backgroundList.front();
		incrementBackground();
		layerList = fboLayerList;
		layerAlpha = fboLayerAlpha;
		for (auto layers : *layerList)
		{
			(*layerAlpha).emplace_back(1.0f);
		}
		keyboard = TouchKeyboard(mWindowWidth, mWindowHeight);
	}

	void UserInterface::setUIResize(int mWindowWidth, int mWindowHeight, drawing::Brush* brush, drawing::Illustrator* mIllustrator, std::shared_ptr<gl::Fbo> fbo, std::vector<std::shared_ptr<gl::Fbo>>* fboLayerList)
	{
		windowWidth = mWindowWidth;
		windowHeight = mWindowHeight;
		mBrush = brush;
		illustrator = mIllustrator;
		modeChangeFlag = true;
		uiFbo.reset();
		uiFbo = fbo;
		modeButtonsFbo.reset();
		brushButtonsFbo.reset();
		deviceButtonsFbo.reset();
		shapeButtonsFbo.reset();
		colorButtonsFbo.reset();
		settingsButtonsFbo.reset();
		//Stores the 'Checkerboard pattern for background
		transparentBackgroundFbo.reset();

		//layerList->clear();
		layerList = fboLayerList;
		for (auto layers : *layerList)
		{
			(*layerAlpha).emplace_back(1.0f);
		}
		//keyboard = TouchKeyboard(mWindowWidth, mWindowHeight);
		keyboard.setTouchKeyboardResize(mWindowWidth, mWindowHeight);
		uiSetup();
		drawUi();
	}

	void UserInterface::initializeMenuLayer()
	{
		menuLayer = MenuLayer(windowWidth, windowHeight);
		initializeTopLeftMenu();
		initializeBottomRightMenu();
		menuLayer.AddMenu(topLeftMenu);
		menuLayer.AddMenu(bottomRightMenu);
	}

	void UserInterface::initializeTopLeftMenu()
	{
		auto colorPickerMenu = createColorPickerMenu();
		auto shapePickerMenu = createShapePickerMenu();
		auto brushPickerMenu = createBrushPickerMenu();
		auto layerVisualizationMenu = createLayerVisualizationMenu();
		auto symmetryDropdownShapes = createSymmetryDropdownShapes();
		auto symmetryMenuShapes = createSymmetryMenuShapes();
		auto symmetryLineMenu = multimap<int, shared_ptr<Menu>>
		{
			make_pair(0, shared_ptr<SymmetryToggleMenu>(new SymmetryToggleMenu(
				vec2(180, 0), Menu::defaultWidth, Menu::defaultHeight, false, 
				symmetryLine, symmetryMenuShapes)))
		};
		auto undoButtonMenu = multimap<int, shared_ptr<Menu>>
		{
			make_pair(0, shared_ptr<UndoButton>(new UndoButton(
				vec2(360, 0), Menu::defaultWidth, Menu::defaultHeight, false,
				illustrator)))
		};

		auto colorPickerDropdown = shared_ptr<Menu>(new Menu(vec2(0, 0), true, Menu::purple, "Colors.png",
			colorPickerMenu, Menu::defaultDropdownCallback));
		auto shapePickerDropdown = shared_ptr<Menu>(new Menu(vec2(60, 0), true, Menu::purple, "Shapes.png",
			shapePickerMenu, Menu::defaultDropdownCallback));
		auto brushPickerDropdown = shared_ptr<Menu>(new Menu(vec2(120, 0), true, Menu::purple, "Brush.png",
			brushPickerMenu, Menu::recursiveDropdownCallback));
		auto symmetryLineButton = shared_ptr<Menu>(new Menu(vec2(180, 0), true, Menu::purple, "",
			symmetryDropdownShapes, symmetryLineMenu, Menu::defaultDropdownCallback));
		auto layerPickerMenu = shared_ptr<Menu>(new Menu(vec2(240, 0), true, Menu::purple, "Layers.png",
			layerVisualizationMenu, Menu::defaultDropdownCallback));
		auto keyboardMenu = shared_ptr<Menu>(new Menu(vec2(300, 0), true, Menu::purple, "Letter.png", nullptr));
		auto undoButton = shared_ptr<Menu>(new Menu(vec2(360, 0), true, Menu::purple, "Undo.png", undoButtonMenu,
			nullptr));

		topLeftMenu = shared_ptr<Menu>(new Menu(vec2(0, 0), 420, 60, true, multimap<int, shared_ptr<Menu>>{
			make_pair(0, colorPickerDropdown),
			make_pair(0, shapePickerDropdown),
			make_pair(0, brushPickerDropdown),
			make_pair(0, symmetryLineButton),
			make_pair(0, layerPickerMenu),
			make_pair(0, keyboardMenu),
			make_pair(0, undoButton)
		}, nullptr));
	}

	void UserInterface::initializeBottomRightMenu()
	{
		auto bottomRightMenuShapes = createBottomRightMenuShapes();
		auto bottomRightMenuComposingMenus = createBottomRightComposingMenus();

		auto startPoint = vec2(windowWidth * 0.8, windowHeight * 0.8);
		auto width = windowWidth - (windowWidth * 0.8);
		auto height = windowHeight - (windowHeight * 0.8);
		bottomRightMenu = shared_ptr<Menu>(new Menu(startPoint, width, height, true, 
			bottomRightMenuShapes, bottomRightMenuComposingMenus, nullptr));
	}

	multimap<int, shared_ptr<Menu>> UserInterface::createColorPickerMenu() const
	{
		multimap<int, shared_ptr<drawing::TouchShape>> colorPickerShapes;
		auto x1 = 0;
		auto y1 = 60;
		auto x2 = 60;
		auto y2 = 120;

		for (auto color : mBrush->getColorList())
		{
			// adds color shapes to the menu to be drawn
			colorPickerShapes.insert(make_pair(0, shared_ptr<drawing::TouchRectangle>
				(new drawing::TouchRectangle(x1, y1, x2, y2, color, 0, true))));

			// adds color menu outline to be drawn
			colorPickerShapes.insert(make_pair(1, shared_ptr<drawing::TouchRectangle>
			// moves the y axis of where the color squares will be drawn
				(new drawing::TouchRectangle(x1, y1, x2, y2, Menu::grey, Menu::defaultBorderThickness, false))));
			y1 += 60;
			y2 += 60;
		}

		//handles which color is being touched and changes to that color
		function<void(vec2 point, BrushModeSelectorMenu *self, drawing::Brush* brush)> colorPickerHandler =
			[](vec2 point, BrushModeSelectorMenu *self, drawing::Brush* brush)
		{
			auto correctedY = point.y - 60;
			int index = static_cast<int>(correctedY / 60);
			brush->changeColor(index);
		};

		// passes these values to the brush handler which passes them to menu and assigns it as a shared_ptr
		auto colorPickerMenu = shared_ptr<BrushModeSelectorMenu>
			(new BrushModeSelectorMenu(vec2(0, 60), 60, 480, false, mBrush, colorPickerHandler, colorPickerShapes));


		return multimap<int, shared_ptr<Menu>>
		{
			make_pair(0, colorPickerMenu)
		};
	}
	
	multimap<int, shared_ptr<Menu>> UserInterface::createShapePickerMenu() const
	{
		auto linePickerMenu = shared_ptr<ShapeModeSelectorMenu>
			(new ShapeModeSelectorMenu(vec2(60, 60), false, mBrush, Shape::Line));

		auto circlePickerMenu = shared_ptr<ShapeModeSelectorMenu>
			(new ShapeModeSelectorMenu(vec2(60, 120), false, mBrush, Shape::Circle));

		auto rectanglePickerMenu = shared_ptr<ShapeModeSelectorMenu>
			(new ShapeModeSelectorMenu(vec2(60, 180), false, mBrush, Shape::Rectangle));

		auto trianglePickerMenu = shared_ptr<ShapeModeSelectorMenu>
			(new ShapeModeSelectorMenu(vec2(60, 240), false, mBrush, Shape::Triangle));

		auto eraserPickerMenu = shared_ptr<ShapeModeSelectorMenu>
			(new ShapeModeSelectorMenu(vec2(60, 300), false, mBrush, Shape::Eraser));

		return multimap<int, shared_ptr<Menu>>
		{
			make_pair(0, linePickerMenu),
			make_pair(0, circlePickerMenu),
			make_pair(0, rectanglePickerMenu),
			make_pair(0, trianglePickerMenu),
			make_pair(0, eraserPickerMenu)
		};
	}

	multimap<int, shared_ptr<Menu>> UserInterface::createBrushPickerMenu() const
	{
		auto sizeLabel = shared_ptr<LabelMenu>(new LabelMenu(vec2(120, 60), 90, 60, false, "Size"));

		auto sizeMinusMenu = shared_ptr<BrushModeSelectorMenu>
			(new BrushModeSelectorMenu(vec2(210, 60), false,
				Menu::black, "BlueMinus.png", mBrush,
				[](vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)
				{ brush->decreaseLineSize(); }));

		auto sizePlusMenu = shared_ptr<BrushModeSelectorMenu>
			(new BrushModeSelectorMenu(vec2(270, 60), false,
				Menu::black, "BluePlus.png", mBrush,
				[](vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)
				{ brush->increaseLineSize(); }));

		auto alphaLabel = shared_ptr<LabelMenu>(new LabelMenu(vec2(120, 120), 90, 60, false, "Alpha"));

		auto alphaMinusMenu = shared_ptr<BrushModeSelectorMenu>
			(new BrushModeSelectorMenu(vec2(210, 120), false,
				Menu::black, "BlueMinus.png", mBrush,
				[](vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)
				{ brush->decreaseAlpha(); }));

		auto alphaPlusMenu = shared_ptr<BrushModeSelectorMenu>
			(new BrushModeSelectorMenu(vec2(270, 120), false,
				Menu::black, "BluePlus.png", mBrush,
				[](vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)
				{ brush->increaseAlpha(); }));

		auto filledLabel = shared_ptr<LabelMenu>(new LabelMenu(vec2(120, 180), 90, 60, false, "Filled"));

		auto filledShapesMenu = shared_ptr<BrushModeSelectorMenu>
			(new BrushModeSelectorMenu(vec2(210, 180), false, mBrush,
				[](vec2 point, BrushModeSelectorMenu* self, drawing::Brush* brush)
				{ brush->toggleFilledShapes(); },
				multimap<int, shared_ptr<Menu>>
				{
					make_pair(0, shared_ptr<SwitchMenu>(new SwitchMenu(vec2(210, 180))))
				}));

		return multimap<int, shared_ptr<Menu>>
		{
			make_pair(0, sizeLabel),
			make_pair(0, sizePlusMenu),
			make_pair(0, sizeMinusMenu),
			make_pair(0, alphaLabel),
			make_pair(0, alphaPlusMenu),
			make_pair(0, alphaMinusMenu),
			make_pair(0, filledLabel),
			make_pair(0, filledShapesMenu)
		};
	}

	multimap<int, shared_ptr<Menu>> UserInterface::createLayerVisualizationMenu() const
	{
		int index = illustrator->GetNumberOfLayersInCanvas() - 1;
		int layerIndex = illustrator->GetNumberOfLayersInCanvas() - 1;
		index = index * 130;
		int layerNumber = 0;
		auto x1 = 290;
		auto y1 = index;
		auto x2 = 640;
		auto y2 = index - 200;
		multimap<int, shared_ptr<Menu>> layerPickerMenus;

		for (auto frame : illustrator->GetLayerList())
		{
			if(layerNumber == 3)
			{
				index = 260;
				x1 = 640;
				y1 = 260;
				x2 = 1040;
				y2 = 60;
			}
			
			auto layerDropdown = std::make_shared<LayerVisualizationMenu>(vec2(x1, y1), x2, y2, layerNumber, false, illustrator, nullptr);
			layerPickerMenus.insert(make_pair(0, layerDropdown));

			auto layerTexture = std::make_shared<LayerTextureMenu>(vec2(x1, y1-200), false, layerNumber, illustrator);
			layerPickerMenus.insert(make_pair(1, layerTexture));

			auto alphaMenu = std::make_shared<AlphaChangerMenu>(vec2(x1-48, y1-197), false, layerNumber, illustrator);
			layerPickerMenus.insert(make_pair(2, alphaMenu));

			index = index + 200;
			y1 = index;
			y2 = index - 200;

			layerNumber++;
			layerIndex--;
		}

		return layerPickerMenus;
	}

	multimap<int, shared_ptr<drawing::TouchShape>> UserInterface::createSymmetryMenuShapes() const
	{
		auto shapes = multimap<int, shared_ptr<drawing::TouchShape>>();
		for (int i = 0; i < 50; i = i + 2)
		{
			shapes.insert(make_pair(0, shared_ptr<drawing::TouchPoint>(new drawing::TouchPoint(
				vec2(windowWidth / 2, windowHeight - i * 50), vec2(windowWidth / 2, windowHeight - (i + 1) * 50),
				Menu::grey, 4))));
		}
		return shapes;
	}
	
	multimap<int, shared_ptr<drawing::TouchShape>> UserInterface::createSymmetryDropdownShapes() const
	{
		return multimap<int, shared_ptr<drawing::TouchShape>>
		{
			make_pair(1, shared_ptr<drawing::TouchPoint>(new drawing::TouchPoint(vec2(210, 5), vec2(210, 10), Menu::grey, Menu::defaultBorderThickness))),
			make_pair(1, shared_ptr<drawing::TouchPoint>(new drawing::TouchPoint(vec2(210, 15), vec2(210, 20), Menu::grey, Menu::defaultBorderThickness))),
			make_pair(1, shared_ptr<drawing::TouchPoint>(new drawing::TouchPoint(vec2(210, 25), vec2(210, 30), Menu::grey, Menu::defaultBorderThickness))),
			make_pair(1, shared_ptr<drawing::TouchPoint>(new drawing::TouchPoint(vec2(210, 35), vec2(210, 40), Menu::grey, Menu::defaultBorderThickness))),
			make_pair(1, shared_ptr<drawing::TouchPoint>(new drawing::TouchPoint(vec2(210, 45), vec2(210, 50), Menu::grey, Menu::defaultBorderThickness))),
			make_pair(1, shared_ptr<drawing::TouchPoint>(new drawing::TouchPoint(vec2(210, 55), vec2(210, 60), Menu::grey, Menu::defaultBorderThickness)))
		};
	}

	multimap<int, shared_ptr<drawing::TouchShape>> UserInterface::createBottomRightMenuShapes() const
	{
		auto backgroundRectangle = shared_ptr<drawing::TouchRectangle>
			(new drawing::TouchRectangle(windowWidth * .8, windowHeight * .8, windowWidth, windowHeight, 
				ColorA(0.82f, 0.82f, 0.82f, 1.0f), 0, true));
		auto outlineRectangle = shared_ptr<drawing::TouchRectangle>
			(new drawing::TouchRectangle(windowWidth * .8, windowHeight * .8, windowWidth, windowHeight,
				ColorA(0.75f, 0.75f, 0.75f, 1.0f), Menu::defaultBorderThickness, false));

		return multimap<int, shared_ptr<drawing::TouchShape>>
		{
			make_pair(0, backgroundRectangle),
			make_pair(1, outlineRectangle)
		};
	}

	multimap<int, shared_ptr<Menu>> UserInterface::createBottomRightComposingMenus() const
	{
		auto deviceModeButtonMenu = createDeviceModeButtonMenu();
		auto settingsButton = createSettingsButtonMenu();
		auto shapeDisplayMenu = createShapeDisplayMenu();
		
		return multimap<int, shared_ptr<Menu>>
		{
			make_pair(0, deviceModeButtonMenu),
			make_pair(0, settingsButton),
			make_pair(0, shapeDisplayMenu)
		};
	}

	shared_ptr<Menu> UserInterface::createDeviceModeButtonMenu() const
	{
		auto deviceModeButtonX1 = windowWidth * .92;
		auto deviceModeButtonY1 = windowHeight * .8;
		auto deviceModeButtonX2 = windowWidth;
		auto deviceModeButtonY2 = windowHeight * .83;
		auto deviceModeButtonStartPoint = vec2(deviceModeButtonX1, deviceModeButtonY1);
		auto deviceModeButtonWidth = windowWidth - windowWidth * .92;
		auto deviceModeButtonHeight = windowHeight * .83 - windowHeight * .8;

		auto deviceModeButtonBackgroundRectangle = shared_ptr<drawing::TouchRectangle>
			(new drawing::TouchRectangle(deviceModeButtonX1, deviceModeButtonY1, deviceModeButtonX2,
				deviceModeButtonY2, ColorA(0.1f, 0.1f, 0.1f, 1.0f), 0, true));
		auto deviceModeButtonText = shared_ptr<drawing::TouchTextLayout>(new drawing::TouchTextLayout(
			deviceModeButtonStartPoint, deviceModeButtonWidth, deviceModeButtonHeight,
			"Device Modes", "Arial", ColorA(0.2f, 0.2f, 0.2f, 0.2f), 50));
		auto deviceModeButtonBorder = shared_ptr<drawing::TouchRectangle>
			(new drawing::TouchRectangle(deviceModeButtonX1, deviceModeButtonY1, deviceModeButtonX2,
				deviceModeButtonY2, ColorA(0.75f, 0.75f, 0.75f, 1.0f), Menu::defaultBorderThickness, false));

		auto deviceModeButtonComposingShapes = multimap<int, shared_ptr<drawing::TouchShape>>
		{
			make_pair(0, deviceModeButtonBackgroundRectangle),
			make_pair(1, deviceModeButtonText),
			make_pair(1, deviceModeButtonBorder),
		};

		auto deviceModeButtonComposingMenus = createDevicesModeButtonComposingMenus();

		return shared_ptr<Menu>(new Menu(deviceModeButtonStartPoint,
			deviceModeButtonWidth, deviceModeButtonHeight, true, deviceModeButtonComposingShapes,
			deviceModeButtonComposingMenus, Menu::recursiveDropdownCallback));

	}

	shared_ptr<Menu> UserInterface::createSettingsButtonMenu() const
	{
		auto settingsButtonX1 = windowWidth * .8;
		auto settingsButtonY1 = windowHeight * .95;
		auto settingsButtonWidth = (windowWidth * .83) - (windowWidth * .8);
		auto settingsButtonHeight = windowHeight - (windowHeight * .95);
		auto settingsButtonStartPoint = vec2(settingsButtonX1, settingsButtonY1);
		auto settingsButtonImage = shared_ptr<drawing::TouchImage>
			(new drawing::TouchImage(settingsButtonStartPoint, settingsButtonWidth, settingsButtonHeight,
				"Settings.png"));
		auto settingsButtonShapes = multimap<int, shared_ptr<drawing::TouchShape>>
		{
			make_pair(0, settingsButtonImage)
		};

		auto settingsButtonMenus = createSettingsButtonComposingMenus();

		return shared_ptr<Menu>(new Menu(settingsButtonStartPoint,
			settingsButtonWidth, settingsButtonHeight, true, settingsButtonShapes,
			settingsButtonMenus, Menu::defaultDropdownCallback));
	}

	shared_ptr<Menu> UserInterface::createShapeDisplayMenu() const
	{
		auto startPoint = vec2(windowWidth * .88, windowHeight * .875);
		auto width = 65;
		auto height = 65;
		return shared_ptr<Menu>(new ShapeDisplayMenu(startPoint, width, height, true, mBrush));
	}

	multimap<int, shared_ptr<Menu>> UserInterface::createSettingsButtonComposingMenus() const
	{
		auto fpsX1 = windowWidth * .65;
		auto fpsY1 = windowHeight * .95;
		auto fpsX2 = windowWidth * .8;
		auto fpsY2 = windowHeight;
		auto fpsStartPoint = vec2(fpsX1, fpsY1);
		int fpsWidth = fpsX2 - fpsX1;
		int fpsHeight = fpsY2 - fpsY1;
		auto sharedUiPtr = make_shared<UserInterface>(*this);
		auto framesPerSecondBackgroundRectangle = shared_ptr<drawing::TouchRectangle>(new drawing::TouchRectangle(
			fpsX1, fpsY1, fpsX2, fpsY2, ColorA(0.0f, 0.0f, 0.0f, 1.0f), 0, true));
		auto framesPerSecondText = shared_ptr<drawing::TouchTextLayout>(new drawing::TouchTextLayout(fpsStartPoint, fpsWidth, fpsHeight, 
			"Frames Per Second", "Arial", ColorA(1.0f, 1.0f, 1.0f, 1.0f), 200));
		auto framesPerSecondShapes = multimap<int, shared_ptr<drawing::TouchShape>>
		{
			make_pair(0, framesPerSecondBackgroundRectangle),
			make_pair(1, framesPerSecondText)
		};
		auto framesPersecondMenu = shared_ptr<UserInterfaceModeSelectorMenu>(new UserInterfaceModeSelectorMenu(fpsStartPoint, fpsWidth, 
			fpsHeight, false, sharedUiPtr, framesPerSecondShapes,
			[](vec2 point, UserInterfaceModeSelectorMenu *self, shared_ptr<UserInterface> ui) { ui->ToggleFps(); }));

		auto bgX1 = windowWidth * .65;
		auto bgY1 = windowHeight * .9;
		auto bgX2 = windowWidth * .8;
		auto bgY2 = windowHeight * 0.95;
		auto bgStartPoint = vec2(bgX1, bgY1);
		int bgWidth = bgX2 - bgX1;
		int bgHeight = bgY2 - bgY1;
		auto backgroundColorBackgroundRectangle = shared_ptr<drawing::TouchRectangle>(new drawing::TouchRectangle(
			bgX1, bgY1, bgX2, bgY2, ColorA(0.0f, 0.0f, 0.0f, 1.0f), 0, true));
		auto backgroundColorText = shared_ptr<drawing::TouchTextLayout>(new drawing::TouchTextLayout(bgStartPoint, bgWidth, bgHeight,
			"Background Color", "Arial", ColorA(1.0f, 1.0f, 1.0f, 1.0f), 200));
		auto backgroundColorShapes = multimap<int, shared_ptr<drawing::TouchShape>>
		{
			make_pair(0, backgroundColorBackgroundRectangle),
			make_pair(1, backgroundColorText)
		};
		auto backgroundColorMenu = shared_ptr<IllustratorModeSelectorMenu>(new IllustratorModeSelectorMenu(bgStartPoint, bgWidth,
			bgHeight, false, illustrator, backgroundColorShapes, multimap<int, shared_ptr<Menu>> (),
			[](vec2 point, IllustratorModeSelectorMenu *self, drawing::Illustrator* illustrator) 
			{ illustrator->IncrementBackgroundColor(); }));
		
		return multimap<int, shared_ptr<Menu>>
		{
			make_pair(0, framesPersecondMenu),
			make_pair(0, backgroundColorMenu)
		};
	}

	multimap<int, shared_ptr<Menu>> UserInterface::createDevicesModeButtonComposingMenus() const
	{
		auto deviceToggleMenusX = windowWidth * .87;
		auto deviceToggleMenusY = windowHeight * .49;

		auto multiTouchToggleStartPoint = vec2(deviceToggleMenusX, deviceToggleMenusY);
		auto multiTouchToggleMenu = shared_ptr<DeviceHandlerModeSelectorMenu>(new DeviceHandlerModeSelectorMenu(
			multiTouchToggleStartPoint, false, "Multi Touch", deviceHandler, devices::Devices::MultiTouch));
		deviceToggleMenusY += DeviceHandlerModeSelectorMenu::defaultHeight;

		auto eyeXToggleStartPoint = vec2(deviceToggleMenusX, deviceToggleMenusY);
		auto eyeXToggleMenu = shared_ptr<DeviceHandlerModeSelectorMenu>(new DeviceHandlerModeSelectorMenu(
			eyeXToggleStartPoint, false, "EyeX", deviceHandler, devices::Devices::EyeX));
		deviceToggleMenusY += DeviceHandlerModeSelectorMenu::defaultHeight;

		auto leapMotionToggleStartPoint = vec2(deviceToggleMenusX, deviceToggleMenusY);
		auto leapMotionToggleMenu = shared_ptr<DeviceHandlerModeSelectorMenu>(new DeviceHandlerModeSelectorMenu(
			leapMotionToggleStartPoint, false, "Leap Motion", deviceHandler, devices::Devices::LeapMotion));
		deviceToggleMenusY += DeviceHandlerModeSelectorMenu::defaultHeight;

		auto leapDrawToggleStartPoint = vec2(deviceToggleMenusX, deviceToggleMenusY);
		auto leapDrawToggleMenu = shared_ptr<DeviceHandlerModeSelectorMenu>(new DeviceHandlerModeSelectorMenu(
			leapDrawToggleStartPoint, false, "Leap Draw", deviceHandler, devices::Devices::LeapDraw));
		deviceToggleMenusY += DeviceHandlerModeSelectorMenu::defaultHeight;

		auto leapGesturesToggleStartPoint = vec2(deviceToggleMenusX, deviceToggleMenusY);
		auto leapGesturesToggleMenu = shared_ptr<DeviceHandlerModeSelectorMenu>(new DeviceHandlerModeSelectorMenu(
			leapGesturesToggleStartPoint, false, "Leap Gestures", deviceHandler, devices::Devices::LeapGesture));
		deviceToggleMenusY += DeviceHandlerModeSelectorMenu::defaultHeight;

		auto realSenseToggleStartPoint = vec2(deviceToggleMenusX, deviceToggleMenusY);
		auto realSenseToggleMenu = shared_ptr<DeviceHandlerModeSelectorMenu>(new DeviceHandlerModeSelectorMenu(
			realSenseToggleStartPoint, false, "Real Sense", deviceHandler, devices::Devices::RealSense));
		deviceToggleMenusY += DeviceHandlerModeSelectorMenu::defaultHeight;

		auto realSenseDrawToggleStartPoint = vec2(deviceToggleMenusX, deviceToggleMenusY);
		auto realSenseDrawToggleMenu = shared_ptr<DeviceHandlerModeSelectorMenu>(new DeviceHandlerModeSelectorMenu(
			realSenseDrawToggleStartPoint, false, "Real Sense Draw", deviceHandler, devices::Devices::RealSenseDraw));
		deviceToggleMenusY += DeviceHandlerModeSelectorMenu::defaultHeight;

		auto realSenseExpressionsToggleStartPoint = vec2(deviceToggleMenusX, deviceToggleMenusY);
		auto realSenseExpressionsToggleMenu = shared_ptr<DeviceHandlerModeSelectorMenu>(new DeviceHandlerModeSelectorMenu(
			realSenseExpressionsToggleStartPoint, false, "Real Sense Expressions", deviceHandler, devices::Devices::RealSenseExpressions));

		return multimap<int, shared_ptr<Menu>>
		{
			make_pair(0, multiTouchToggleMenu),
			make_pair(0, eyeXToggleMenu),
			make_pair(0, leapMotionToggleMenu),
			make_pair(0, leapDrawToggleMenu),
			make_pair(0, leapGesturesToggleMenu),
			make_pair(0, realSenseToggleMenu),
			make_pair(0, realSenseDrawToggleMenu),
			make_pair(0, realSenseExpressionsToggleMenu)
		};
	}

	void UserInterface::setModeButtons(bool sModeButtons)
	{
		modeButtons = sModeButtons;
	}

	bool UserInterface::getModeButtons()
	{
		return modeButtons;
	}

	//todo remove
	//draws the shapes dropdown menu on the top left of application
	//todo get rid of this
	void UserInterface::drawShapesButtonsFbo()
	{
		shapeButtonsFbo->bindFramebuffer();

		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//draws menu outline for shapes in top left menu
		for (int i = 0; i < 5; i++)
		{
			gl::color(1.0f, 1.0f, 1.0f);
			gl::drawSolidRect(Rectf(50, 50 * i + 50, 100, 50 * i + 100));
			gl::color(0.75f, 0.75f, .75f, 1.0f);
			gl::drawStrokedRect(Rectf(50, 50 * (i) + 50, 100, 50 * i + 100), uiOutlineSize);
		}

		//draws the eraser icon in top left menu
		gl::TextureRef texture = gl::Texture::create(loadImage(loadAsset("Eraser.png")));
		gl::color(1.0, 1.0, 1.0, 1.0);
		gl::draw(texture, Rectf(55, 255, 95, 295));

		//draws line, circle, square, and triangle icon in the top left menu
		gl::color(mBrush->getColor());
		gl::lineWidth(2);
		gl::drawLine(vec2(55, 95), vec2(95, 55));

		//if getfilledshapes is selected, then the shapes icons menu will become filled.
		if (mBrush->getFilledShapes()) gl::drawSolidCircle(vec2(75, 125), 15);
		else gl::drawStrokedCircle(vec2(75, 125), 15);

		if (mBrush->getFilledShapes()) gl::drawSolidRect(Rectf(60, 188, 90, 160));
		else gl::drawStrokedRect(Rectf(60, 188, 90, 160));

		if (mBrush->getFilledShapes()) gl::drawSolidTriangle(vec2(55, 240), vec2(95, 240), vec2(73, 205));
		else
		{
			gl::drawLine(vec2(55, 240), vec2(95, 240));
			gl::drawLine(vec2(95, 240), vec2(73, 205));
			gl::drawLine(vec2(73, 205), vec2(55, 240));
		}

		shapeButtonsFbo->unbindFramebuffer();
	}


	//todo get rid of this
	//draws the menu for device modes on the bottom right
	void UserInterface::drawDeviceButtonsFbo()
	{
		deviceButtonsFbo->bindFramebuffer();

		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//draws the background of the different device modes in the bottom right
		gl::lineWidth(5);
		gl::color(0.0f, 0.0f, 0.0f, 1.0f);
		gl::drawSolidRect(Rectf(windowWidth * .9, windowHeight * .56, windowWidth, windowHeight * .8));
		gl::color(1.0f, 1.0f, 1.0f, 1.0f);

		//draws the names of the different device modes in the bottom right
		//draws multitouch menu
		TextLayout layout1;
		layout1.clear(ColorA(0.2f, 0.2f, 0.2f, 0.2f));
		layout1.setFont(Font("Arial", 50));
		layout1.setColor(Color(1, 1, 1));
		layout1.addLine(std::string(" MultiTouch"));
		Surface8u rendered = layout1.render(true, false);
		gl::Texture2dRef mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(windowWidth * .9, windowHeight * .77, windowWidth, windowHeight * .8));

		//draws eyex menu
		TextLayout layout2;
		layout2.clear(ColorA(0.2f, 0.2f, 0.2f, 0.2f));
		layout2.setFont(Font("Arial", 50));
		layout2.setColor(Color(1, 1, 1));
		layout2.addLine(std::string("       EyeX"));
		rendered = layout2.render(true, false);
		mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(windowWidth * .9, windowHeight * .74, windowWidth, windowHeight * .77));

		//draws Leap Gesture
		TextLayout layout3;
		layout3.clear(ColorA(0.2f, 0.2f, 0.2f, 0.2f));
		layout3.setFont(Font("Arial", 50));
		layout3.setColor(Color(1, 1, 1));
		layout3.addLine(std::string("Leap Gesture"));
		rendered = layout3.render(true, false);
		mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(windowWidth * .9, windowHeight * .71, windowWidth, windowHeight * .74));

		//draws Leap Draw
		TextLayout layout4;
		layout4.clear(ColorA(0.2f, 0.2f, 0.2f, 0.2f));
		layout4.setFont(Font("Arial", 50));
		layout4.setColor(Color(1, 1, 1));
		layout4.addLine(std::string("  Leap Draw"));
		rendered = layout4.render(true, false);
		mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(windowWidth * .9, windowHeight * .68, windowWidth, windowHeight * .71));

		//draws Leap Motion
		TextLayout layout5;
		layout5.clear(ColorA(0.2f, 0.2f, 0.2f, 0.2f));
		layout5.setFont(Font("Arial", 50));
		layout5.setColor(Color(1, 1, 1));
		layout5.addLine(std::string("Leap Motion"));
		rendered = layout5.render(true, false);
		mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(windowWidth * .9, windowHeight * .65, windowWidth, windowHeight * .68));

		//draws Real Sense Expressions
		TextLayout layout6;
		layout6.clear(ColorA(0.2f, 0.2f, 0.2f, 0.2f));
		layout6.setFont(Font("Arial", 50));
		layout6.setColor(Color(1, 1, 1));
		layout6.addLine(std::string("Real Sense Expressions"));
		rendered = layout6.render(true, false);
		mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(windowWidth * .9, windowHeight * .62, windowWidth, windowHeight * .65));

		//draws Real Sense Draw
		TextLayout layout7;
		layout7.clear(ColorA(0.2f, 0.2f, 0.2f, 0.2f));
		layout7.setFont(Font("Arial", 50));
		layout7.setColor(Color(1, 1, 1));
		layout7.addLine(std::string("Real Sense Draw"));
		rendered = layout7.render(true, false);
		mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(windowWidth * .9, windowHeight * .59, windowWidth, windowHeight * .62));

		//draws Real Sense
		TextLayout layout8;
		layout8.clear(ColorA(0.2f, 0.2f, 0.2f, 0.2f));
		layout8.setFont(Font("Arial", 50));
		layout8.setColor(Color(1, 1, 1));
		layout8.addLine(std::string(" Real Sense "));
		rendered = layout8.render(true, false);
		mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(windowWidth * .9, windowHeight * .56, windowWidth, windowHeight * .59));

		gl::color(0.0f, 0.0f, 0.0f);

		//draws the black/green squares for the Real Sense status, does not set the toggle touchpoints
		if (deviceHandler->realSenseStatus()) gl::color(0.0, 1.0, 0.0);
		else gl::color(0.0, 0.0, 0.0);
		gl::drawSolidRect(Rectf(windowWidth * .87, windowHeight * .56, windowWidth * .89, windowHeight * .59));

		//draws the black/green squares for the Real Sense Draw, does not set the toggle touchpoints
		if (deviceHandler->realSenseDrawStatus()) gl::color(0.0, 1.0, 0.0);
		else gl::color(0.0, 0.0, 0.0);
		gl::drawSolidRect(Rectf(windowWidth * .87, windowHeight * .59, windowWidth * .89, windowHeight * .62));

		//draws the black/green squares for the Real Sense Expressions, does not set the toggle touchpoints
		if (deviceHandler->realSenseExpressionsStatus()) gl::color(0.0, 1.0, 0.0);
		else gl::color(0.0, 0.0, 0.0);
		gl::drawSolidRect(Rectf(windowWidth * .87, windowHeight * .62, windowWidth * .89, windowHeight * .65));

		//draws the black/green squares for the Leap status, does not set the toggle touchpoints
		if (deviceHandler->leapStatus()) gl::color(0.0, 1.0, 0.0);
		else gl::color(0.0, 0.0, 0.0);
		gl::drawSolidRect(Rectf(windowWidth * .87, windowHeight * .65, windowWidth * .89, windowHeight * .68));

		//draws the black/green squares for the Leap Draw, does not set the toggle touchpoints
		if (deviceHandler->leapDrawStatus()) gl::color(0.0, 1.0, 0.0);
		else gl::color(0.0, 0.0, 0.0);
		gl::drawSolidRect(Rectf(windowWidth * .87, windowHeight * .68, windowWidth * .89, windowHeight * .71));

		//draws the black/green squares for the Leap Gesture, does not set the toggle touchpoints
		if (deviceHandler->leapGestureStatus()) gl::color(0.0, 1.0, 0.0);
		else gl::color(0.0, 0.0, 0.0);
		gl::drawSolidRect(Rectf(windowWidth * .87, windowHeight * .71, windowWidth * .89, windowHeight * .74));

		//draws the black/green squares for the eyeX status, does not set the toggle touchpoints
		if (deviceHandler->eyeXStatus()) gl::color(0.0, 1.0, 0.0);
		else gl::color(0.0, 0.0, 0.0);
		gl::drawSolidRect(Rectf(windowWidth * .87, windowHeight * .74, windowWidth * .89, windowHeight * .77));

		//draws the black/green squares for the Multitouch status, does not set the toggle touchpoints
		if (deviceHandler->multiTouchStatus()) gl::color(0.0, 1.0, 0.0);
		else gl::color(0.0, 0.0, 0.0);
		gl::drawSolidRect(Rectf(windowWidth * .87, windowHeight * .77, windowWidth * .89, windowHeight * .8));

		gl::color(0.75, 0.75, .75, 1.0);

		// draws horizontal grey lines in the bottom right device menu which separates the black/green square indicators
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .56, windowWidth * .89, windowHeight * .59));
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .59, windowWidth * .89, windowHeight * .62));
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .62, windowWidth * .89, windowHeight * .65));
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .65, windowWidth * .89, windowHeight * .68));
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .68, windowWidth * .89, windowHeight * .71));
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .71, windowWidth * .89, windowHeight * .74));
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .74, windowWidth * .89, windowHeight * .77));
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .77, windowWidth * .89, windowHeight * .8));

		//Not sure, draws something in the device menu
		gl::drawStrokedRect(Rectf(windowWidth * .87, windowHeight * .56, windowWidth * .89, windowHeight * .8), uiOutlineSize);

		deviceButtonsFbo->unbindFramebuffer();
	}

	//todo remove this
	//draws device button for top left menu,aka. size + -, Alpha(Transparency) + -, filled shapes
	void UserInterface::drawBrushButtonsFbo()
	{
		brushButtonsFbo->bindFramebuffer();

		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		/*LineSize Button*/
		gl::color(0.0, 0.0, 0.0, 1.0);
		gl::drawSolidRect(Rectf(100, 50, 200, 100));
		TextLayout layout1;
		layout1.clear(ColorA(0.0f, 0.0f, 0.0f, 0.0f));
		layout1.setFont(Font("Arial", 200));
		layout1.setColor(Color(1, 1, 1));
		layout1.addLine(std::string("Size"));
		Surface8u rendered = layout1.render(true, false);
		gl::Texture2dRef mTexture = gl::Texture2d::create(rendered);
		gl::color(Color::white());
		gl::draw(mTexture, Rectf(Rectf(100, 50, 200, 100)));

		gl::color(0.75, 0.75, .75, 1.0);
		gl::drawStrokedRect(Rectf(100, 50, 200, 100), uiOutlineSize);

		//Plus Button 
		gl::color(0.0, 0.0, 0.0, 1.0);
		gl::drawSolidRect(Rectf(200, 50, 250, 100));
		gl::TextureRef texture = gl::Texture::create(loadImage(loadAsset("BluePlus.png")));

		gl::color(1.0, 1.0, 1.0, 1.0);
		gl::draw(texture, Rectf(200, 50, 250, 100));

		gl::color(0.75, 0.75, .75, 1.0);
		gl::drawStrokedRect(Rectf(200, 50, 250, 100), uiOutlineSize);

		//Minus Button
		gl::color(0.0, 0.0, 0.0, 1.0);
		gl::drawSolidRect(Rectf(250, 50, 300, 100));

		texture = gl::Texture::create(loadImage(loadAsset("BlueMinus.png")));

		gl::color(1.0, 1.0, 1.0, 1.0);
		gl::draw(texture, Rectf(250, 50, 300, 100));

		gl::color(0.75, 0.75, .75, 1.0);
		gl::drawStrokedRect(Rectf(250, 50, 300, 100), uiOutlineSize);

		/*Transparency Button*/

		gl::color(0.0, 0.0, 0.0, 1.0);
		gl::drawSolidRect(Rectf(100, 100, 200, 150));
		TextLayout layout2;
		layout2.clear(ColorA(0.0f, 0.0f, 0.0f, 0.0f));
		layout2.setFont(Font("Arial", 200));
		layout2.setColor(Color(1, 1, 1));
		layout2.addLine(std::string("Alpha"));
		Surface8u rendered2 = layout2.render(true, false);
		gl::Texture2dRef mTexture2 = gl::Texture2d::create(rendered2);
		gl::color(Color::white());
		gl::draw(mTexture2, Rectf(Rectf(100, 100, 200, 150)));

		gl::color(0.75, 0.75, .75, 1.0);
		gl::drawStrokedRect(Rectf(100, 100, 200, 150), uiOutlineSize);

		//Plus Button 

		gl::color(0.0, 0.0, 0.0, 1.0);
		gl::drawSolidRect(Rectf(200, 100, 250, 150));

		texture = gl::Texture::create(loadImage(loadAsset("BluePlus.png")));

		gl::color(1.0, 1.0, 1.0, 1.0);
		gl::draw(texture, Rectf(200, 100, 250, 150));

		gl::color(0.75, 0.75, .75, 1.0);
		gl::drawStrokedRect(Rectf(200, 100, 250, 150), uiOutlineSize);

		//Minus Button

		gl::color(0.0, 0.0, 0.0, 1.0);
		gl::drawSolidRect(Rectf(250, 100, 300, 150));

		texture = gl::Texture::create(loadImage(loadAsset("BlueMinus.png")));

		gl::color(1.0, 1.0, 1.0, 1.0);
		gl::draw(texture, Rectf(250, 100, 300, 150));

		gl::color(0.75, 0.75, .75, 1.0);
		gl::drawStrokedRect(Rectf(250, 100, 300, 150), uiOutlineSize);

		/*Filled Shapes Button*/

		/*Fill Shapes*/
		gl::color(0.0, 0.0, 0.0, 1.0);
		gl::drawSolidRect(Rectf(100, 150, 200, 200));

		TextLayout layout3;
		layout3.clear(ColorA(0.0f, 0.0f, 0.0f, 0.0f));
		layout3.setFont(Font("Arial", 200));
		layout3.setColor(Color(1, 1, 1));
		layout3.addLine(std::string("Filled"));
		Surface8u rendered3 = layout3.render(true, false);
		gl::Texture2dRef mTexture3 = gl::Texture2d::create(rendered3);
		gl::color(Color::white());
		gl::draw(mTexture3, Rectf(Rectf(100, 150, 200, 200)));

		gl::color(0.75, 0.75, .75, 1.0);
		gl::drawStrokedRect(Rectf(100, 150, 200, 200), uiOutlineSize);

		//Enable and Disabled Button
		if (mBrush->getFilledShapes())
		{
			gl::color(0.0, 1.0, 0.0, 1.0);
			gl::drawSolidRect(Rectf(200, 150, 250, 200));
			gl::color(0.75, 0.75, .75, 1.0);
			gl::drawStrokedRect(Rectf(200, 150, 250, 200), uiOutlineSize);
		}
		else
		{
			gl::color(0.0, 0.0, 0.0, 1.0);
			gl::drawSolidRect(Rectf(200, 150, 250, 200));
			gl::color(0.75, 0.75, .75, 1.0);
			gl::drawStrokedRect(Rectf(200, 150, 250, 200), uiOutlineSize);
		}

		brushButtonsFbo->unbindFramebuffer();
	}

	bool UserInterface::getFps()
	{
		return FPS;
	}

	//used for moving the keyboard
	void UserInterface::endButtonPress(TouchEvent::Touch touch)
	{
		if (keyboard.getMovingKeyboard())
		{
			if (keyboard.checkMovingId(touch.getId())) keyboard.finishMoving();
		}
	}

	void UserInterface::uiSetup()
	{
		//Setup UI function. Sets up the UI, mainly Draw calls to write the buttons to frame buffer objects.
		//Errors occured when this function was called in TouchPointsApp 'setup', so it is now called in the update call.
		gl::Fbo::Format format;

		//Draws Settings menu FBO
		gl::color(1.0, 1.0, 1.0, 1.0);

		//todo get rid of this
		//settingsButtonsFbo.reset();
		settingsButtonsFbo = gl::Fbo::create(windowWidth, windowHeight, format);

		settingsButtonsFbo->bindFramebuffer();
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		settingsButtonsFbo->unbindFramebuffer();

		gl::color(1.0, 1.0, 1.0, 1.0);

		//Draws Device Buttons FBO
		//		deviceButtonsFbo.reset();
		deviceButtonsFbo = gl::Fbo::create(windowWidth, windowHeight, format);

		drawDeviceButtonsFbo();

		modeButtonsFbo = gl::Fbo::create(windowWidth, windowHeight, format);

		modeButtonsFbo->bindFramebuffer();

		modeButtonsFbo->unbindFramebuffer();

		gl::color(1.0, 1.0, 1.0, 1.0);
		//Loads the asset for transparent Background and writes it to the FBO.
		//		transparentBackgroundFbo.reset();
		//todo get rid of this
		transparentBackgroundFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		transparentBackgroundFbo->bindFramebuffer();
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		cinder::gl::TextureRef tempText = gl::Texture::create(loadImage(loadAsset("TransparentBackground.png")));
		gl::draw(tempText, Rectf(0, 0, windowWidth, windowHeight));
		transparentBackgroundFbo->unbindFramebuffer();
		gl::color(1.0, 1.0, 1.0, 0.5);
		gl::draw(transparentBackgroundFbo->getColorTexture());
		keyboard.createKeyboard();
	}

	//todo get rid of this
	std::shared_ptr<gl::Fbo> UserInterface::getTransparentBackground()
	{
		return transparentBackgroundFbo;
	}

	//todo get rid of this
	bool UserInterface::isBackgroundTransparent()
	{
		return transparentBackground;
	}

	//todo get rid of this
	void UserInterface::incrementBackground()
	{
		//Checks the current color
		Color tempColor = backgroundList.front();

		//If the background istransparent we disable it and change color
		if (transparentBackground)
		{
			transparentBackground = false;
			backgroundList.pop_front();
			//Assign the new background color to the new front of list.
			backgroundColor = backgroundList.front();
			//Place the old background color back into the list
			backgroundList.emplace_back(tempColor);
			return;
		}

		if (tempColor == Color(256.0, 256.0, 256.0))
		{
			transparentBackground = true;
			return;
		}
		backgroundList.pop_front();
		//Assign the new background color to the new front of list.
		changeBackgroundColor(backgroundList.front());
		//Place the old background color back into the list
		backgroundList.emplace_back(tempColor);
	}
	
	//todo get rid of this
	float UserInterface::getLayerAlpha(int layerNumber)
	{
		return (*layerAlpha)[layerNumber];
	}

	bool UserInterface::getUiFlag()
	{
		return uiFlag;
	}

	void UserInterface::toggleUiFlag()
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

	void UserInterface::changeModeButtons(bool x)
	{
		modeButtons = x;
	}

	bool UserInterface::getUiFboFlag()
	{
		return uiFboFlag;
	}

	void UserInterface::clearLayers()
	{
		for (auto layer : *layerList)
		{
			layer->bindFramebuffer();
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 0.0);

			glClear(GL_COLOR_BUFFER_BIT);
			layer->unbindFramebuffer();
		}
		illustrator->clearTimeMachine();
	}

	void UserInterface::setModeChangeFlag()
	{
		modeChangeFlag = true;
	}

	//todo refactor to use illustrator
	Color UserInterface::getBackgroundColor()
	{
		return backgroundColor;
	}

	//todo refactor to use illustrator
	void UserInterface::changeBackgroundColor(Color background)
	{
		backgroundColor = background;
	}

	void UserInterface::slideButtons(TouchEvent::Touch touch)
	{
		int x = touch.getX();
		int y = touch.getY();
		//todo get rid of this
		if (layerVisualization)
		{
			int yDist = (*layerList).size() * 200 + 50;
			int size = 0;
			auto index = illustrator->GetNumberOfLayersInCanvas() - 1;
			for (auto frame : (*layerList))
			{
				if (x > 200 && x < 250)
				{
					if (y > yDist - 200 && y < yDist)
					{
						auto newAlpha = ((y - ((float)yDist - 200)) / ((float)yDist - ((float)yDist - 200)));
						(*layerAlpha)[size] = newAlpha;
						illustrator->SetAlpha(index, newAlpha);
					}
				}
				yDist = yDist - 200;
				size = size + 1;
				index--;
			}
		}
		if (keyboard.keyboardStatus())
		{
			if (keyboard.getMovingKeyboard())
			{
				if (keyboard.checkMovingId(touch.getId()))
				{
					keyboard.setAnchor(vec2(x, y));
				}
			}
		}
	}

	bool UserInterface::inInteractiveUi(float x, float y, uint32_t id)
	{
		return menuLayer.OnTouch(vec2(x, y));

		//ONLY ON IF MULTITOUCH IS DISABLED!
		if (deviceHandler->multiTouchStatus() == false)
		{
			//If the multi-touch is 'disabled' we still allow the user to toggle devices on and off using the multitouch
			if (deviceButtons)
			{
				if (x < windowWidth && x > windowWidth * .87)
				{
					if (y > windowHeight * .56 && y < windowHeight * .59)
					{
						deviceHandler->toggleRealSense();
						drawDeviceButtonsFbo();
						modeChangeFlag = true;
						return true;
					}

					if (y > windowHeight * .59 && y < windowHeight * .62)
					{
						deviceHandler->toggleRealSenseDraw();
						drawDeviceButtonsFbo();
						modeChangeFlag = true;
						return true;
					}
					if (y > windowHeight * .62 && y < windowHeight * .65)
					{
						deviceHandler->toggleRealSenseExpressions();
						drawDeviceButtonsFbo();
						modeChangeFlag = true;
						return true;
					}
					if (y > windowHeight * .65 && y < windowHeight * .68)
					{
						deviceHandler->toggleLeap();
						drawDeviceButtonsFbo();
						modeChangeFlag = true;
						return true;
					}
					if (y > windowHeight * .68 && y < windowHeight * .71)
					{
						deviceHandler->toggleLeapDraw();
						drawDeviceButtonsFbo();
						return true;
					}
					if (y > windowHeight * .71 && y < windowHeight * .74)
					{
						deviceHandler->toggleLeapGesture();
						drawDeviceButtonsFbo();
						return true;
					}
					if (y > windowHeight * .74 && y < windowHeight * .77)
					{
						deviceHandler->toggleEyeX();
						drawDeviceButtonsFbo();
						if (deviceHandler->eyeXStatus() == false)
						{
							modeButtons = true;
						}
						modeChangeFlag = true;
						return true;
					}
					if (y > windowHeight * .77 && y < windowHeight * .8)
					{
						deviceHandler->toggleMultiTouch();
						drawDeviceButtonsFbo();
						modeChangeFlag = true;
						return true;
					}
				}
			}
			return true;
		}

		//modeButtons UI

		//Actual 'Interactive Ui'
		if (illustrator->getNumberOfActiveDrawings() == 0)
		{
			if (keyboard.getSettingText())
			{
				illustrator->saveCurrentFbo();
				keyboard.setTextAnchor(vec2(x, y));
				keyboard.turnOnKeyboard();
				return true;
			}

			//Checks keyboard input if its on
			if (keyboard.keyboardStatus())
			{
				//Checks if the user presses a keyboard key
				if (keyboard.onKeyboardButtons(vec2(x, y)))
				{
					//Checks if the keyboard is 'done' and needs to be cleaned up.
					if (keyboard.needsCleanup())
					{
						//Cleans up the keyboard by disabling it and deleting the string
						keyboard.endText();
						//Writes the keyboard text to the current Fbo.
						layerList->back()->bindFramebuffer();
						glClearColor(1.0, 1.0, 1.0, 0.0);
						glClear(GL_COLOR_BUFFER_BIT);

						gl::color(1.0, 1.0, 1.0, 1.0);
						gl::draw(keyboard.getTextFbo()->getColorTexture());
						layerList->back()->unbindFramebuffer();
					}
					return true;
				}
				//If the keyboard is on we want to check if the user is trying to drag it
				if (keyboard.onKeyboardSurface(vec2(x, y)))
				{
					bool tempBool = true;
					keyboard.setMovingKeyboard(tempBool);
					keyboard.setMovingId(id, vec2(x, y));
					return true;
				}

				//If they are clicking outside the keyboard they want to 'shut it down'
				//Finish cleaning up the keyboard.
				keyboard.endText();
				layerList->back()->bindFramebuffer();
				glClearColor(1.0, 1.0, 1.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT);

				gl::color(1.0, 1.0, 1.0, 1.0);
				gl::draw(keyboard.getTextFbo()->getColorTexture());
				layerList->back()->unbindFramebuffer();

				return true;
			}

			if (settingsButtons)
			{//Settings Buttons

				if (windowWidth * .65 < x && x < windowWidth * .8)
				{
					//FPS Buttons
					if (windowHeight * .9 < y && y < windowHeight * .95)
					{
						incrementBackground();
						return true;
					}

					//Cycle Background button
					if (windowHeight * .95 < y && y < windowHeight)
					{
						FPS = !FPS;
						return true;
					}
				}
			}
			if (uiFboFlag)
			{//Mode box FBO Flag.
				//Device Modes button
				if (x > windowWidth * .92 && x < windowWidth && y > windowHeight * .8 && y < windowHeight * .84)
				{
					deviceButtons = !deviceButtons;
					return true;
				}

				//Settings Button
				if (windowWidth * .8 < x && x < windowWidth * .83 && windowHeight * .95 < y && y < windowHeight)
				{
					settingsButtons = !settingsButtons;
					return true;
				}
			}

			//todo remove this
			if (modeButtons)
			{
				if (uiFboFlag)
				{
					//Color change button.
					if (x < 50 && y < 50)
					{
						colorButtons = !colorButtons;
						return true;
					}
					else if (x < 100 && y < 50)
					{
						shapeButtons = !shapeButtons;
						return true;
					}
					else if (x < 150 && y < 50)
					{
						//mBrush->changeFilledShapes(!mBrush->getFilledShapes());
						//modeChangeFlag = true;
						brushButtons = !brushButtons;
						return true;
					}
					else if (x < 200 && y < 50)
					{
						mBrush->getSymmetry()->toggleSymmetry();
						return true;
					}
					else if (x < 250 && y < 50)
					{
						layerVisualization = !layerVisualization;
						return true;
					}
					else if (x < 300 && y < 50)
					{
						keyboard.beginSettingTextAnchor();
						return true;
					}
					else if (x < 350 && y < 50)
					{
						//TODO: remove this once all devices use new drawing system
						illustrator->undoDraw(backgroundColor);
						illustrator->Undo();
						return true;
					}
				}
			}
		}
		//todo remove this
		//Color buttons UI
		if (colorButtons)
		{
			for (int i = 0; i < 8; i++)
			{ // i < total colors
				if (x < 50 && y < (50 * i + 100))
				{
					//currColor = i;
					mBrush->changeStaticColor(static_cast<drawing::ourColors>(i));
					colorButtons = false;
					drawShapesButtonsFbo();
					modeChangeFlag = true;
					return true;
				}
			}
		}

		//todo remove this
		if (layerVisualization)
		{
			int yDist = (*layerList).size() * 200 + 50;
			int size = 0;
			int index = illustrator->GetNumberOfLayersInCanvas() - 1;
			for (auto frame : (*layerList))
			{
				if (x > 200 && x < 600)
				{
					if (x < 250)
					{
						//Signals that the user is pressing layerVisualization 'slide button'
						return true;
					}
					if (y > yDist - 200 && y < yDist)
					{
						illustrator->MakeLayerActive(index);
						//Swaps layers
						std::shared_ptr<gl::Fbo> temp = (*layerList)[2];
						(*layerList)[2] = (*layerList)[size];
						(*layerList)[size] = temp;

						//Swaps alpha value of layers
						float tempAlpha = (*layerAlpha)[2];
						(*layerAlpha)[2] = (*layerAlpha)[size];
						(*layerAlpha)[size] = tempAlpha;

						return true;
					}
				}
				yDist = yDist - 200;
				size = size + 1;
				index--;
			}
		}

		//todo remove this
		if (brushButtons)
		{
			//Line Size Plus button
			if (x > 200 && x < 250 && y > 51 && y < 100)
			{
				mBrush->increaseLineSize();
				modeChangeFlag = true;
				return true;
			}
			//Line Size minus button
			if (x > 250 && x < 300 && y > 50 && y < 100)
			{
				mBrush->decreaseLineSize();
				modeChangeFlag = true;
				return true;
			}
			//Transparency Plus button
			if (x > 200 && x < 250 && y > 101 && y < 150)
			{
				mBrush->increaseAlpha();
				modeChangeFlag = true;
				return true;
			}
			//Transparency minus button
			if (x > 250 && x < 300 && y > 101 && y < 150)
			{
				mBrush->decreaseAlpha();
				modeChangeFlag = true;
				return true;
			}
			//Filled Shapes
			if (x > 200 && x < 250 && y > 151 && y < 200)
			{
				mBrush->changeFilledShapes(!mBrush->getFilledShapes());
				drawBrushButtonsFbo();
				drawShapesButtonsFbo();
				modeChangeFlag = true;
				return true;
			}
		}

		//todo remove this
		if (shapeButtons)
		{
			if (x > 50 && x < 100 && y < 100)
			{
				mBrush->changeShape(Shape::Shape::Line);
				shapeButtons = false;
				modeChangeFlag = true;
				return true;
			}
			if (x > 50 && x < 100 && y < 150)
			{
				mBrush->changeShape(Shape::Shape::Circle);
				shapeButtons = false;
				modeChangeFlag = true;
				return true;
			}
			if (x > 50 && x < 100 && y < 200)
			{
				mBrush->changeShape(Shape::Shape::Rectangle);
				shapeButtons = false;
				modeChangeFlag = true;
				return true;
			}
			if (x > 50 && x < 100 && y < 250)
			{
				mBrush->changeShape(Shape::Shape::Triangle);
				shapeButtons = false;
				modeChangeFlag = true;
				return true;
			}
			if (x > 50 && x < 100 && y < 300)
			{
				mBrush->changeShape(Shape::Shape::Eraser);
				shapeButtons = false;
				modeChangeFlag = true;
				return true;
			}
		}
		if (deviceButtons)
		{
			if (x < windowWidth && x > windowWidth * .87)
			{
				if (y > windowHeight * .56 && y < windowHeight * .59)
				{
					deviceHandler->toggleRealSense();
					drawDeviceButtonsFbo();
					modeChangeFlag = true;
					return true;
				}

				if (y > windowHeight * .59 && y < windowHeight * .62)
				{
					deviceHandler->toggleRealSenseDraw();
					drawDeviceButtonsFbo();
					modeChangeFlag = true;
					return true;
				}
				if (y > windowHeight * .62 && y < windowHeight * .65)
				{
					deviceHandler->toggleRealSenseExpressions();
					drawDeviceButtonsFbo();
					modeChangeFlag = true;
					return true;
				}
				if (y > windowHeight * .65 && y < windowHeight * .68)
				{
					deviceHandler->toggleLeap();
					drawDeviceButtonsFbo();
					modeChangeFlag = true;
					return true;
				}
				if (y > windowHeight * .68 && y < windowHeight * .71)
				{
					deviceHandler->toggleLeapDraw();
					drawDeviceButtonsFbo();
					return true;
				}
				if (y > windowHeight * .71 && y < windowHeight * .74)
				{
					deviceHandler->toggleLeapGesture();
					drawDeviceButtonsFbo();
					return true;
				}
				if (y > windowHeight * .74 && y < windowHeight * .77)
				{
					deviceHandler->toggleEyeX();
					drawDeviceButtonsFbo();
					if (deviceHandler->eyeXStatus() == false)
					{
						modeButtons = true;
					}
					modeChangeFlag = true;
					return true;
				}
				if (y > windowHeight * .77 && y < windowHeight * .8)
				{
					deviceHandler->toggleMultiTouch();
					drawDeviceButtonsFbo();
					modeChangeFlag = true;
					return true;
				}
			}
		}

		return false;
	}

	//draws the top left and bottom right menus
	void UserInterface::drawUi()
	{
		menuLayer.Draw();

		gl::color(1.0, 1.0, 1.0, 1.0);

		modeChangeFlag = true;
		//todo get rid of this
		//Draws to the UI FBO. Currently only houses 'modebox' in the fbo.
		if (modeChangeFlag)
		{
			modeChangeFlag = false;
			uiFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);

			uiFbo->unbindFramebuffer();
			//uiFboFlag = false;
		}
		if (uiFboFlag)
		{//Constantly drawn Ui buttons. If ui flag is off, we shut down the ui
			//Currently shuts down nothing because it needs to be developed in parralel to inInteractiveUi
		}
		//todo get rid of this
		//draws the settings menu, aka frames per sec and cycle background
		if (settingsButtons)
		{
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(settingsButtonsFbo->getColorTexture());
		}

		//todo get rid of this
		//draws the top left menu of different modes
		if (modeButtons)
		{
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(modeButtonsFbo->getColorTexture());
		}
		//todo get rid of this
		//draws the drop down menu of brush size, alpha and solid shapes
		if (brushButtons)
		{
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(brushButtonsFbo->getColorTexture());
		}

		//todo get rid of this
		//draws the color selection drop down menu
		if (colorButtons)
		{
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(colorButtonsFbo->getColorTexture());
		}
		
		//todo get rid of this
		//draws the drop down menu of shapes
		if (shapeButtons)
		{
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(shapeButtonsFbo->getColorTexture());
		}

		//todo get rid of this
		if (mBrush->getSymmetry()->getSymmetryOn())
		{
			for (int i = 0; i < 50; i = i + 2)
			{
				gl::lineWidth(4);
				gl::color(0.75, 0.75, 0.75);
				gl::drawLine(vec2(windowWidth / 2, windowHeight - i * 50), vec2(windowWidth / 2, windowHeight - (i + 1) * 50));
			}
		}
		
		//todo get rid of this
		if (layerVisualization)
		{
			int y = layerList->size();
			y = y * 200 + 50;
			int layerNumber = 0;
			int index = illustrator->GetNumberOfLayersInCanvas() - 1;
			for (auto frame : *layerList)
			{
				gl::color(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
				gl::drawSolidRect(Rectf(200, (y - 200), 600, y));

				gl::color(0.75, 0.75, .75, 1.0);
				gl::drawStrokedRect(Rectf(200, y - 200, 600, y), uiOutlineSize);
				//gl::color(1.0, 1.0, 1.0, (*layerAlpha)[layerNumber]);
				gl::color(1.0, 1.0, 1.0, (*layerAlpha)[layerNumber]);
				//gl::draw(frame->getColorTexture(), Rectf(200, (y - 200), 600, y));
				gl::draw(illustrator->GetLayerTexture(index), Rectf(200, (y - 200), 600, y));
				gl::color(0.0, 0.0, 0.0, 1.0);
				gl::drawSolidRect(Rectf(200, y - 200, 250, y));
				gl::color(0.75, 0.75, .75, 1.0);
				gl::drawStrokedRect(Rectf(200, y - 200, 250, y), uiOutlineSize);
				gl::color(1.0, 1.0, 1.0, 1.0);

				//Draws indicator for alpha level
				gl::color(0.0, 1.0f, 0.0, 1.0f);
				gl::drawSolidCircle(vec2(225, (y - 200) + (*layerAlpha)[layerNumber] * (y - (y - 200))), 5.0f);

				for (int x = 0; x < 8; x++)
				{
					gl::color(1.0, 1.0, 1.0, 1.0 - .125 * x);
					gl::drawSolidRect(Rectf(200, (y - 200) + x * 25, 250, (y - 200) + (x + 1) * 25));
				}
				y = y - 200;
				layerNumber++;
				index--;
			}
		}

		if (deviceButtons)
		{
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(deviceButtonsFbo->getColorTexture());
		}

		//Draws keyboard if it is on
		if (keyboard.keyboardStatus())
		{
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(keyboard.getKeyboardFbo()->getColorTexture());
			gl::draw(keyboard.getTextFbo()->getColorTexture());
		}
		gl::color(1.0, 1.0, 1.0, 1.0);
		if (keyboard.getMovingKeyboard())
		{
			gl::draw((keyboard.getMovingKeyboardFbo()->getColorTexture()));
		}
	}
}}
