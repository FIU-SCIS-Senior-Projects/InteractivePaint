#pragma once
#include "Brush.h"
#include <cinder/gl/Fbo.h>
#include "Alpha.h"
#include <guid.h>

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
		void AddSymmetricDrawables(pair<shared_ptr<IDrawable>, shared_ptr<IDrawable>> symmetricDrawables);
		void Draw();
		void Undo();
		void ClearLayer();
		inline float GetAlpha() const { return alpha.GetValue(); }
		inline void SetAlpha(float value) { alpha.SetValue(value); }
		inline gl::Texture2dRef GetFrameBufferTexture() const { return framebuffer->getColorTexture(); }
	protected:
		int windowWidth;
		int windowHeight;
		//shared prt because this is the only way lib cinder will return an fbo
		shared_ptr<gl::Fbo> framebuffer;
		Alpha alpha;
		gl::Fbo::Format format;
		GuidGenerator guidGenerator;
		typedef pair<shared_ptr<IDrawable>, Guid> DrawableGuidPair;
		vector<DrawableGuidPair> drawablesStack;
		void drawSetup() const;
		void drawTearDown() const;
	private:
		void resetFramebuffer();
	};
}}
