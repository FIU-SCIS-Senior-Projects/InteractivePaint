#pragma once
#include <vector>
#include "Layer.h"

using namespace std;

namespace touchpoints { namespace drawing
{
	class Canvas
	{
	public:
		Canvas();
		Canvas(int windowWidth, int windowHeight, int numberOfLayers);
		void SetWindowWidth(int width);
		void SetWindowHeight(int height);
		void SetWindowDimensions(int width, int height);
		void MakeLayerActive(int index);
		void Draw();
		void Undo();
		void AddShape(shared_ptr<TouchShape> shape);
		inline float GetTopAlpha() { return layers[activeLayerIndex].GetAlpha(); }
		inline void SetTopAlpha(float value) { layers[activeLayerIndex].SetAlpha(value); }
		float GetAlpha(int index);
		void SetAlpha(int index, float value);
	private:
		int numberOfLayers;
		int activeLayerIndex;
		vector<int> layerIndexOrder;
		vector<Layer> layers;
	};
}}