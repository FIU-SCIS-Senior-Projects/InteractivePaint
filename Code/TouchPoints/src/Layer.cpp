#include "Layer.h"
#include <cinder/gl/draw.h>

namespace touchpoints { namespace drawing
{
	Layer::Layer() {}

	Layer::Layer(int windowWidth, int windowHeight) 
		: windowWidth(windowWidth), windowHeight(windowHeight)
	{
		framebuffer = gl::Fbo::create(windowWidth, windowHeight, format);
	}

	void Layer::SetWindowWidth(int width)
	{
		windowWidth = width;
		resetFramebuffer();
	}

	void Layer::SetWindowHeight(int height)
	{
		windowHeight = height;
		resetFramebuffer();
	}

	void Layer::SetWindowDimensions(int width, int height)
	{
		windowWidth = width;
		windowHeight = height;
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
		drawablesStack.push_back(drawable);
	}

	void Layer::Draw()
	{
		framebuffer->bindFramebuffer();
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		for(auto drawable: drawablesStack)
		{
			drawable->Draw();
		}

		framebuffer->unbindFramebuffer();

		gl::draw(framebuffer->getColorTexture());
	}

	void Layer::Undo()
	{
		if (drawablesStack.size() > 0)
		{
			drawablesStack.pop_back();
		}
	}
}}
