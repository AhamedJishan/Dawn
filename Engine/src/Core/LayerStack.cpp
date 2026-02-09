#include "LayerStack.h"

#include "Layer.h"

namespace Dawn
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers)
		{
			layer->OnDetach();
			delete layer;
		}
		mLayers.clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		mLayers.emplace_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer()
	{
		if (mLayers.empty())
			return;

		Layer* layer = mLayers.back();
		layer->OnDetach();
		delete layer;
		mLayers.pop_back();
	}

	void LayerStack::Update(float deltaTime)
	{
		for (Layer* layer : mLayers)
			layer->OnUpdate(deltaTime);
	}
}