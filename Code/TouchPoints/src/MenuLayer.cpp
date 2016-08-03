#include "MenuLayer.h"

namespace touchpoints { namespace ui
{
	MenuLayer::MenuLayer() {}

	MenuLayer::MenuLayer(int windowWidth, int windowHeight) : Layer(windowWidth, windowHeight, drawing::ourColors::Transparent) {}

	void MenuLayer::OnTouch(vec2 point) const
	{
		for(auto menu : drawablesStack)
		{
			menu->OnTouch(point);
		}
	}
	void MenuLayer::AddMenu(shared_ptr<Menu> menu)
	{
		drawablesStack.push_back(menu);
	}
	
	void MenuLayer::SetWindowWidth(int width)
	{
		drawing::Layer::SetWindowWidth(width);
	}

	void MenuLayer::SetWindowHeight(int height)
	{
		drawing::Layer::SetWindowHeight(height);
	}

	void MenuLayer::SetWindowDimensions(int width, int height)
	{
		drawing::Layer::SetWindowDimensions(width, height);
	}

	void MenuLayer::Draw()
	{
		drawSetup();

		//background not drawn for MenuLayer

		for (auto drawable : drawablesStack)
		{
			drawable->Draw();
		}

		drawTearDown();
	}

	void MenuLayer::Undo()
	{
		drawing::Layer::Undo();
	}

	float MenuLayer::GetAlpha() const
	{
		return drawing::Layer::GetAlpha();
	}

	void MenuLayer::SetAlpha(float value)
	{
		drawing::Layer::SetAlpha(value);
	}

	gl::Texture2dRef MenuLayer::GetFrameBufferTexture() const
	{
		return drawing::Layer::GetFrameBufferTexture();
	}
}}