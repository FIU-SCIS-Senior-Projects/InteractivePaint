#include "TouchPointsApp.h"
#include <cinder/Utilities.h>

namespace touchpoints { namespace app
{
	void TouchPointsApp::prepareSettings(TouchPointsApp::Settings* settings)
	{

		settings->setResizable();
		DisplayRef display = Display::getMainDisplay();
		auto width = display->getWidth() - 10;
		auto height = display->getHeight() - 70;

		settings->setWindowSize(width, height);
		settings->setWindowPos(5, 30);
//		cinder::app::Window::setPos(width, height);

		settings->setTitle("InteractivePaint");

		// By default, multi-touch is disabled on desktop and enabled on mobile platforms.
		// You enable multi-touch from the SettingsFn that fires before the app is constructed.
		settings->setMultiTouchEnabled(true);
		// On mobile, if you disable multitouch then touch events will arrive via mouseDown(), mouseDrag(), etc.
	}

	void TouchPointsApp::setup()
	{
		CI_LOG_I("MT: " << System::hasMultiTouch() << " Max points: " << System::getMaxMultiTouchPoints());
		glEnable(GL_LINE_SMOOTH);
		
		gl::Fbo::Format format;
		firstFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		secondFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		thirdFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		activeFbo = gl::Fbo::create(windowWidth, windowHeight, format);

		iconFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		saveImageFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		//Set up UI
		uiFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		//Set up image feedback fbo
		imageFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		//Set up fbo for proxy menu
		radialFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		//Background FBO Testing
		backgroundFbo = gl::Fbo::create(windowWidth, windowHeight, format);
		//Set up fingerlocation FBo
		fingerLocationFbo = gl::Fbo::create(windowWidth, windowHeight, format);

		setFrameRate(FRAME_RATE);

		mySymmetry = drawing::SymmetryLine(windowWidth / 2, true);

		//Sets up layers
		layerList.emplace_back(firstFbo);
		layerList.emplace_back(secondFbo);
		layerList.emplace_back(thirdFbo);

		//Set up Brush
		ColorA newColor = ColorA(0.0f, 0.0f, 0.0f, 1.0f);
		float tempFloat = 1.0f;
		int tempInt = 1;
		Shape::Shape myShape = Shape::Shape::Line;
		brush = drawing::Brush(myShape, newColor, tempFloat, tempInt, false, false, false, &mySymmetry);
		illustrator = drawing::Illustrator(&brush, &layerList, windowWidth, windowHeight);
		deviceHandler = devices::DeviceHandler();
		getHomeDirectory();
		imageHandler = drawing::ImageHandler(&layerList, &layerAlpha);

		//RealSense Setup
		realSenseHandler = devices::RealSenseHandler(&illustrator);

		//Set up UI
		deviceHandler.deviceConnection();
		lastDeviceCheck = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		gui = ui::UserInterface(windowWidth, windowHeight, &brush, &illustrator, &deviceHandler, uiFbo, &layerList, &layerAlpha);

		leapMotionHandler = devices::LeapMotionHandler(windowWidth, windowHeight, &illustrator, &brush, &imageHandler, &gui);
		leapMotionHandler.InitLeapMotion();

		/*Kinect*/
		kinectHandler = devices::KinectHandler();

		deviceHandler.deviceConnection();
		Mode::DefaultModes resultMode = deviceHandler.getDefaultMode();
		setDefaultMode(resultMode);

		TX_CONNECTIONSTATE eyeXConnectedState = deviceHandler.GetEyeXConnected() ? TX_CONNECTIONSTATE_CONNECTED : TX_CONNECTIONSTATE_DISCONNECTED;
		eyeXHandler = devices::EyeXHandler(0.0f, 0.0f, eyeXConnectedState);
		eyeXHandler.InitEyeX();
	}

