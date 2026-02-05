#pragma once

namespace Dawn
{
	class Renderer
	{
	public:
		Renderer(class Application* application);
		~Renderer();

		bool Init();

		void Draw();

	private:
		class Application* mApplication;
	};
}