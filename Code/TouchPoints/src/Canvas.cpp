#include "Canvas.h"
#include <cassert>

namespace touchpoints { namespace drawing
{
	Canvas::Canvas() {}

	Canvas::Canvas(int windowWidth, int windowHeight, int numberOfLayers, int maxNumberOfLayers)
		: activeLayerIndex(0)
	{
		assert(numberOfLayers > 0 && numberOfLayers <= maxNumberOfLayers, "Error in creating canvas");

		this->numberOfLayers = numberOfLayers;
		this->maxNumberOfLayers = maxNumberOfLayers;
		layerIndexActive = vector<IndexActive>(numberOfLayers);
		layers = vector<Layer>(numberOfLayers);
		for (int i = 0; i < numberOfLayers; i++)
		{
			layerIndexActive[i] = IndexActive(i, true);
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

		auto newActiveLayerIndex = layerIndexActive[index];
		layerIndexActive.erase(layerIndexActive.begin() + index);
		layerIndexActive.insert(layerIndexActive.begin(), newActiveLayerIndex);
		activeLayerIndex = newActiveLayerIndex.index;
	}

	void Canvas::Draw()
	{
		for (int i = numberOfLayers - 1; i >= 0; i--)
		{
			auto layerIndexAndActive = layerIndexActive[i];
			bool layerIsActive = layerIndexAndActive.active;
			int layerIndex = layerIndexAndActive.index;
			if(layerIsActive)
			{
				layers[layerIndex].Draw();
			}
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

	void Canvas::AddSymmetricShapes(pair<shared_ptr<TouchShape>, shared_ptr<TouchShape>> symmetricShapes)
	{
		layers[activeLayerIndex].AddSymmetricDrawables(symmetricShapes);
	}

	void Canvas::DeleteLayer(int index)
	{
		assert(index >= 0 && index < numberOfLayers, "Cannot delete layer, index out of range");

		auto indexActive = layerIndexActive[index];
		layers[indexActive.index].ClearLayer();
		//make layer inactive
		indexActive.active = false;
		//move indexactive down to bottom of list
		layerIndexActive.erase(layerIndexActive.begin() + index);
		layerIndexActive.insert(layerIndexActive.end(), indexActive);

		numberOfLayers--;
	}

	void Canvas::AddLayer()
	{
		assert(numberOfLayers + 1 <= maxNumberOfLayers, "Adding too many layers");

		layerIndexActive[numberOfLayers].active = true;

		numberOfLayers++;
	}

	float Canvas::GetAlpha(int index) const
	{
		assert(index >= 0 && index < numberOfLayers, "Cannot get alpha of layer, index out of range");

		auto actualIndex = layerIndexActive[index].index;
		return layers[actualIndex].GetAlpha();
	}

	void Canvas::SetAlpha(int index, float value)
	{
		assert(index >= 0 && index < numberOfLayers, "Cannot set alpha of layer, index out of range");

		auto actualIndex = layerIndexActive[index].index;
		layers[actualIndex].SetAlpha(value);
	}

	gl::Texture2dRef Canvas::GetLayerTexture(int index) const
	{
		assert(index >= 0 && index < numberOfLayers, "Cannot get texture of layer, index out of range");

		auto actualIndex = layerIndexActive[index].index;
		return layers[actualIndex].GetFrameBufferTexture();
	}
}}