	void TouchPointsApp::drawRadial()
	{
		gl::color(1.0, 1.0, 1.0, 1.0);
		radialFbo->bindFramebuffer();
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		radialFbo->unbindFramebuffer();

		//Draw to radial menu buffer
		radialFbo->bindFramebuffer();
		//Make background transparent
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Center Circle
		gl::color(1.0f, 0.9f, 0.5f);
		gl::drawStrokedCircle(radialCenter, 30.0f, 2.0f);

		gl::color(.24f, .24f, .24f, 0.8f);

		gl::drawSolidCircle(radialCenter, 29.0f);

		//Draws outer ring
		gl::color(.24f, .24f, .24f, 0.8f);
		gl::drawStrokedCircle(radialCenter, 100.0f, 60.0f);

		gl::color(0.0f, 0.0f, 0.0f, 0.5f);
		gl::drawStrokedCircle(radialCenter, 131.0f, 2.0f, 300);

		gl::color(0.0f, 0.0f, 0.0f, 0.8f);
		gl::drawStrokedCircle(radialCenter, 69.0f, 2.0f, 300);

		//Draw left icon
		gl::color(1.0, 1.0, 1.0, 1.0);

		radialFbo->bindFramebuffer();

		//New way to Draw icons to FBO.
		gl::TextureRef texture = gl::Texture::create(loadImage(loadAsset("Colors.png")));

		gl::color(1.0, 1.0, 1.0, 1.0);

		gl::draw(texture, Rectf(radialCenter.x - 125, radialCenter.y - 25, radialCenter.x - 75, radialCenter.y + 25));

		texture = gl::Texture::create(loadImage(loadAsset("Shapes.png")));

		gl::color(1.0, 1.0, 1.0, 1.0);

		gl::draw(texture, Rectf(radialCenter.x + 75, radialCenter.y - 25, radialCenter.x + 125, radialCenter.y + 25));

		//Draw Upper Icon
		texture = gl::Texture::create(loadImage(loadAsset("Undo.png")));

		gl::color(1.0, 1.0, 1.0, 1.0);

		gl::draw(texture, Rectf(radialCenter.x - 25, radialCenter.y - 130, radialCenter.x + 25, radialCenter.y - 70));

		//Draw Lower Icon
		gl::color(1.0f, 0.9f, 0.5f);
		gl::drawStrokedCircle(vec2(radialCenter.x, radialCenter.y + 100), 30.0f, 2.0f);

		gl::color(1.0, 1.0, 1.0);
		gl::drawSolidCircle(vec2(radialCenter.x, radialCenter.y + 100), 29.0f);

		radialFbo->unbindFramebuffer();

		radialActive = true;
	}

