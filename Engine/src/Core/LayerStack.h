#pragma once

#include <vector>

namespace Dawn
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(class Layer* layer);
		void PopLayer();

		void Update(float deltaTime);
		void ImGuiRender();

	private:
		std::vector<class Layer*> mLayers;
	};
}