#include "Canvas.h"
#include <cassert>

namespace touchpoints { namespace drawing
{
	Canvas::Canvas() {}

	Canvas::Canvas(int windowWidth, int windowHeight, int numberOfLayers)
		: numberOfLayers(numberOfLayers), activeLayerIndex(0)
	{
		layerIndexOrder = vector<int>(numberOfLayers);
		layers = vector<Layer>(numberOfLayers);
		for (int i = 0; i < numberOfLayers; i++)
		{
			layerIndexOrder[i] = i;
			layers[i] = Layer(windowWidth, windowHeight);
		}
	}

	void Canvas::SetWindowWidth(int width)
	{
		for (auto layer : layers)
		{
			layer.SetWindowWidth(width);
		}
	}

	void Canvas::SetWindowHeight(int height)
	{
		for (auto layer : layers)
		{
			layer.SetWindowHeight(height);
		}
	}

	void Canvas::SetWindowDimensions(int width, int height)
	{
		for (auto layer : layers)
		{
			layer.SetWindowDimensions(width, height);
		}
	}

	void Canvas::MakeLayerActive(int index)
	{
		assert(index >= 0 && index < numberOfLayers, "Cannot make layer active, index out of range");

		auto newActiveLayerIndex = layerIndexOrder[index];
		layerIndexOrder.erase(layerIndexOrder.begin() + index);
		layerIndexOrder.insert(layerIndexOrder.begin(), newActiveLayerIndex);
		activeLayerIndex = newActiveLayerIndex;
	}

	void Canvas::Draw()
	{
		for (int i = numberOfLayers - 1; i >= 0; i--)
		{
			auto layerIndex = layerIndexOrder[i];
			layers[layerIndex].Draw();
		}
	}

	void Canvas::Undo()
	{
		layers[activeLayerIndex].Undo();
	}

	void Canvas::AddShape(shared_ptr<TouchShape> shape)
	{
		layers[activeLayerIndex].AddDrawable(shape);
	}

	float Canvas::GetAlpha(int index) const
	{
		assert(index >= 0 && index < numberOfLayers, "Cannot get alpha of layer, index out of range");

		auto actualIndex = layerIndexOrder[index];
		return layers[actualIndex].GetAlpha();
	}

	void Canvas::SetAlpha(int index, float value)
	{
		assert(index >= 0 && index < numberOfLayers, "Cannot set alpha of layer, index out of range");

		auto actualIndex = layerIndexOrder[index];
		layers[actualIndex].SetAlpha(value);
	}

	gl::Texture2dRef Canvas::GetLayerTexture(int index) const
	{
		assert(index >= 0 && index < numberOfLayers, "Cannot get texture of layer, index out of range");

		auto actualIndex = layerIndexOrder[index];
		return layers[actualIndex].GetFrameBufferTexture();
	}
}}