	void TouchPointsApp::keyDown(KeyEvent event)
	{
		if (event.getChar() == 'z')
		{
			// Toggle full screen when the user presses the 'f' key.
			if (lineSize != 1.0f) lineSize--;

			brush.decreaseLineSize();
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'x')
		{
			// Clear the list of points when the user presses the space bar.
			if (lineSize != 15.0f) lineSize++;

			brush.increaseLineSize();
			gui.setModeChangeFlag();
		}
		else if (event.getCode() == KeyEvent::KEY_ESCAPE)
		{
			//Clears EyeX context then quits the program.
			eyeXHandler.EyeXTearDown();
			quit();
		}
		else if (event.getChar() == 'r') //Turns on random color mode
		{
			//Turns on random color mode
			brush.changeRandColor(!brush.getRandColor());

			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'q') //Cycles through colors
		{
			brush.decrementColor();
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'w') //Cycles through colors
		{
			brush.incrementColor();

			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 't') //Increase transparency / Decrease alpha
		{
			brush.decreaseAlpha();
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'y') //Decrease transparency/ increase alpha
		{
			brush.increaseAlpha();
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'e') //Eraser mode
		{
			brush.changeShape(Shape::Eraser);
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'c') //Clear Screen (Broken with framebuffers).
		{
			gui.clearLayers();
		}
		else if (event.getChar() == 'b')
		{
			gui.incrementBackground();
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'u')
		{
			brush.changeShape(Shape::Shape::Line);
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'i')
		{
			brush.changeShape(Shape::Shape::Circle);
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'f')
		{
			brush.changeFilledShapes(!brush.getFilledShapes());
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'o')
		{
			brush.changeShape(Shape::Shape::Rectangle);
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'p')
		{
			brush.changeShape(Shape::Shape::Triangle);
			gui.setModeChangeFlag();
		}
		else if (event.getChar() == 'n')
		{
			leapMotionHandler.leapSave();
		}
		else if (event.getChar() == 'j')
		{
			imageHandler.changeImageType("jpeg");
		}
		else if (event.getChar() == 't')
		{
			imageHandler.changeImageType("tif");
		}
		else if (event.getChar() == 'v')
		{
			leapDrawFlag = !leapDrawFlag;
		}
		else if (event.getChar() == 's')
		{
			mySymmetry.toggleSymmetry();
		}
		else if (event.getChar() == 'a')
		{
			drawRadial();
		}
		else if (event.getChar() == 'g')
		{
			gui.toggleUiFlag();
		}
		else if (event.getChar() == ' ')
		{
			if (deviceHandler.eyeXStatus())
			{
				if (eyeXHandler.GetGazePositionX() <= windowWidth / 2
					&& eyeXHandler.GetGazePositionY() <= windowHeight / 2)
				{
					brush.changeShape(Shape::Shape::Line);
					gui.setModeChangeFlag();
					imageHandler.loadIcon(SHAPE_LINE);
				}
				else if (eyeXHandler.GetGazePositionX() >= windowWidth / 2
					&& eyeXHandler.GetGazePositionY() <= windowHeight / 2)
				{
					brush.changeShape(Shape::Shape::Circle);
					gui.setModeChangeFlag();
					imageHandler.loadIcon(SHAPE_Circle);
				}
				else if (eyeXHandler.GetGazePositionX() <= windowWidth / 2
					&& eyeXHandler.GetGazePositionY() >= windowHeight / 2)
				{
					brush.changeShape(Shape::Shape::Rectangle);
					gui.setModeChangeFlag();
					imageHandler.loadIcon(SHAPE_Rectangle);
				}
				else if (eyeXHandler.GetGazePositionX() >= windowWidth / 2
					&& eyeXHandler.GetGazePositionY() >= windowHeight / 2)
				{
					brush.changeShape(Shape::Shape::Triangle);
					gui.setModeChangeFlag();
					imageHandler.loadIcon(SHAPE_Triangle);
				}
			}
		}
		else if (event.getChar() == 'l')
		{
			leapMotionHandler.leapShapeChange();
		}
		else if (event.getChar() == 'k')
		{
			leapMotionHandler.leapColorChange();
		}
		else if (event.getChar() == '1')
		{
			leapMotionHandler.leapShapeChange();
		}
		else if (event.getChar() == '2')
		{
			//TODO: remove this once all devices use new drawing system
			illustrator.undoDraw(gui.getBackgroundColor());
			illustrator.Undo();
		}
		else if (event.getChar() == '3')
		{
			leapMotionHandler.leapColorChange();
		}
		else if (event.getChar() == '4')
		{
			mySymmetry.toggleSymmetry();
		}
		else if (event.getChar() == '5')
		{
			gui.toggleUiFlag();
		}
	}

	bool TouchPointsApp::findMultiTouchGestures(TouchEvent::Touch previousPoint, TouchEvent::Touch currentPoint)
	{
		//Checks for double tap
		//Detects double tap.
		if (0.25 > currentPoint.getTime() - previousPoint.getTime())
		{
			if (previousPoint.getX() < currentPoint.getX() + 20 && previousPoint.getX() > currentPoint.getX() - 20)
			{
				if (previousPoint.getY() < currentPoint.getY() + 20 && previousPoint.getY() > currentPoint.getY() - 20)
				{
					radialCenter = currentPoint.getPos();

					drawRadial();
					radialActive = true;
					bufferTouches.erase(previousPoint.getId());
					bufferTouches.erase(currentPoint.getId());
					return true;
				}
			}
		}
		//'Two finger tap' gesture
		if (0.05 > currentPoint.getTime() - previousPoint.getTime())
		{
			if (((previousPoint.getX() > currentPoint.getX() + 20) && previousPoint.getX() < currentPoint.getX() + 75) || (previousPoint.getX() < currentPoint.getX() - 20 && previousPoint.getX() > currentPoint.getX() - 75))
			{
				if ((previousPoint.getY() < currentPoint.getY() + 120) && (previousPoint.getY() > currentPoint.getY() - 120))
				{
					leapMotionHandler.leapColorChange();
					bufferTouches.erase(previousPoint.getId());
					bufferTouches.erase(currentPoint.getId());
					return true;
				}
			}
		}
		return false;
	}

