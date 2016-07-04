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
	private:
		int numberOfLayers;
		int activeLayerIndex;
		vector<int> layerIndexOrder;
		vector<Layer> layers;
	};
}}