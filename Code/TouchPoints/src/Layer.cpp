#include "Layer.h"
#include <cinder/gl/draw.h>
#include <cinder/gl/scoped.h>

namespace touchpoints { namespace drawing
{
	Layer::Layer() {}

	Layer::Layer(int windowWidth, int windowHeight, ourColors backgroundColor)
		: windowWidth(windowWidth), windowHeight(windowHeight), backgroundColor(backgroundColor)
	{
		framebuffer = gl::Fbo::create(windowWidth, windowHeight, format);
		initializeBackground();
	}

	void Layer::SetWindowWidth(int width)
	{
		windowWidth = width;
		resizeBackground(width, 0);
		resetFramebuffer();
	}

	void Layer::SetWindowHeight(int height)
	{
		windowHeight = height;
		resizeBackground(0, height);
		resetFramebuffer();
	}

	void Layer::SetWindowDimensions(int width, int height)
	{
		windowWidth = width;
		windowHeight = height;
		resizeBackground(width, height);
		resetFramebuffer();
	}

	void Layer::resetFramebuffer()
	{
		framebuffer.reset();
		
		framebuffer = gl::Fbo::create(windowWidth, windowHeight, format);
		framebuffer->bindFramebuffer();
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		framebuffer->unbindFramebuffer();
	}

	void Layer::AddDrawable(shared_ptr<IDrawable> drawable)
	{
		auto guid = guidGenerator.newGuid();
		drawablesStack.push_back(make_pair(drawable, guid));
	}

	void Layer::AddSymmetricDrawables(pair<shared_ptr<IDrawable>, shared_ptr<IDrawable>> symmetricDrawables)
	{
		auto guid = guidGenerator.newGuid();
		drawablesStack.push_back(make_pair(symmetricDrawables.first, guid));
		drawablesStack.push_back(make_pair(symmetricDrawables.second, guid));
	}

	void Layer::Draw()
	{
		drawSetup();

		if(backgroundColor == ourColors::Transparent)
		{
			transparentBackground.Draw();
		}
		else
		{
			background.Draw();
		}

		for(auto drawable: drawablesStack)
		{
			drawable.first->Draw();
		}

		drawTearDown();
	}

	void Layer::drawSetup() const
	{
		framebuffer->bindFramebuffer();
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
	}

	void Layer::drawTearDown() const
	{
		framebuffer->unbindFramebuffer();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		gl::color(1.0, 1.0, 1.0, alpha.GetValue());
		gl::draw(framebuffer->getColorTexture());
	}

	void Layer::Undo()
	{
		if (drawablesStack.size() > 0)
		{
			auto firstGuid = drawablesStack.back().second;
			drawablesStack.pop_back();
			if(drawablesStack.size() > 0)
			{
				auto secondGuid = drawablesStack.back().second;
				if (firstGuid == secondGuid)
				{
					//only remove this shape if it is a symmetric pair of the last shape
					drawablesStack.pop_back();
				}
			}
		}
	}

	void Layer::ClearLayer()
	{
		drawablesStack.clear();
		resetFramebuffer();
	}

	void Layer::setBackgroundColor(ourColors value)
	{
		backgroundColor = value; 
		if(backgroundColor != ourColors::Transparent)
		{
			background.setColor(ColorEnumToColorAMapper(value));
		}
	}

	void Layer::IncrementBackgroundColor()
	{
		setBackgroundColor(IncrementColorEnum(backgroundColor));
	}

	void Layer::initializeBackground()
	{
		auto x1 = 0;
		auto y1 = 0;
		auto x2 = windowWidth;
		auto y2 = windowHeight;
		background = TouchRectangle(x1, y1, x2, y2, ColorEnumToColorAMapper(backgroundColor), 0, true);
		transparentBackground = TouchImage(vec2(x1, y1), windowWidth, windowHeight, "TransparentBackground.png");
	}

	void Layer::resizeBackground(unsigned int width, unsigned int height)
	{
		background.Resize(width, height);
		transparentBackground.Resize(width, height);
	}
}}
