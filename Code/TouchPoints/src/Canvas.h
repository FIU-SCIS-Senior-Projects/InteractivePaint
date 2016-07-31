#pragma once
#include <vector>
#include "Layer.h"

using namespace std;

namespace touchpoints { namespace drawing
{
	struct IndexActive
	{
		IndexActive() {}
		IndexActive(int index, bool active) : index(index), active(active) {}
		int index;
		bool active;
	};

	class Canvas
	{
	public:
		Canvas();
		Canvas(int windowWidth, int windowHeight, int numberOfLayers, int maxNumberOfLayers = 3);
		void SetWindowWidth(int width);
		void SetWindowHeight(int height);
		void SetWindowDimensions(int width, int height);
		void MakeLayerActive(int index);
		void Draw();
		void Undo();
		void AddShape(shared_ptr<TouchShape> shape);
		void AddSymmetricShapes(pair<shared_ptr<TouchShape>, shared_ptr<TouchShape>> symmetricShapes);
		void DeleteLayer(int index);
		void AddLayer();
		inline int GetNumberOfLayers() const { return numberOfLayers; }
		inline float GetTopAlpha() const { return layers[activeLayerIndex].GetAlpha(); }
		inline void SetTopAlpha(float value) { layers[activeLayerIndex].SetAlpha(value); }
		float GetAlpha(int index) const;
		void SetAlpha(int index, float value);
		inline gl::Texture2dRef GetTopLayerTexture() const { return layers[activeLayerIndex].GetFrameBufferTexture(); }
		gl::Texture2dRef GetLayerTexture(int index) const;
	private:
		int numberOfLayers;
		int maxNumberOfLayers;
		int activeLayerIndex;
		vector<IndexActive> layerIndexActive;
		vector<Layer> layers;
	};
}}