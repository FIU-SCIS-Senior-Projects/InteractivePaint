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

	void Layer::AddShape(shared_ptr<TouchShape> shape)
	{
		shapesStack.push_back(shape);
	}

	void Layer::Draw()
	{
		framebuffer->bindFramebuffer();
		

		for(auto shape: shapesStack)
		{
			shape->draw();
		}

		framebuffer->unbindFramebuffer();

		gl::draw(framebuffer->getColorTexture());
	}

	void Layer::Undo()
	{
		shapesStack.pop_back();
	}
}}