	void TouchPointsApp::touchesBegan(TouchEvent event)
	{
		for (const auto& touch : event.getTouches())
		{
			if (!deviceHandler.multiTouchStatus())
			{
				gui.inInteractiveUi(touch.getX(), touch.getY(), touch.getId());
				return;
			}

			if (radialActive)
			{
				float x = touch.getX();
				float y = touch.getY();
				if ((((radialCenter.x - 100) - 30) < x && x < ((radialCenter.x - 100) + 30)) && ((radialCenter.y - 30) < y && y < (radialCenter.y + 30)))
				{
					//brush.changeStaticColor();
					leapMotionHandler.leapColorChange();
					return;
				}

				if ((((radialCenter.x + 100) - 30) < x && x < ((radialCenter.x + 100) + 30)) && ((radialCenter.y - 30) < y && y < (radialCenter.y + 30)))
				{
					//brush.cycleShape();
					leapMotionHandler.leapShapeChange();
					return;
				}

				if ((((radialCenter.x) - 30) < x && x < ((radialCenter.x) + 30)) && ((radialCenter.y - 100) - 30) < y && y < (radialCenter.y - 100) + 30)
				{
					//mySymmetry.toggleSymmetry();
					//TODO: remove this once all devices use new drawing system
					illustrator.undoDraw(gui.getBackgroundColor());
					illustrator.Undo();
					return;
				}

				if ((((radialCenter.x) - 30) < x && x < ((radialCenter.x) + 30)) && ((radialCenter.y + 100) - 30) < y && y < (radialCenter.y + 100) + 30)
				{
					gui.toggleUiFlag();
					return;
				}

				if ((radialCenter.x - 30) < x && x < ((radialCenter.x + 30)) && ((radialCenter.y - 30) < y && y < (radialCenter.y + 30)))
				{
					radialActive = false;
					return;
				}
			}
			if (gui.inInteractiveUi(touch.getX(), touch.getY(), touch.getId()))
			{
				return;
			}
			else
			{
				list<TouchEvent::Touch> tempList;
				tempList.push_back(touch);
				bufferTouches.emplace(touch.getId(), tempList);

				if (!findMultiTouchGestures(previousTouch, touch))
				{
					//If we didn't find a gesture this time, save the point incase.
					previousTouch = touch;
				}
				else
				{
					//Clears the previous touch if we found a multitouch gesture.
					previousTouch.setPos(vec2(-100, -100));
				}
			}
		}
	}

	void TouchPointsApp::touchesMoved(TouchEvent event)
	{
		if (!deviceHandler.multiTouchStatus()) return;

		for (const auto& touch : event.getTouches())
		{
			gui.slideButtons(touch);
			if (bufferTouches.find(touch.getId()) == bufferTouches.end())
			{
				illustrator.movingTouchShapes(touch.getId(), touch.getPos(), touch.getPrevPos());
			}
			else
			{
				TouchEvent::Touch myTouch = bufferTouches[touch.getId()].front();

				//'Extended Touch' Gesture
				if (touch.getPos() == touch.getPrevPos() && touch.getTime() > myTouch.getTime() + .75)
				{
					leapMotionHandler.leapShapeChange();
					bufferTouches[touch.getId()].clear();
					bufferTouches[touch.getId()].emplace_front(touch);
				}
				else
				//On finger movement, we know it is no longer a gesture. 
				//So we remove it from 'buffer' touches and begin drawing
				if ((touch.getPos().x > myTouch.getX() + 25 || touch.getX() < myTouch.getX() - 25) || (touch.getY() > myTouch.getY() + 25 || touch.getY() < myTouch.getY() - 25))
				{
					illustrator.beginTouchShapes(touch.getId(), bufferTouches[touch.getId()].front().getPos());
					for (auto bufferTouch : bufferTouches[touch.getId()])
					{
						illustrator.movingTouchShapes(bufferTouch.getId(), bufferTouch.getPos(), bufferTouch.getPrevPos());
					}
					illustrator.movingTouchShapes(touch.getId(), touch.getPos(), touch.getPrevPos());
					bufferTouches.erase(touch.getId());
				}
				else
				{
					bufferTouches[touch.getId()].emplace_back(touch);
				}
			}
		}
	}

