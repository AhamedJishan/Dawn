#include "Renderer.h"

#include <glad/glad.h>
#include "Core/Application.h"
#include "Platform/Window.h"

namespace Dawn
{
	Renderer::Renderer(class Application* application)
		:mApplication(application)
	{
	}

	Renderer::~Renderer()
	{
	}
	
	bool Renderer::Init()
	{
		// Assumes a valid Opengl context is already initialised
		// This was done in Window::Init()

		int x, y;
		mApplication->GetWindow()->GetFrameBufferSize(x, y);
		glViewport(0, 0, x, y);

		mApplication->GetWindow()->SetFrameBufferSizeCallback([this](int width, int height) { glViewport(0, 0, width, height);});

		// TODO: Set Opengl state, like depth testing, etc.

		return true;
	}
	
	void Renderer::Draw()
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: Draw the scene
	}
}