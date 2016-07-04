#pragma once
#include "Brush.h"
#include <cinder/gl/Fbo.h>

namespace touchpoints { namespace drawing
{
	class Layer
	{
	public:
		Layer();
		Layer(int windowWidth, int windowHeight);
		void SetWindowWidth(int width);
		void SetWindowHeight(int height);
		void SetWindowDimensions(int width, int height);
		void AddDrawable(shared_ptr<IDrawable> drawable);
		void Draw();
		void Undo();
	private:
		int windowWidth;
		int windowHeight;
		//shared prt because this is the only way lib cinder will return an fbo
		shared_ptr<gl::Fbo> framebuffer;
		gl::Fbo::Format format;
		vector<shared_ptr<IDrawable>> drawablesStack;
		void resetFramebuffer();
	};
}}