	void TouchPointsApp::touchesEnded(TouchEvent event)
	{
		if (!deviceHandler.multiTouchStatus()) return;
		for (const auto& touch : event.getTouches())
		{
			bufferTouches.erase(touch.getId());
			illustrator.endTouchShapes(touch.getId());
			gui.endButtonPress(touch);
		}
	}

	void TouchPointsApp::setDefaultMode(Mode::DefaultModes mode)
	{
		switch (mode)
		{
			case Mode::DefaultModes::MLER:
				gui.changeModeButtons(true);
				break; 
			case Mode::DefaultModes::MLE:
				gui.changeModeButtons(true);
				break;
			case Mode::DefaultModes::MLR:
				gui.changeModeButtons(true);
				break;
			case Mode::DefaultModes::MER:
				gui.changeModeButtons(true);
				break;
			case Mode::DefaultModes::LER:
				gui.changeModeButtons(false);
				break;
			case Mode::DefaultModes::ML:
				gui.changeModeButtons(true);
				break;
			case Mode::DefaultModes::ME:
				gui.changeModeButtons(true);
				break;
			case Mode::DefaultModes::MR:
				gui.changeModeButtons(true);
				break;
			case Mode::DefaultModes::LE:
				gui.changeModeButtons(false);
				break;
			case Mode::DefaultModes::LR:
				gui.changeModeButtons(false);
				break;
			case Mode::DefaultModes::ER:
				gui.changeModeButtons(false);
				break;
			case Mode::DefaultModes::M:
				gui.changeModeButtons(true);
				break;
			case Mode::DefaultModes::L:
				gui.changeModeButtons(false);
				break;
			case Mode::DefaultModes::E:
				gui.changeModeButtons(false);
				break;
			case Mode::DefaultModes::R:
				gui.changeModeButtons(false);
				break;
		}
	}

	void TouchPointsApp::update()
	{
		if (!setupComplete)
		{
			gui.uiSetup();
			realSenseHandler.realSenseSetup();
			setupComplete = true;
		}
		/*Kinect*/
		kinectHandler.Update(illustrator);

		//Increment the frame counter
		frames++;

		//Gets current clock time in milliseconds
		std::chrono::milliseconds currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		//This is how often we will check our device connection in milliseconds.
		std::chrono::milliseconds checkTime{2000};
		if (currentTime - lastDeviceCheck >= checkTime)
		{
			//Updates FPS - Ensure framesDrawn is being divided by how many seconds it takes to get into this loop.
			fps = frames / 2;
			frames = 0;

			//Resets our lastDeviceCheck time.
			lastDeviceCheck = currentTime;

			//Checks if any device statuses have changed.
			if (deviceHandler.deviceConnection())
			{
				gui.drawDeviceButtonsFbo();
				gui.setModeChangeFlag();
			}
			//Checks to see if default mode needs to be reset
			if (deviceHandler.getUpdateDefaultFlag())
			{
				deviceHandler.updateDefaultMode();
				Mode::DefaultModes resultMode = deviceHandler.getDefaultMode();
				setDefaultMode(resultMode);
			}
		}

		//Checks the real sense device and updates anything associated with it.
		if (deviceHandler.realSenseStatus())
		{
			//Checks if expressions is turned on.
			if (deviceHandler.realSenseExpressions())
			{
				if (illustrator.getNumberOfActiveDrawings() == 0)
				{
					realSenseHandler.streamData();

					if (realSenseHandler.getBrowGestureFlag())
					{
						//TODO: remove this once all devices use new drawing system
						illustrator.undoDraw(gui.getBackgroundColor());
						illustrator.Undo();
					}
					if (realSenseHandler.getKissGestureFlag())
					{
						gui.toggleUiFlag();
					}
					if (realSenseHandler.getTongueGestureFlag())
					{
						mySymmetry.toggleSymmetry();
					}
					if (realSenseHandler.getCheekGestureFlag())
					{
						leapMotionHandler.leapColorChange();
					}
					if (realSenseHandler.getSmileGestureFlag())
					{
						leapMotionHandler.leapShapeChange();
					}
					realSenseHandler.resetGesturesFlag();
				}
			}

			//Real Sense Draw
			if (deviceHandler.realSenseDraw())
			{
				//Draw functions for real sense.
				realSenseHandler.streamCursorData();
				realSenseHandler.realSenseDraw(fingerLocationFbo);
			}
		}

		//Updates the active shapes being drawn by the user
		gl::color(1.0, 1.0, 1.0, 1.0);
		activeFbo->bindFramebuffer();
		//Clears Previous Active Shapes
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		//Draws all active Shapes (non-permanent);
		illustrator.drawActiveShapes();
		activeFbo->unbindFramebuffer();

		illustrator.Update();
	}

