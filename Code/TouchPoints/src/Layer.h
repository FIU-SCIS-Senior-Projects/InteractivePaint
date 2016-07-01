#pragma once
#include "Brush.h"
#include <cinder/gl/Fbo.h>

namespace touchpoints { namespace drawing
{
	class Layer
	{
	public:
		Layer(int windowWidth, int windowHeight, Brush &brush);
		void SetWindowWidth(int width);
		void SetWindowHeight(int height);
		void SetWindowDemensions(int width, int height);
		void AddShape(shared_ptr<TouchShape> shape);
		void Draw();
		void Undo();
	private:
		int windowWidth;
		int windowHeight;
		//shared prt because this is the only way lib cinder will return an fbo
		shared_ptr<gl::Fbo> framebuffer;
		gl::Fbo::Format format;
		vector<shared_ptr<TouchShape>> shapesStack;
		Brush brush;

		void resetFramebuffer();
	};
}}
