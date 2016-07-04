#include "Canvas.h"

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
		assert(index >= 0 && index < numberOfLayers);

		auto layerIndexOrderBeggining = layerIndexOrder.begin();
		auto newActiveLayerIndex = *layerIndexOrder.erase(layerIndexOrderBeggining + index);
		layerIndexOrder.insert(layerIndexOrderBeggining, newActiveLayerIndex);
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
}}