	void TouchPointsApp::draw()
	{
		//Clears the Application Context to the background color with a 0 Alpha Value (transparent)
		Color myBG = gui.getBackgroundColor();
		glClearColor(myBG.r, myBG.g, myBG.b, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//Currently ProcessDrawInput before drawing layers to prevent flickering. 
		//However, this makes it impossible to see green 'hands' on top of images.
		if (deviceHandler.leapStatus())
		{
			leapMotionHandler.SetCurrentFrame();
			if (deviceHandler.leapDraw())
			{
				if (leapDrawFlag)
				{
					leapMotionHandler.ProcessDrawInput(lockCurrentFrame);
				}
			}
			if (deviceHandler.leapGesture())
			{
				if (!lockCurrentFrame)
				{
					//Calls specified action from gesture recgonized
					leapMotionHandler.gestRecognition();
				}
			}
			lockCurrentFrame = false;
		}

		//Draw Checkerboard pattern if background is transparent
		if (gui.isBackgroundTransparent())
		{
			gl::color(.5, .5, .5, 1.0);
			gl::draw(gui.getTransparentBackground()->getColorTexture());
		}
		//Loop Which Draws our Layers
		int x = 0;
		for (auto frames : layerList)
		{
			gl::color(1.0, 1.0, 1.0, gui.getLayerAlpha(x));
			gl::draw(frames->getColorTexture());
			x++;
		}
		
		if (imageHandler.getStartUpFlag())
		{
			imageHandler.loadStartIcon(START_LOGO);
		}
		
		//Draws icons that provides feedback
		imageHandler.displayIcon();
		imageHandler.displayStartIcon();
		
		//Draws all the UI elements (Currently only updated the uiFbo which stores data for the mode box), drawing is done below.
		gui.drawUi();
		
		//Draws radial menu
		if (radialActive)
		{
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(radialFbo->getColorTexture());
		}

		gl::color(1.0, 1.0, 1.0, 1.0);
		
		//Draws the frame buffer for UI
		if (deviceHandler.eyeXStatus())
		{
			gl::color(0.0f, 0.0f, 0.0f, 0.4f);

			vec2 gaze1(eyeXHandler.GetGazePositionX() - 10, eyeXHandler.GetGazePositionY());
			vec2 gaze2(eyeXHandler.GetGazePositionX() + 10, eyeXHandler.GetGazePositionY());

			gl::drawStrokedCircle(gaze1, 10.0f, 10.0f);
			gl::drawStrokedCircle(gaze2, 10.0f, 10.0f);

			gl::color(1.0, 1.0, 1.0, 1.0);
			if (eyeXHandler.GetGazePositionX() < 500 && eyeXHandler.GetGazePositionY() < 100)
			{
				bool tempBool = true;
				gui.changeModeButtons(tempBool);
			}
			else
			{
				bool tempBool = false;
				gui.changeModeButtons(tempBool);
			}

			if (eyeXHandler.GetGazePositionX() > windowWidth * .75 && eyeXHandler.GetGazePositionY() > windowHeight * .6)
			{
				gl::draw(uiFbo->getColorTexture());
			}
		}
		else if (gui.getUiFboFlag()) gl::draw(uiFbo->getColorTexture());

		//Draws all the active shapes being drawn by the user
		gl::color(1.0, 1.0, 1.0, 1.0);
		gl::draw(activeFbo->getColorTexture());

		if (gui.getFps())
		{
			auto mFont = Font("Quicksand Book Regular", 36.0f);
			gl::drawString("Framerate: " + toString(static_cast<int>(getAverageFps())), vec2(windowWidth * .90, windowHeight * .01), ColorA(0.0, 0.0, 0.0, 1.0), mFont);
		}

		if (realSenseHandler.getHoverFlag())
		{
			//Draws finger location
			gl::color(1.0, 1.0, 1.0, 1.0);
			gl::draw(fingerLocationFbo->getColorTexture());
		}

		illustrator.Draw();
	}

	void TouchPointsApp::resize()
	{
		if (resizeCount > 0)
		{
			//From TouchPointsApp.cpp
			windowWidth = getWindowSize().x;
			windowHeight = getWindowSize().y;
			if (windowWidth == 0 || windowHeight == 0)
			{
				return;
			}

			radialCenter = vec2(windowWidth * .5, windowHeight * .5);

//			//Fbo for UserInterface
			//add bind, clear, and unbind for all fbos created here to test if it clears buffers
			gl::Fbo::Format format;
			firstFbo.reset();
			secondFbo.reset();
			thirdFbo.reset();
			activeFbo.reset();

			firstFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			firstFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			firstFbo->unbindFramebuffer();

			secondFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			secondFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			secondFbo->unbindFramebuffer();

			thirdFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			thirdFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			thirdFbo->unbindFramebuffer();

			activeFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			activeFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			activeFbo->unbindFramebuffer();

			layerList[0];
//			firstFbo->mHeight = windowHeight;

			iconFbo.reset();
			saveImageFbo.reset();
			iconFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			iconFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			iconFbo->unbindFramebuffer();

			saveImageFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			saveImageFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			saveImageFbo->unbindFramebuffer();


			//Set up UI
			uiFbo.reset();
			uiFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			uiFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			uiFbo->unbindFramebuffer();

			//Set up image feedback fbo
			imageFbo.reset();
			imageFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			imageFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			imageFbo->unbindFramebuffer();

			//Set up fbo for proxy menu
			radialFbo.reset();
			radialFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			radialFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			radialFbo->unbindFramebuffer();

			//Background FBO Testing
			backgroundFbo.reset();
			backgroundFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			backgroundFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			backgroundFbo->unbindFramebuffer();

			//Set up fingerlocation FBo
			fingerLocationFbo.reset();
			fingerLocationFbo = gl::Fbo::create(windowWidth, windowHeight, format);
			fingerLocationFbo->bindFramebuffer();
			glClearColor(1.0, 1.0, 1.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);
			fingerLocationFbo->unbindFramebuffer();

			setFrameRate(FRAME_RATE);

			mySymmetry.setSymmetryWindowWidth(windowWidth / 2, true);

			//Sets up layers
			layerList.clear();



			layerList.emplace_back(firstFbo);
			layerList.emplace_back(secondFbo);
			layerList.emplace_back(thirdFbo);

			//create a setter for brush
			brush.setSymmetry(&mySymmetry);

			//create a setter for illustrator
			illustrator.setIllustratorResize(&brush, &layerList);
			getHomeDirectory();

			//create a setter for ImageHandler
			imageHandler.setImageHandlerResize(&layerList);

			//RealSense Setup
			//create a setter for RealSenseHandler
			realSenseHandler.setRealSenseResize(&illustrator);


			//create a setter for windowWidth, windowHeight, &brush, &illustrator, uiFbo, &layerList
			gui.setUIResize(windowWidth, windowHeight, &brush, &illustrator, uiFbo, &layerList);


			//create a setter for leapMotionHandler
			leapMotionHandler.setLeapMotionResize(windowWidth, windowHeight);
		}
		resizeCount++;
	}
}}
