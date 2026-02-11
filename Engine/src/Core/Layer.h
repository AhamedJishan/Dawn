#pragma once

namespace Dawn
{
	class Layer
	{
	public:
		Layer() {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
	};
}